#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "stat.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint
sys_getsize(void)
{
  return myproc()->sz;
}

void 
sys_shutdown(void) 
{
  outw(0xB004, 0x0|0x2000);
  outw(0x604, 0x0|0x2000);
}

int 
sys_incr(void) {
  int arg1;
  argint(0, &arg1);
  return arg1 + 1;
}

int 
sys_add(void) {
  struct intArrStruct *inp;
  argptr(0 , (void*) &inp ,sizeof(*inp));

  int i, sum;
  for (i = 0, sum = 0; i < inp->size; i++) {
    sum += inp->nums[i];
  }

  return sum;
}

char *
sys_substr(void) {
  int start, len;
  char *str;
  argstr(0, &str);
  argint(1, &start);
  argint(2, &len);

  int i, j;
  for (i = start, j = 0; j < len;) {
    str[j++] = str[i++];
  }
  str[j] = '\0';

  return str;
}
