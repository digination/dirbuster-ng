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
    while ((c = getopt(argc, argv, "hqvw:d:n:")) != -1) {
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


int load_dict() {
  return 0;
}


void* dbng_engine(void* queue_arg) {

  struct queue* db_queue = (struct queue*) queue_arg;
  extern dbng_config conf0;
  char * url;
  CURL *curl;
  CURLcode response;
  char *wl;
  curl = curl_easy_init();

  while(db_queue->head) {

    //we lock the queue's mutex, get the entry then unlock
	pthread_mutex_lock(db_queue->mutex);
	//memcpy sur entry -> wl sinon pb race cond ??
    wl = db_queue->head->entry;
    queue_rem(db_queue);
	pthread_mutex_unlock(db_queue->mutex);

    //we construct the url given host and wl
	url = (char*) malloc((strlen(conf0.host) + strlen(wl) +2) * sizeof(char) );
	strncpy(url,conf0.host,strlen(conf0.host));
	strncat(url,"/",1 * sizeof(char));  
	strncat(url,wl,strlen(wl));
	
    curl_easy_setopt(curl, CURLOPT_URL,url);
    response = curl_easy_perform(curl);
    if (response != 404 ) {
      output("FOUND %s (response code %d)\n",url,response);    
    }

	curl_easy_cleanup(curl);
    free(url);
  }		
}



int init_workers() {
  extern dbng_config conf0;
  conf0.workers = (pthread_t*) malloc(conf0.nb_workers * sizeof(pthread_t));
}


int init_workloads(struct queue* db_queue) {
	
	extern dbng_config conf0;
	if (NULL == conf0.dict) {
      
	}
}

int usage() {

  printf("Usage: dirbuster-ng [options...] <url>\n\
Options:\n -w <workers>\tDefines the number of threads to use to make the attack\n\
 -d <dict>\tLoads an external textfile to use as a dictionary\n\
 -q\t Enable quiet mode (relevent only with the -W flag)\n\
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

  conf0.host = argv[argc -1];
	
  db_queue = queue_new();
  db_queue->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(db_queue->mutex,NULL);

  parse_arguments(argc,argv);
  if (!conf0.quiet) version();
  //debug
  printf("WORKERS:%d\nHOST:%s\nQUIET:%d",
         conf0.nb_workers,
         conf0.host,
         conf0.quiet);
  init_workers();	
}
