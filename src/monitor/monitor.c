#include "../../include/codexion.h"

static int  check_burnout(t_sim *sim)
{
    int     i;
    int     n;
    long    now;
    long    deadline;
    long    last_start;

    n = sim->args.number_of_coders;
    i = 0;
    while (i < n)
    {
        pthread_mutex_lock(&sim->coders[i].data_lock);
        last_start = sim->coders[i].last_compile_start;
        pthread_mutex_unlock(&sim->coders[i].data_lock);
        now = get_timestamp_ms();
        deadline = last_start + sim->args.time_to_burnout;
        if (now >= deadline)
        {
            log_action(sim, sim->coders[i].id, "burned out");
            return (1);
        }
        i++;
    }
    return (0);
}

static int  check_all_done(t_sim *sim)
{
    int i;
    int n;
    int done;

    n = sim->args.number_of_coders;
    i = 0;
    while (i < n)
    {
        pthread_mutex_lock(&sim->coders[i].data_lock);
        done = sim->coders[i].compiles_done;
        pthread_mutex_unlock(&sim->coders[i].data_lock);
        if (done < sim->args.number_of_compiles_required)
            return (0);
        i++;
    }
    return (1);
}

void    *monitor_routine(void *arg)
{
    t_sim   *sim;

    sim = (t_sim *)arg;
    while (1)
    {
        pthread_mutex_lock(&sim->stop_lock);
        if (sim->stop)
        {
            pthread_mutex_unlock(&sim->stop_lock);
            break ;
        }
        pthread_mutex_unlock(&sim->stop_lock);
        if (check_burnout(sim))
        {
            pthread_mutex_lock(&sim->stop_lock);
            sim->stop = 1;
            pthread_mutex_unlock(&sim->stop_lock);
            break ;
        }
        if (check_all_done(sim))
        {
            pthread_mutex_lock(&sim->stop_lock);
            sim->stop = 1;
            pthread_mutex_unlock(&sim->stop_lock);
            break ;
        }
        usleep(1000);
    }
    return (NULL);
}
