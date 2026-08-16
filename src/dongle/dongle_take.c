#include "../../include/codexion.h"


static void wait_cooldown(t_dongle *d, long cooldown)
{
    long    now;
    long    elapsed;

    now = get_timestamp_ms();
    elapsed = now - d->free_since;
    while (d->free_since != 0 && elapsed < cooldown)
    {
        usleep((cooldown - elapsed) * 1000);
        now = get_timestamp_ms();
        elapsed = now - d->free_since;
    }
}

// static void take_one_dongle(t_dongle *d, t_sim *sim)
// {
//     pthread_mutex_lock(&d->lock);
//     while (d->is_taken)
//         pthread_cond_wait(&d->cond, &d->lock);
//     wait_cooldown(d, sim->args.dongle_cooldown);
//     d->is_taken = 1;
//     pthread_mutex_unlock(&d->lock);
// }

static long compute_key(t_coder *coder)
{
    if (coder->sim->args.scheduler == SCHED_FIFO_MODE)
        return (get_timestamp_ms());
    return (coder->last_compile_start + coder->sim->args.time_to_burnout);
}

static void take_one_dongle(t_dongle *d, t_coder *coder)
{
    long    key;

    pthread_mutex_lock(&d->lock);
    key = compute_key(coder);
    if (d->is_taken || d->wait_queue.size > 0)
    {
        heap_push(&d->wait_queue, coder->id, key);
        while (1)
        {
            pthread_cond_wait(&d->cond, &d->lock);
            if (d->next_turn_id == coder->id)
                break ;
        }
    }
    wait_cooldown(d, coder->sim->args.dongle_cooldown);
    d->is_taken = 1;
    d->next_turn_id = -1;
    pthread_mutex_unlock(&d->lock);
}

void    take_dongles(t_coder *coder)
{
    t_dongle    *first;
    t_dongle    *second;

    if (coder->left->id == coder->right->id)
    {
        take_one_dongle(coder->left, coder);
        log_action(coder->sim, coder->id, "has taken a dongle");
        return ;
    }
    if (coder->left->id < coder->right->id)
    {
        first = coder->left;
        second = coder->right;
    }
    else
    {
        first = coder->right;
        second = coder->left;
    }
    take_one_dongle(first, coder);
    log_action(coder->sim, coder->id, "has taken a dongle");
    take_one_dongle(second, coder);
    log_action(coder->sim, coder->id, "has taken a dongle");
}
