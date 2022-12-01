#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif
#include<random>
#include<queue>
#include "channel.cpp"
using namespace std;

#define PASSENGERS 5
#define MEAN 5

int M, N, P;
int w, x, y, z;

sem_t boarding;
sem_t kiosks;
sem_t specialKiosk;
pthread_mutex_t kioskMtx;
sem_t *belts;
FILE *inpf;
time_t globalTime = time(NULL);
queue<int> emptyKiosks;

int elapsedTime() {
  return time(NULL) - globalTime;
}

void * passenger(void * arg) {
  int isVip = rand() % 3 == 0;
  int *pid = (int*) arg;
  const char* vipStatus = (isVip) ? " (VIP)": "";
  int index;
  fprintf(stdout, "Passenger %2d%6s has arrived at the airport at time %d\n", pid, vipStatus, elapsedTime());

  // Self check-in at kiosk
  sem_wait(&kiosks);
  pthread_mutex_lock(&kioskMtx);
  index = emptyKiosks.front();
  emptyKiosks.pop();
  pthread_mutex_unlock(&kioskMtx);
  fprintf(stdout, "Passenger %2d%6s has started self-check in at kiosk %d at time %d\n", pid, vipStatus, index + 1, elapsedTime());
  sleep(w);
  fprintf(stdout, "Passenger %2d%6s has received boarding pass at %d\n", pid, vipStatus, elapsedTime());
  pthread_mutex_lock(&kioskMtx);
  emptyKiosks.push(index);
  pthread_mutex_unlock(&kioskMtx);
  sem_post(&kiosks);

  if (!isVip) {
    // Security check
    index = rand() % N;
    fprintf(stdout, "Passenger %2d%6s has started waiting for security check in belt %d at %d\n", pid, vipStatus, index + 1, elapsedTime());
    sem_wait(&belts[index]);
    fprintf(stdout, "Passenger %2d%6s has started the security check at %d\n", pid, vipStatus, elapsedTime());
    sleep(x);
    fprintf(stdout, "Passenger %2d%6s has crossed the security check at %d\n", pid, vipStatus, elapsedTime());
    sem_post(&belts[index]);
  } else {
    // VIP Channel
    fprintf(stdout, "Passenger %2d%6s has started waiting for the VIP Channel at %d\n", pid, vipStatus, elapsedTime());
    enterLtoR();
    fprintf(stdout, "Passenger %2d%6s has hopped on the VIP channel (L to R) at %d\n", pid, vipStatus, elapsedTime());
    sleep(z);
    fprintf(stdout, "Passenger %2d%6s has crossed the VIP channel at %d\n", pid, vipStatus, elapsedTime());
    exitLtoR();
  }

  // Losing boarding pass
  while (rand() % 4 == 0) {
    fprintf(stdout, "Passenger %2d%6s has lost their boarding pass, waiting for VIP Channel at %d\n", pid, vipStatus, elapsedTime());
    enterRtoL();
    fprintf(stdout, "Passenger %2d%6s has arrived at VIP channel (R to L) at %d\n", pid, vipStatus, elapsedTime());
    sleep(z);
    fprintf(stdout, "Passenger %2d%6s has crossed the VIP channel, waiting at special kiosk at %d\n", pid, vipStatus, elapsedTime());
    exitRtoL();
    
    // Regain pass at kiosk
    sem_wait(&specialKiosk);
    sleep(w);
    fprintf(stdout, "Passenger %2d%6s has regained boarding pass, waiting for VIP Channel at %d\n", pid, vipStatus, elapsedTime());
    sem_post(&specialKiosk);

    enterLtoR();
    fprintf(stdout, "Passenger %2d%6s has hopped on the VIP channel (L to R) at %d\n", pid, vipStatus, elapsedTime());
    sleep(z);
    fprintf(stdout, "Passenger %2d%6s has crossed the VIP channel at %d\n", pid, vipStatus, elapsedTime());
    exitLtoR();
  }

  // Boarding gate
  sem_wait(&boarding);
  fprintf(stdout, "Passenger %2d%6s has started boarding the plane at %d\n", pid, vipStatus, elapsedTime());
  sleep(y);
  fprintf(stdout, "Passenger %2d%6s has boarded the plane at %d\n", pid, vipStatus, elapsedTime());
  sem_post(&boarding);

	pthread_exit(NULL);
}

int main () {
  // Reading inputs
  inpf = fopen("input.txt","r");
  // freopen("output.txt", "w", stdout);
  fscanf(inpf, "%d %d %d\n", &M, &N, &P);
  fprintf(stdout, "%d %d %d\n", M, N, P);
  fscanf(inpf, "%d %d %d %d\n", &w, &x, &y, &z);
  fprintf(stdout, "%d %d %d %d\n", w, x, y, z);

  // Initialization
  srand(time(0));
  sem_init(&boarding, 0, 1);
	pthread_mutex_init(&kioskMtx, NULL);
  sem_init(&kiosks, 0, M);  
  sem_init(&specialKiosk, 0, 1);  
  for (int i=0; i < M; i++)
    emptyKiosks.push(i);
  belts = (sem_t *) malloc(sizeof(sem_t) * N);
  for (int i=0; i < N; i++)
    sem_init(&belts[i], 0, P);
  vipChannelInit();

  int pid = 1;
  default_random_engine generator;
  poisson_distribution<int> distribution(MEAN);
  while (pid <= PASSENGERS) {
    pthread_t p1;
    pthread_create(&p1, NULL, passenger, (void *) pid);
    pid += 1;
    sleep(distribution(generator));
  }

  sem_destroy(&kiosks);
  sem_destroy(&specialKiosk);
  pthread_mutex_destroy(&kioskMtx);
  for (int i=0; i < N; i++)
    sem_destroy(&belts[i]);
  sem_destroy(&boarding);
  vipChannelClose();
  fclose(inpf);
  pthread_exit(NULL);

  return 0;
}