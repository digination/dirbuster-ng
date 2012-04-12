#include <pthread.h>

typedef struct dbng_config {

  int nb_workers;
  uint8_t quiet;
  pthread_t* workers;
  char *dict;
  char* host;
  long timeout;
	
} dbng_config;
