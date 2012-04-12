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
    while ((c = getopt(argc, argv, "qvw:d:h:n:")) != -1) {
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
				conf0.host = optarg;
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
  char * url;
  CURL *curl;
  CURLcode response;	

  curl = curl_easy_init();

  while(db_queue->head) {
    curl_easy_setopt(curl, CURLOPT_URL,"http://google.com");
    response = curl_easy_perform(curl);
    if (response != 404 ) {
      output("FOUND %s (response code %d)\n",url,response);    
    }
  }		
}

int init_workers() {
  extern dbng_config conf0;
  conf0.workers = (pthread_t*) malloc(conf0.nb_workers * sizeof(pthread_t));
}


int version() {
  printf("{Dirbuster NG 0.1} (c)2012 WintermeW\n");
}


int main(int argc, char **argv)
{
  extern dbng_config conf0;
  parse_arguments(argc,argv);
  if (!conf0.quiet) version();
  //debug
  printf("WORKERS:%d\nHOST:%s\nQUIET:%d",
         conf0.nb_workers,
         conf0.host,
         conf0.quiet);
  init_workers();	
}
