#include "../include/codexion.h"


static int  init_dongles(t_sim *sim)
{
    int i;
    int n;
 
    n = sim->args.number_of_coders;
    sim->dongles = malloc(sizeof(t_dongle) * n);
    if (!sim->dongles)
        return (0);
    i = 0;
    while (i < n)
    {
        sim->dongles[i].id = i;
        sim->dongles[i].is_taken = 0;
        sim->dongles[i].free_since = 0;
        if (pthread_mutex_init(&sim->dongles[i].lock, NULL) != 0)
            return (0);
        if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
            return (0);
        if (!heap_init(&sim->dongles[i].wait_queue, n))
            return (0);
        i++;
    }
    return (1);
}


static int init_coders(t_sim *sim)
{
    int i;
    int n;

    n = sim->args.number_of_coders;
    sim->coders = malloc(sizeof(t_coder) * n);
    if (!sim->coders)
        return (0);
    i = 0;
    while (i < n)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].compiles_done = 0;
        sim->coders[i].last_compile_start = 0;
        sim->coders[i].sim = sim;
        if (pthread_mutex_init(&sim->coders[i].data_lock, NULL) != 0)
            return (0);
        if (n == 1)
        {
            sim->coders[i].left = &sim->dongles[0];
            sim->coders[i].right = &sim->dongles[0];
        }
        else
        {
            sim->coders[i].left = &sim->dongles[i];
            sim->coders[i].right = &sim->dongles[(i + 1) % n];
        }
        i++;
    }
    return (1);
}


int init_sim(t_sim *sim, t_args *args)
{
    memset(sim, 0, sizeof(t_sim));
    sim->args = *args;
    sim->stop = 0;
    if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&sim->log_lock, NULL) != 0)
        return (0);
    if (!init_dongles(sim))
        return (0);
    if (!init_coders(sim))
        return (0);
    return (1);
}

void    cleanup_sim(t_sim *sim)
{
    int i;
    int n;

    n = sim->args.number_of_coders;
    if (sim->dongles)
    {
        i = 0;
        while (i < n)
        {
            pthread_mutex_destroy(&sim->dongles[i].lock);
            pthread_cond_destroy(&sim->dongles[i].cond);
            heap_destroy(&sim->dongles[i].wait_queue);
            i++;
        }
        free(sim->dongles);
    }
    if (sim->coders)
    {
        i = 0;
        while (i < n)
        {
            pthread_mutex_destroy(&sim->coders[i].data_lock);
            i++;
        }
        free(sim->coders);
    }
    pthread_mutex_destroy(&sim->stop_lock);
    pthread_mutex_destroy(&sim->log_lock);
}
