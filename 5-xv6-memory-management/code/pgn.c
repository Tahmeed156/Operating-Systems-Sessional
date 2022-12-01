#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  printf(1, "[pg] Test paging\n");
  int i, n = 15000, x = 10000;

  printf(1, "[pg] malloc start\n");
  int *arr = malloc(n * sizeof(int));
  printf(1, "[pg] malloc done\n");

  for (int i = 0; i < n; i++) {
    arr[i] = i;
  }

  printf(1, "[pg] sbrk start\n");
  sbrk(x * sizeof(int));
  printf(1, "[pg] sbrk done\n");
  printf(1, "[pg] Checking start\n");

  // fork(); // Test with forking. Works.

  for (i = 0; i < n; i++) {
    if (arr[i] !=  i) {
      printf(1, "%d did not match\n", i);
      exit();
    }
  }

  // while(wait() != -1); // Test with forking. Works.
  printf(1, "[pg] Test successful (%d pgfaults handled)\n", getpgfaults());
  exit();
}