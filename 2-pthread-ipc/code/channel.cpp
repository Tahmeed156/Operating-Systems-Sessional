#include<pthread.h>
#include<stdlib.h>

pthread_cond_t pasgExit;
pthread_mutex_t pasgExitMtx;
pthread_mutex_t pasgStateMtx;
int pasgNum[3] = { 0 };
extern int z;
enum Direction { LtoR=0, RtoL=1, inTransit=2 } dir;

void vipChannelInit() {
  dir = LtoR;
  pthread_cond_init (&pasgExit, NULL);
	pthread_mutex_init(&pasgExitMtx, NULL);
	pthread_mutex_init(&pasgStateMtx, NULL);
}

void enterLtoR() {
  pasgNum[LtoR] += 1;
  // Waiting for R-L passengers in transit
  while (true) {
    pthread_mutex_lock(&pasgStateMtx);
    if (!(dir == RtoL && pasgNum[inTransit] > 0)) {
      pasgNum[LtoR] -= 1;
      if (dir == RtoL) {
        dir = LtoR;
        printf("VIP channel changing direction to Left->Right (to boarding gate)\n");
      }
      pasgNum[inTransit] += 1;
      pthread_mutex_unlock(&pasgStateMtx);
      break;
    } else {
      pthread_mutex_unlock(&pasgStateMtx);
      pthread_cond_wait(&pasgExit, &pasgExitMtx);
    }
  }
}

void exitLtoR() {
  pasgNum[inTransit] -= 1;
  pthread_cond_broadcast(&pasgExit);
}

void enterRtoL() {
  // Waiting for L-R passengers to finish
  // Cases - passengers in transit in opposite dir, empty walkway
  while (true) {
    pthread_mutex_lock(&pasgStateMtx);
    if (pasgNum[LtoR] == 0 && !(pasgNum[inTransit] > 0 && dir == LtoR)) {
      if (dir == LtoR) {
        dir = RtoL;
        printf("VIP channel changing direction to Right->Left (to special kiosk)\n");
      }
      pasgNum[inTransit] += 1;
      pthread_mutex_unlock(&pasgStateMtx);
      break;
    } else { 
      // FIXME: Thread may switch right here
      pthread_mutex_unlock(&pasgStateMtx);
      pthread_cond_wait(&pasgExit, &pasgExitMtx);
    }
  }
  
}

void exitRtoL() {
  pasgNum[inTransit] -= 1;
  pthread_cond_broadcast(&pasgExit);
}

void vipChannelClose() {
  pthread_mutex_destroy(&pasgStateMtx);
  pthread_mutex_destroy(&pasgExitMtx);
  pthread_cond_destroy(&pasgExit);
}