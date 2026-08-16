#include "../../include/codexion.h"

void    do_compile(t_coder *coder)
{
    take_dongles(coder);
    pthread_mutex_lock(&coder->data_lock);
    coder->last_compile_start = get_timestamp_ms();
    pthread_mutex_unlock(&coder->data_lock);
    log_action(coder->sim, coder->id, "is compiling");
    usleep(coder->sim->args.time_to_compile * 1000);
    release_dongles(coder);
    pthread_mutex_lock(&coder->data_lock);
    coder->compiles_done++;
    pthread_mutex_unlock(&coder->data_lock);
}
