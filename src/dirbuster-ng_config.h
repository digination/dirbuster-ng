#include <pthread.h>

typedef struct dbng_config {

  int nb_workers;
  char external_dict[1024];
  uint8_t quiet;
  pthread_t* workers;
  char* host;
	
} dbng_config;
