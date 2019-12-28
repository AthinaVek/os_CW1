#ifndef ERGASIA1_H
#define ERGASIA1_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/times.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <time.h>

struct sharedStruct{
    int part;
    int type;
    int count;
    clock_t time;
};

struct sharedSyn{
    int part[3];
    int type;
	int name;
    int count;
    clock_t time;
};

#endif /* ERGASIA1_H */

