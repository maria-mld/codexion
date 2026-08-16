#include "../../include/codexion.h"

static int should_stop(t_sim *sim)
{
    int stop;

    pthread_mutex_lock(&sim->stop_lock);
    stop = sim->stop;
    pthread_mutex_unlock(&sim->stop_lock);
    return (stop);
}

void    *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while (!should_stop(coder->sim))
    {
        do_compile(coder);
        if (should_stop(coder->sim))
            break ;
        do_debug(coder);
        if (should_stop(coder->sim))
            break ;
        do_refactor(coder);
    }
    return (NULL);
}