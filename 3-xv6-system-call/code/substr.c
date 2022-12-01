#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char * argv[]) {
  int start, len;
  char *str = (char *) argv[1];
  start = atoi(argv[2]);
  len = atoi(argv[3]);

  printf(1, "%s\n", substr(str, start, len));

  exit();
}