#include "kernel/types.h"
#include "user/user.h"

void 
primes(int left)
{
  int n;
  int p;
  if (!read(left, &p, 4)) {
    return;
  }
  printf("prime %d\n", p);

  int right[2];
  int num;
  pipe(right);
  if (fork() == 0) {
    close(left);
    close(right[1]);
    primes(right[0]);
    close(right[0]);
    exit();
  } else {
    close(right[0]);
    while((n = read(left, &num, 4)) > 0){
      if ((num % p) == 0) {
        continue;
      }
      write(right[1], &num, 4);
    }
    close(left);
    close(right[1]);
  }
}

int 
main()
{
  int left[2];
  int i;
  pipe(left);

  if (fork() == 0) {
    close(left[1]);
    primes(left[0]);
    exit();
  } else {
    close(left[0]);
    for (i = 2; i <= 35; i++) {
      write(left[1], &i, 4);
    }
    close(left[1]);
  }
  exit();
}
