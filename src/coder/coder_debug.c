#include "../../include/codexion.h"

void    do_debug(t_coder *coder)
{
    log_action(coder->sim, coder->id, "is debugging");
    usleep(coder->sim->args.time_to_debug * 1000);
}

