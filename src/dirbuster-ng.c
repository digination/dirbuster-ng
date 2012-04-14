#include "dirbuster-ng.h"

/*we split the argument parsing from main()
because it's more clear */
int parse_arguments(int argc, char **argv)
{
    extern dbng_config conf0;
    char *cvalue = NULL;
    int index;
    int c;
    int opterr = 0;	
    while ((c = getopt(argc, argv, "hqvw:d:n:t:X:")) != -1) {
		switch (c) {
			case 'v':
		  		return;
			case 'w':
    	    	conf0.nb_workers = atoi(optarg);
		    	break;
			case 'q':
				conf0.quiet = 1;
		    	break;
			case 'h':
				usage();
		    	break;
		    case 'd':
		      conf0.dict = optarg;
			  break;
			case 't':
			  conf0.timeout = atoi(optarg);
			  break;
			case 'X':
			  conf0.proxy = optarg;
			default:
    			break;
		}
    }
}

int output(char* fmt,...) 
{
  va_list args;
  extern dbng_config conf0;
  if (conf0.quiet) return -1;	
  va_start(args,fmt);
  vprintf(fmt,args);
  va_end(args);
}

static size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  (void)ptr;
  (void)data;
  return (size_t)(size * nmemb);
}

void* dbng_engine(void* queue_arg) 
{
  struct queue* db_queue = (struct queue*) queue_arg;
  extern dbng_config conf0;
  char * url;
  CURL *curl;
  CURLcode response;
  char *wl;
  int final_url_len = 0;
  int wl_len = 0;
  long http_code;

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT,conf0.timeout);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1);

  if (conf0.proxy != NULL) {
    curl_easy_setopt(curl, CURLOPT_PROXY,conf0.proxy);
  }
	
  while(db_queue->head) {
	  
	pthread_mutex_lock(db_queue->mutex);
	wl_len = strlen(db_queue->head->entry)+1;
    wl = (char*) malloc (wl_len * sizeof(char));
	setZeroN(wl,wl_len);
	strncpy(wl,db_queue->head->entry,strlen(db_queue->head->entry));
    queue_rem(db_queue);
	pthread_mutex_unlock(db_queue->mutex);

    //we construct the url given host and wl
	final_url_len = strlen(conf0.host) + strlen(wl) +2;
	url = (char*) malloc(final_url_len * sizeof(char) );
	setZeroN(url,final_url_len);
	strncpy(url,conf0.host,strlen(conf0.host));
	strncat(url,"/",1 * sizeof(char));  
	strncat(url,wl,strlen(wl));
    free(wl);
	  
    curl_easy_setopt(curl, CURLOPT_URL,url);
    response = curl_easy_perform(curl);
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code == 200 || http_code == 403) {
      output("FOUND %s (response code %d)\n",trim(url),http_code);
    }  
    free(url);
  }		
  curl_easy_cleanup(curl);
}

int load_dict(struct queue* db_queue) {
	
  extern dbng_config conf0;	
  FILE* dict_fh;
  char * buffer = (char*) malloc(4096 * sizeof(char));
  dict_fh = fopen(conf0.dict,"r");

  if (!dict_fh) {
    fprintf(stderr,"ERROR: cannot open dictionary, quitting..\n");
	exit(1);
  }
	
  while (!feof(dict_fh)) {
    fgets(buffer,4096*sizeof(char),dict_fh);
	//handling of recursion ?? 
    //( dup the queue to keep an initial copy, save found dirs)
    queue_add(db_queue,buffer);
  }
	
}


int init_config(dbng_config* conf0) {
  conf0->quiet = 0;
  conf0->nb_workers = DEFAULT_WORKERS;
  conf0->timeout = DEFAULT_TIMEOUT;
  conf0->host = NULL;
  conf0->dict = NULL;
  conf0->proxy = NULL;
}

int init_workers(struct queue* db_queue) {
	
  extern dbng_config conf0;
  int i;
  int ret;
  conf0.workers = (pthread_t*) malloc(conf0.nb_workers * sizeof(pthread_t));
  for (i=0;i< conf0.nb_workers;i++) {
    ret = pthread_create(&conf0.workers[i],NULL,dbng_engine,(void*) db_queue);
  }
	
}

int init_workloads(struct queue* db_queue) {
	
	extern dbng_config conf0;
	extern const char* def_dict[];
	extern int dd_nbentries;
	int i;

	//no external dictionary defined, using the internal
	if (NULL == conf0.dict) {
      for(i=0;i<dd_nbentries;i++) {
	    queue_add(db_queue,(char*)def_dict[i]);
	  }
	}

    else {
      load_dict(db_queue);
	}
}

int usage() {

  printf("Usage: dirbuster-ng [options...] <url>\n\
Options:\n -w <nb_threads>\tDefines the number of threads to use to make the attack\n\
 -d <dict>\tLoads an external textfile to use as a dictionary\n\
 -t <seconds>\tSets the timeout in seconds for each http query\n\
 -q\t Enable quiet mode (relevent only with the -W flag)\n\
 -X <proxy_server:port>\tUse an HTTP proxy server to perform the queries\n\
 -h\t Prints this help then exits\n\
 -v\t Prints the program version then exits\n");

  exit(0);
}

int version() {
  printf("{Dirbuster NG 0.1} (c)2012 WintermeW\n");
}


int main(int argc, char **argv)
{
  extern dbng_config conf0;
  struct queue* db_queue;
	
  if (argc < 2) usage();

  init_config(&conf0);
  conf0.host = argv[argc -1];	
  db_queue = queue_new();
  db_queue->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(db_queue->mutex,NULL);

  parse_arguments(argc,argv);
  if (!conf0.quiet) version();

  init_workloads(db_queue);
  init_workers(db_queue);
	
  for(;;) {
	  sleep(.1);
	  if (! db_queue->head) break;
  }
	
}
