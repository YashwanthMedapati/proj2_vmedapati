#include <stdlib.h>
#include "mybarrier.h"


mybarrier_t * mybarrier_init(unsigned int count)
{
    mybarrier_t * barrier = malloc(sizeof(*barrier));

    /* Implementation */

	if (NULL == barrier) {
        return NULL;
    }
	
	barrier->count    = (int)count;
    barrier->arrived  = 0;
    barrier->released = 0;

    if (pthread_mutex_init(&barrier->mutex, NULL) != 0) {
        free(barrier);
        return NULL;
    }

    if (pthread_cond_init(&barrier->cond, NULL) != 0) {
        pthread_mutex_destroy(&barrier->mutex);
        free(barrier);
        return NULL;
    }
    
    return barrier;	
}

int mybarrier_destroy(mybarrier_t * barrier)
{
	int ret = 0;
	/* Implementation */

	if (barrier == NULL) {
        return -1;
    }

	if (pthread_mutex_lock(&barrier->mutex) != 0) {
        return -1;
    }
	while (barrier->arrived > 0) {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }

    pthread_mutex_unlock(&barrier->mutex);

    if (pthread_mutex_destroy(&barrier->mutex) != 0) {
        ret = -1;
    }

    if (pthread_cond_destroy(&barrier->cond) != 0) {
        ret = -1;
    }
	free(barrier);
	
	return ret;
}

int mybarrier_wait(mybarrier_t * barrier)
{
    if (barrier == NULL) {
        return -1;
    }

    if (pthread_mutex_lock(&barrier->mutex) != 0) {
        return -1;
    }

    if (barrier->released) {
        pthread_mutex_unlock(&barrier->mutex);
        return -1;
    }
    barrier->arrived++;

    if (barrier->arrived == barrier->count) {
        barrier->released = 1;
        pthread_cond_broadcast(&barrier->cond);
    } 
	else {
        while (!barrier->released) {
            pthread_cond_wait(&barrier->cond, &barrier->mutex);
        }
    }
    barrier->arrived--;

    if (barrier->released && barrier->arrived == 0) {
        pthread_cond_signal(&barrier->cond);
    }

    pthread_mutex_unlock(&barrier->mutex);
    return 0;
}
