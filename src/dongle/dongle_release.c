#include "../../include/codexion.h"


// static void release_one(t_dongle *d)
// {
//     pthread_mutex_lock(&d->lock);
//     d->is_taken = 0;
//     d->free_since = get_timestamp_ms();
//     pthread_cond_broadcast(&d->cond);
//     pthread_mutex_unlock(&d->lock);
// }

static void release_one(t_dongle *d)
{
    t_request   next;

    pthread_mutex_lock(&d->lock);
    d->is_taken = 0;
    d->free_since = get_timestamp_ms();
    if (!heap_is_empty(&d->wait_queue))
    {
        heap_pop(&d->wait_queue, &next);
        d->next_turn_id = next.coder_id;
    }
    else
        d->next_turn_id = -1;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->lock);
}

void release_dongles(t_coder *coder)
{
    if (coder->left->id == coder->right->id)
    {
        release_one(coder->left);
        return ;
    }
    release_one(coder->left);
    release_one(coder->right);
}