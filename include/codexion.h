#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include "types.h"

/* ---- Parsing ---- */
int     parse_args(int argc, char **argv, t_args *args);

/* ---- Utils ---- */
long    get_timestamp_ms(void);
void    log_action(t_sim *sim, int coder_id, const char *action);

/* ---- Init / cleanup ---- */
int     init_sim(t_sim *sim, t_args *args);
void    cleanup_sim(t_sim *sim);

/* ---- Dongles ---- */
void    take_dongles(t_coder *coder);
void    release_dongles(t_coder *coder);

/* ---- Coder lifecycle ---- */
void    do_compile(t_coder *coder);
void    do_debug(t_coder *coder);
void    do_refactor(t_coder *coder);
void    *coder_routine(void *arg);

/* ---- Monitor ---- */
void    *monitor_routine(void *arg);

/* ---- Scheduler (heap) ---- */
int     heap_init(t_heap *h, int capacity);
void    heap_destroy(t_heap *h);
int     heap_push(t_heap *h, int coder_id, long key);
int     heap_pop(t_heap *h, t_request *out);
int     heap_is_empty(t_heap *h);

#endif
