#include "../include/codexion.h"

int main(int argc, char **argv)
{
    t_args  args;
    t_sim   sim;
    int     i;
    int     n;

    if (!parse_args(argc, argv, &args))
        return (1);
    if (!init_sim(&sim, &args))
    {
        fprintf(stderr, "Error: initialization failed\n");
        return (1);
    }
    sim.start_time = get_timestamp_ms();
    n = sim.args.number_of_coders;
    i = 0;
    while (i < n)
        sim.coders[i++].last_compile_start = sim.start_time;
    if (pthread_create(&sim.monitor, NULL, monitor_routine, &sim) != 0)
    {
        fprintf(stderr, "Error: pthread_create (monitor) failed\n");
        cleanup_sim(&sim);
        return (1);
    }
    i = 0;
    while (i < n)
    {
        if (pthread_create(&sim.coders[i].thread, NULL,
                coder_routine, &sim.coders[i]) != 0)
        {
            fprintf(stderr, "Error: pthread_create failed\n");
            cleanup_sim(&sim);
            return (1);
        }
        i++;
    }
    i = 0;
    while (i < n)
        pthread_join(sim.coders[i++].thread, NULL);
    pthread_join(sim.monitor, NULL);
    cleanup_sim(&sim);
    return (0);
}