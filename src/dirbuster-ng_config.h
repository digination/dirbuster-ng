#include <pthread.h>
#include <stdio.h>

typedef struct dbng_config {

  int nb_workers;
  uint8_t quiet;
  uint8_t disable_ncurses;
  pthread_t* workers;
  char *dict;
  char* host;
  long timeout;
  char* proxy;
  char* uagent;
  char* proxy_auth;
  char* http_auth;
  FILE* output_file;	

} dbng_config;
