#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(2, "usage: sleep sleepTime\n");
    exit();
  }

  sleep(atoi(argv[1]));
  exit();
}
