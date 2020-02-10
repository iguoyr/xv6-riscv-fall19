#include "kernel/types.h"
#include "user/user.h"

int 
main()
{
  int p[2];
  char buf[10];

  pipe(p);
  if (fork() == 0)
  {
    read(p[0], &buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    close(p[0]);
    write(p[1], "pong", 4);
    close(p[1]);
    exit();
  }
  else
  {
    write(p[1], "ping", 4);
    close(p[1]);
    wait();
    read(p[0], &buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    close(p[0]);
  }
  exit();
}