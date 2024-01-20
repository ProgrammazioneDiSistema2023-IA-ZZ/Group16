#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ticks;

  if(argc != 2){
    fprintf(2, "Usage: sleep <n ticks>\n");
    exit(1);
  }

  ticks = atoi(argv[1]);

  if(sleep(ticks) < 0){
    fprintf(2, "sleep failed\n");
    exit(-1);
  }
  
  exit(0);
}
