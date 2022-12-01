#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void printUsage() {
  printf(1, "Usage:\n");
  printf(1, "incr <num>\n");
  printf(1, "adds 1 the the given argument\n");
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    printf(1, "Invalid number of arguments\n");
    printUsage();
  } else {
    printf(1, "%d\n", incr(atoi(argv[1])));
  }
  exit();
}