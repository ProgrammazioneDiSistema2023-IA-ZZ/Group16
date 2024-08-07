#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "semaphore.h"
#include "proc.h"

struct semaphore sems[10];  //semaphores for user's programs

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sigalarm(void)
{
  int ticks;
  uint64 addr;

  argint(0, &ticks);
  argaddr(1, &addr);

  myproc()->ticks = ticks;
  myproc()->handler = addr;
  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p = myproc();
  memmove(p->trapframe, p->alarm_tf, PGSIZE);   // restore the user trapframe from the saved alarm trapframe
  kfree(p->alarm_tf);                           // free the memory allocated for the saved alarm trap trapframe.
  p->cur_ticks = 0;
  p->alarm_on = 0;
  return 0;
}

uint64
sys_sem_init(void)
{
  int sem, count;
  argint(0, &sem);
  argint(1, &count);
  if (sem < 0 || sem > 10)
    return -1;
  initsemaphore(&sems[sem], "usersem", count);
  return 0;
}

uint64
sys_sem_wait(void)
{
  int sem;
  argint(0, &sem);
  if (sem < 0 || sem > 10)
    return -1;
  P(&sems[sem]);
  return 0;
}

uint64
sys_sem_signal(void)
{
  int sem;
  argint(0, &sem);
  if (sem < 0 || sem > 10)
    return -1;
  V(&sems[sem]);
  return 0;
}
