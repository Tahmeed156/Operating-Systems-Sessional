#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  printf(1, "QEMU: Terminated\n");
  shutdown();
  exit();
}