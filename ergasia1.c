
#include "Ergasia1.h"


union semun {
   int val;                  //value for SETVAL
   struct semid_ds *buf;     // buffer for IPC_STAT, IPC_SET
   unsigned short *array;    // array for GETALL, SETALL
};

int sem_down(int sem_id) {
   struct sembuf sem_d;
   sem_d.sem_num = 0;
   sem_d.sem_op = -1;
   sem_d.sem_flg = 0;
   if (semop(sem_id,&sem_d,1) == -1) {
       return -1;
   }
   return 0;
}

int sem_up(int sem_id) {
   struct sembuf sem_d;
   sem_d.sem_num = 0;
   sem_d.sem_op = 1;
   sem_d.sem_flg = 0;
   if (semop(sem_id,&sem_d,1) == -1) {
       return -1;
   }
   return 0;
}

int sem_Init(int sem_id, int val) {
   union semun arg;
   arg.val = val;
   if (semctl(sem_id,0,SETVAL,arg) == -1) {
       perror("Semaphore Setting Value ");
       return -1;
   }
   return 0;
}


void constructor(int i, int num_parts, int k_mutex, int v_mutex, struct sharedStruct *shrd){
    int *parts;
    int rnd;
    struct sharedStruct tempShrd;
    
    parts = malloc((num_parts)*(sizeof(int)));      //ftiaxnw ta onomata twn eksarthmatwn
    for (int j=0; j<num_parts; j++){
        rnd = (rand() % 8999) + 1000;
        parts[j] = rnd;
       // printf("KATASKEUH %d --- %d !@#$ %d Fixed at time %Lf\n", i, parts[j],getpid(),(long double)tempShrd.time);
        sem_down(k_mutex);
        tempShrd.part = parts[j];
		tempShrd.time = clock();				// Begin time
        tempShrd.type = i;
        memcpy(shrd, &tempShrd, sizeof(struct sharedStruct));
        sem_up(v_mutex);
    }
    printf(".....................Construction Ended\n");
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


int main(int argc, char** argv){
    int num_parts;
    int status=0;
    pid_t pid, *pids=NULL;
    int v_mutex, ve_mutex, k_mutex, e_mutex, s_mutex, es_mutex;
    int shm_v, shm_e, shm_s;
    struct sharedStruct *shrd_v, *shrd_e;
	struct sharedSyn *shrd_s;
    double running_average;
    
    if (argc == 2){
        num_parts = atoi(argv[1]);
        
	    k_mutex = semget(IPC_PRIVATE,1,IPC_CREAT | 0666);
		if (k_mutex == -1) {
			perror("Semaphore Creation\n");
			semctl(k_mutex,0,IPC_RMID);
			exit(0);
		}
		if (sem_Init(k_mutex,1) == -1) {
			perror("Semaphore Initialize\n");
			semctl(k_mutex,0,IPC_RMID);
			exit(0);
		}
	        
	    v_mutex = semget(IPC_PRIVATE,1,IPC_CREAT | 0666); 
		if (v_mutex == -1) {   
			perror("Semaphore Creation\n");
			semctl(v_mutex,0,IPC_RMID);
			exit(0);
		}
		if (sem_Init(v_mutex,0) == -1) {
			perror("Semaphore Initialize\n");
			semctl(v_mutex,0,IPC_RMID);
			exit(0);
		}
	        
	    e_mutex = semget(IPC_PRIVATE,1,IPC_CREAT | 0666);
		if (e_mutex == -1) {   
			perror("Semaphore Creation\n");
			semctl(e_mutex,0,IPC_RMID);
			exit(0);
		}
		if (sem_Init(e_mutex,0) == -1) {
			perror("Semaphore Initialize\n");
			semctl(e_mutex,0,IPC_RMID);
			exit(0);
		}
	        
	    ve_mutex = semget(IPC_PRIVATE,1,IPC_CREAT | 0666);
		if (ve_mutex == -1) {   
			perror("Semaphore Creation\n");
			semctl(ve_mutex,0,IPC_RMID);
			exit(0);
		}
		if (sem_Init(ve_mutex,1) == -1) {
			perror("Semaphore Initialize\n");
			semctl(ve_mutex,0,IPC_RMID);
			exit(0);
		}
	        
	    es_mutex = semget(IPC_PRIVATE,1,IPC_CREAT | 0666);
		if (es_mutex == -1) {   
			perror("Semaphore Creation\n");
			semctl(es_mutex,0,IPC_RMID);
			exit(0);
		}
		if (sem_Init(es_mutex,1) == -1) {
			perror("Semaphore Initialize\n");
			semctl(es_mutex,0,IPC_RMID);
			exit(0);
		}
	        
	    s_mutex = semget(IPC_PRIVATE,1,IPC_CREAT | 0666);
		if (s_mutex == -1) {   
			perror("Semaphore Creation\n");
			semctl(s_mutex,0,IPC_RMID);
			exit(0);
		}
		if (sem_Init(s_mutex,0) == -1) {
			perror("Semaphore Initialize\n");
			semctl(s_mutex,0,IPC_RMID);
			exit(0);
		}

	    /////// SHARED MEMORIES ////////
	    shm_v = shmget(IPC_PRIVATE,sizeof(struct sharedStruct),0666|IPC_CREAT);
		if (shm_v == -1) {
		    	perror("Error in input shared memory creation(PAINTER)\n");
			shmctl(shm_v,IPC_RMID,0);
			exit(0);
		}
		shrd_v = (struct sharedStruct*)shmat(shm_v,(struct sharedStruct*)0,0);
		if (shrd_v == (struct sharedStruct*)-1 ) {
		    	perror("Error in shared memory attach(PAINTER)\n");
		    	shmctl(shm_v,IPC_RMID,0);
		    	exit(0);
		}
	        
	    shm_e = shmget(IPC_PRIVATE,sizeof(struct sharedStruct),0666|IPC_CREAT);
		if (shm_e == -1) {
		    	perror("Error in input shared memory creation(PAINTER)\n");
			shmctl(shm_e,IPC_RMID,0);
			exit(0);
		}
		shrd_e = (struct sharedStruct*)shmat(shm_e,(struct sharedStruct*)0,0);
		if (shrd_e == (struct sharedStruct*)-1 ) {
		    	perror("Error in shared memory attach(PAINTER)\n");
		    	shmctl(shm_e,IPC_RMID,0);
		    	exit(0);
		}
	        
	    shm_s = shmget(IPC_PRIVATE,sizeof(struct sharedSyn),0666|IPC_CREAT);
		if (shm_s == -1) {
		    	perror("Error in input shared memory creation(PAINTER)\n");
			shmctl(shm_s,IPC_RMID,0);
			exit(0);
		}
		shrd_s = (struct sharedSyn*)shmat(shm_s,(struct sharedSyn*)0,0);
		if (shrd_s == (struct sharedSyn*)-1 ) {
		    	perror("Error in shared memory attach(PAINTER)\n");
		    	shmctl(shm_s,IPC_RMID,0);
		    	exit(0);
		}
	    shrd_s->count = 0;
		shrd_s->name = 1;
		for(int i = 0 ; i<3 ; i++){
			shrd_s->part[i] = -1;
		}

        ////// DIERGASIES //////
        pids = (pid_t *)malloc(8*sizeof(pid_t));
        for (int i=0; i<8; i++){  // diergasies 3+1+3+1
            pid = fork();
            if (pid < 0){
                printf("ERROR fork\n");
                return -1;
            }
            ///// PARENT /////
            else if (pid > 0){
            }
            ///// CHILD /////
            else{
                if ((i >= 0) && (i <= 2)){    // 3 diergasies kataskeuhs
                    constructor(i+1, num_parts, k_mutex, v_mutex, shrd_v);
                }
                else if (i == 3){   // diergasia vafeiou
					clock_t arrived;
					double painter_average=0,time_spent=0;
                    for (int m=0; m<3*num_parts; m++){
                        sem_down(v_mutex);
                        sem_down(ve_mutex);
                        memcpy(shrd_e, shrd_v, sizeof(struct sharedStruct));
						arrived = clock();
						time_spent = (double) arrived - shrd_v->time ;
						painter_average = painter_average + time_spent;
						// usleep(shrd_v->type*500);
                       // printf("VAFEIO ========== %d  TYPE %d number %d\n", shrd_v->part, shrd_v->type, m+1);
                        sem_up(e_mutex);
						sem_up(k_mutex);
                    }
					running_average = painter_average/(CLOCKS_PER_SEC * 3*num_parts);
					printf("Painter Ended with average waiting time %f\n",running_average);
                    printf(".....................Painter Ended\n");
                }
                else if ((i >= 4) && (i <= 6)){    // 3 diergasies elegxou
                    struct sharedStruct tempShrd;
                    int *e_parts,*time_parts;
                    int m=0,pos=0;
                    e_parts = malloc(num_parts*sizeof(int));
					time_parts = malloc(num_parts*sizeof(int));
        			struct sharedSyn temp;
					while(1){
						sem_down(e_mutex);
                        memcpy(&tempShrd, shrd_e, sizeof(struct sharedStruct));
                        // usleep(tempShrd.type*300);
                        if (tempShrd.type == i-3){
	                        e_parts[m] = tempShrd.part;
							time_parts[m] = tempShrd.time;
	                       // printf("Putting in check table %d ========== %d  number %d i=%d\n",  tempShrd.type, tempShrd.part, m, i-3);
							m++;
							sem_up(ve_mutex);
						}
						else
							sem_up(e_mutex);
						if(m == num_parts) break;
					}
                    while(1) {
						sem_down(es_mutex);
						memcpy(&temp, shrd_s, sizeof(struct sharedSyn));
						//printf("-----------_>%s %d %d\n",temp.name,i-4, temp.part[i-4]);
						if( temp.part[i-4] == -1){
							//printf("is  -1  send\n");
							temp.part[i-4] = 1;
							temp.name = e_parts[pos];
							temp.time = time_parts[pos];
							pos++;
							//printf("%d ========================= %d !!!!!!!!!!!!!!!!!!!!!!!!! %d \n",temp.name,temp.part[i-4],pos);
							memcpy(shrd_s, &temp, sizeof(struct sharedSyn));
							sem_up(s_mutex);
						}
						else
							sem_up(es_mutex);
                        if (pos == num_parts){	
                            break;
                        }
                    }
                    printf(".....................Check ended\n");
                }
                else if (i == 7){    // diergasia sunarmologishs
                    struct sharedSyn tempShrd;
					tempShrd.count = 0;
                    int l=0;
					int *avaliable_table = malloc(3*sizeof(int));
					char **name = malloc(3*sizeof(char*));
					for(int i=0; i<3 ; i++){
						name[i] = malloc(5*sizeof(char));
						avaliable_table[i] = -1;
					}
						int t=0;
					FILE *file = fopen("results.txt","w");
					if ( file == NULL){
						printf("Error in reading file\n");
						exit(0);
					}
					double assemble_time = 0, total=0, time;
					clock_t arrived;
					int count ;
                    while(1) {
						sem_down(s_mutex);
						l++;
						count = 0;
                      	memcpy(&tempShrd, shrd_s, sizeof(struct sharedSyn));		// pairnei to part
						for( int i = 0; i < 3 ; i++){
							//printf("Searching.........................%d\n",tempShrd.part[i]);
							if(tempShrd.part[i] != -1 ){
								arrived = clock();
								time = (double)arrived - tempShrd.time;
								// printf("\n%7f %7f %7f\n",time,(double) arrived, (double) tempShrd.time);
								count++;
								assemble_time += time ;
							}
						//	printf("Count is  %d \n",count);
						}
						if( count == 3){
							//printf("Found 3 parts\n");
							t++;
							total += assemble_time;
							assemble_time = assemble_time / (CLOCKS_PER_SEC * 3);
							//printf("Average time is %f %d %d\n",assemble_time,t,tempShrd.name);
							for( int i = 0; i < 3 ; i++)
							tempShrd.part[i] = -1;
							tempShrd.name = 1;
							count = 0;
							memcpy(shrd_s, &tempShrd, sizeof(struct sharedSyn));
						}
						sem_up(es_mutex);
                        if(t == num_parts) break;
                    }
					printf("Total average time of creating a product is %f\n",total/(CLOCKS_PER_SEC * num_parts));
                    printf(".....................assembler ended with %d \n",t);
                }
                exit(0);
            }
        }
        for (int i=0; i<8; i++) {
            int status;
            pid_t p = wait(&status); // perimenei ta paidia
        }
    }
    else
        printf("ERROR! Wrong number of arguments.\n");
    
    return 0;
}