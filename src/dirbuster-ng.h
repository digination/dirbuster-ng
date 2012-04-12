#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include "dirbuster-ng_config.h"
#include "dict.h"
#include "queue.h"
#include <curl/curl.h>


#define DEFAULT_WORKERS 8

dbng_config conf0;

