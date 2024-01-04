/* 
 * Program that uses Unix system calls to "ping-pong" a byte between 
 * two processes over a pair of pipes, one for each direction.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pipe_parent_to_child[2];
  int pipe_child_to_parent[2];

  if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
    fprintf(2, "Error creating pipes\n");
    exit(-1);
  }

  int child_pid = fork();
  if (child_pid == -1) {
    fprintf(2, "Error forking\n");
    exit(-1);
  }

  if (child_pid == 0) {
    // Child process
    close(pipe_parent_to_child[1]); // Close write end from parent to child
    close(pipe_child_to_parent[0]); // Close read end from child to parent

    char byte;
    read(pipe_parent_to_child[0], &byte, sizeof(byte));
    fprintf(1, "%d: received ping\n", getpid());

    write(pipe_child_to_parent[1], &byte, sizeof(byte));
    
    close(pipe_parent_to_child[0]);
    close(pipe_child_to_parent[1]);
    exit(0);
  } else {
    // Parent process
    close(pipe_parent_to_child[0]); // Close read end from parent to child
    close(pipe_child_to_parent[1]); // Close write end from child to parent

    char byte = 'O';
    
    write(pipe_parent_to_child[1], &byte, sizeof(byte));
    wait(0);

    read(pipe_child_to_parent[0], &byte, sizeof(byte));
    fprintf(1, "%d: received pong\n", getpid());

    close(pipe_parent_to_child[1]);
    close(pipe_child_to_parent[0]);
  }
  exit(0);
}
