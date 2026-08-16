#include "../include/codexion.h"


static int is_valid_number(const char *str)
{
    int i;

    i = 0;
    if (!str || !str[0])
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

static long ft_atol(const char *str)
{
    long    result;
    int     i;

    result = 0;
    i = 0;
    while (str[i])
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result);
}

static int parse_positive_long(const char *str, long *out)
{
    if (!is_valid_number(str))
        return (0);
    *out = ft_atol(str);
    if (*out <= 0)
        return (0);
    return (1);
}


static int  parse_scheduler(const char *str, int *out)
{
    if (strcmp(str, "fifo") == 0)
    {
        *out = SCHED_FIFO_MODE;
        return (1);
    }
    if (strcmp(str, "edf") == 0)
    {
        *out = SCHED_EDF_MODE;
        return (1);
    }
    return (0);
}


int parse_args(int argc, char **argv, t_args *args)
{
    long    tmp;
    if (argc != 9)
    {
        fprintf(stderr, "Error: expected 8 arguments\n");
        return (0);
    }
    if (!parse_positive_long(argv[1], &tmp) || tmp > 2147483647)
    {
        fprintf(stderr, "Error: invalid number_of_coders\n");
        return (0);
    }
    args->number_of_coders = (int)tmp;
    if (!parse_positive_long(argv[2], &args->time_to_burnout))
        return (fprintf(stderr, "Error: invalid time_to_burnout\n"), 0);
    if (!parse_positive_long(argv[3], &args->time_to_compile))
        return (fprintf(stderr, "Error: invalid time_to_compile\n"), 0);
    if (!parse_positive_long(argv[4], &args->time_to_debug))
        return (fprintf(stderr, "Error: invalid time_to_debug\n"), 0);
    if (!parse_positive_long(argv[5], &args->time_to_refactor))
        return (fprintf(stderr, "Error: invalid time_to_refactor\n"), 0);
    if (!parse_positive_long(argv[6], &tmp) || tmp > 2147483647)
        return (fprintf(stderr, "Error: invalid number_of_compiles_required\n"), 0);

    args->number_of_compiles_required = (int)tmp;
    if (!parse_positive_long(argv[7], &args->dongle_cooldown))
        return (fprintf(stderr, "Error: invalid dongle_cooldown\n"), 0);
    if (!parse_scheduler(argv[8], &args->scheduler))
        return (fprintf(stderr, "Error: scheduler must be 'fifo' or 'edf'\n"), 0);
    return (1);
    
}