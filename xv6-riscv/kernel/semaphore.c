//Semaphore

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "semaphore.h"

void
initsemaphore(struct semaphore *sem, char *name, int initial_count)
{
    sem->name = name;
    sem->count = initial_count;
    initlock(&sem->lock, "semaphore lock");
}

void
P(struct semaphore *sem)
{
    acquire(&sem->lock);
    while(sem->count == 0)
        sleep(sem, &sem->lock);
    sem->count--;
    release(&sem->lock);
}

void
V(struct semaphore *sem)
{
    acquire(&sem->lock);
    sem->count++;
    wakeup(sem);
    release(&sem->lock);
}