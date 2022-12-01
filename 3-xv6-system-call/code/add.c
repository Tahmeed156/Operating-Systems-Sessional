#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void printUsage() {
  printf(1, "Usage:\n");
  printf(1, "add <num1> <num2> [<num3>...]\n");
  printf(1, "returns the sum of arguments\n");
}

int main(int argc, char * argv[]) {
  if (argc < 3) {
    printf(1, "Need at least 2 arguments\n");
    printUsage();
    exit();
  } 

  struct intArrStruct *argStruct = malloc (sizeof(struct intArrStruct));
  argStruct->size = argc - 1;
  argStruct->nums = malloc(argStruct->size * sizeof(int));

  int i;
  for(i = 0; i < argStruct->size; i++){
    argStruct->nums[i] = atoi(argv[i+1]);
  }

  printf(1, "%d\n", add(argStruct));

  free(argStruct->nums);
  free(argStruct);
  exit();
}