#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  printf(1, "[pgf] Test forked paging\n");
  int i, j, pid = 99;

  for (i = 0; i < 5; i++) {
    // printf(1, "sbrk 4096 (%dth time)\n", i);
    sbrk(4096);
  }

  pid = fork();

  if (pid == 0) {
    for (j = 0; j < 10; j++) {
      // printf(1, "[%d] sbrk 4096 (%dth time)\n", pid, i + j);
      sbrk(4096);
    }
  }

  for (j = 0; j < 10; j++) {
    // printf(1, "[%d] sbrk 4096 (%dth time)\n", pid, i + j);
    sbrk(4096);
  }

  // Wait for children
  while(wait() != -1);
  if (pid != 0)
    printf(1, "[pg] Test successful (%d pgfaults handled)\n", getpgfaults());
  exit();
}