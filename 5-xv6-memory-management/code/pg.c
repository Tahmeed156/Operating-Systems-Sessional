#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  printf(1, "[pg] Test paging\n");
  int i, numPages = 25;

  if(argc == 2) {
    numPages = atoi(argv[1]);
  }

  for (i = 0; i < numPages; i++) {
    printf(1, "sbrk 4096 (%dth time)\n", i);
    sbrk(4096);
  }

  printf(1, "[pg] Test successful (%d pgfaults handled)\n", getpgfaults());
  exit();
}