#ifndef __THREAD_SPAWN__
#define __THREAD_SPAWN__

void thread_spawn(void *new_thread(void *attr), void *args);

#endif
