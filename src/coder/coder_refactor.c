#include "../../include/codexion.h"

void    do_refactor(t_coder *coder)
{
    log_action(coder->sim, coder->id, "is refactoring");
    usleep(coder->sim->args.time_to_refactor * 1000);
}
