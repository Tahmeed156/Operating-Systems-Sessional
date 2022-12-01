#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  printf(1, "xv6: Fetching process size!\n");
  printf(1, "Using %d bytes\n", getsize());
  exit();
}