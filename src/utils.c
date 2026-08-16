#include "../include/codexion.h"


long    get_timestamp_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void log_action(t_sim *sim, int coder_id, const char *action)
{
    long    ts;

    pthread_mutex_lock(&sim->stop_lock);
    if (sim->stop && strcmp(action, "burned out") != 0)
    {
        pthread_mutex_unlock(&sim->stop_lock);
        return ;
    }
    pthread_mutex_unlock(&sim->stop_lock);

    ts = get_timestamp_ms() - sim->start_time;
    pthread_mutex_lock(&sim->log_lock);
    printf("%ld %d %s\n", ts, coder_id, action);
    pthread_mutex_unlock(&sim->log_lock);
}