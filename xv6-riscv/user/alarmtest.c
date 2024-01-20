//
// test program for the alarm feature
//

#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/stat.h"
#include "user/user.h"

void test0();
void test1();
void test2();
void periodic();
void slow_handler();

int
main(int argc, char *argv[])
{
  test0();
  test1();
  test2();
  exit(0);
}

volatile static int count;

void
periodic()
{
  count += 1;
  printf("alarm!\n");
  sigreturn();
}

//
// tests whether the kernel calls the alarm 
// handler even a single time
//
void
test0()
{
  int i;
  printf("test0 start\n");
  count = 0;
  sigalarm(2, periodic);
  for (i = 0; i < 1000 * 500000; i++) {
    if ((i % 1000000) == 0) {
      write(2, ".", 1);
    }
    if (count > 0) {
      break;
    }
  }
  sigalarm(0, 0);
  if (count > 0) {
    printf("test0 passed\n");
  } else {
    printf("\ntest0 failed: the kernel never called the alarm handler\n");
  }
}

void __attribute__ ((noinline)) foo(int i, int *j) {
  if((i % 2500000) == 0) {
    write(2, ".", 1);
  }
  *j += 1;
}

//
// tests whether the kernel calls the alarm
// handler multiple times
//
// tests that when the handler returns, it returns to
// the point in the program where the timer interrupt 
// occurred, with all registers holding the same values they
// held when the interrupt occurred
//
void
test1()
{
  int i;
  int j;

  printf("test1 start\n");
  count = 0;
  j = 0;
  sigalarm(2, periodic);
  for (i = 0; i < 500000000; i++) {
    if (count >= 10) {
      break;
    }
    foo(i, &j);
  }
  if (count < 10) {
    printf("\ntest1 failed: too few calls to the handler\n");
  } else if (i != j) {
    printf("\ntest1 failed: foo() executed fewer times than it was called\n");
  } else {
    printf("test1 passed: periodic was called %d times\n", count);
  }
}

//
// tests that the kernel does not allow reentrant alarm calls
// if the alarm handler doesn't return, a new one shouldn't be called
//
void
test2()
{
  int i;
  int pid;
  int status;

  printf("test2 start\n");
  if ((pid = fork()) < 0) {
    printf("test2: fork failed\n");
  }
  if (pid == 0) {
    count = 0;
    sigalarm(2, slow_handler);
    for (i = 0; i < 1000 * 500000; i++) {
      if ((i % 1000000) == 0) {
        write(2, ".", 1);
      }
      if (count > 0) {
        break;
      }
    }
    if (count == 0) {
      printf("\ntest2 failed: the kernel never called the alarm handler\n");
      exit(1);
    }
    exit(0);
  }
  wait(&status);
  if (status == 0) {
    printf("test2 passed\n");
  }
}

void
slow_handler()
{
  int i;
  count += 1;
  printf("alarm!\n");
  if (count > 1) {
    printf("test2 failed: alarm handler called more than once\n");
    exit(1);
  }
  for (i = 0; i < 1000 * 500000; i++) {
    asm volatile("nop");  // avoid compiler optimization away loop
  }
  sigalarm(0, 0);
  sigreturn();
}
