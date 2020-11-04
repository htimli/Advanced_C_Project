
#define _XOPEN_SOURCE

#include "myassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../CONFIG/config.h"
#include "client_orchestre.h"
#include "service_orchestre.h"


//pipe
#include <sys/types.h>
#include <sys/stat.h>
//sem
#include <sys/ipc.h>
#include <sys/sem.h>

//anonyme pipe
#include <unistd.h>

#include <fcntl.h>  


#include <string.h>

#include <sys/wait.h>


int ret_v;
char * prefixName(char * basename){
	char * nametube = malloc(sizeof(char)*100);
	strcpy(nametube,"/tmp/");
	strcat(nametube,basename);
	return nametube;
}

//gestion tube annonyme
void create_tubeA_orchestre_service(tubeA *tb){
	int t[2];
	
	int ret_val= pipe(t);

	tb->fdRead= t[0];
	tb->fdWrite= t[1];

	myassert(ret_val!=-1,"Echec de creation de tube annonyme");	
}


//=============================================================================
void write_tubeA_orchestre_service(tubeA tb,void * buff, int size){
	close(tb.fdRead);
	write(tb.fdWrite,buff,size);
}

//=============================================================================
void read_tubeA_orchestre_service(int t,void * buff, int size){
	read(t,buff,size);
}


//gestion tube nommé  // l'orch cree des tube nommée et les donne au service et client
//tubes nommés  - com entre client service

void create_tubeN_client_service(char * basename){
	
	unlink(prefixName(basename));
	int ret_val= mkfifo(prefixName(basename),00777) ;
	myassert(ret_val!=-1,"Erreur de creation tube nommé Client_service");	
}
void destruct_tubeN(char * basename){

	unlink(prefixName(basename));
}
//a refaire create tubeN service -> client


//gestion semaphore 

int create_sem_orchestre_service(){
	key_t semkey=-1;
	semkey = ftok("service_orchestre.h", PROJECT_ID_SERVICE);
	myassert(semkey !=- 1,"IPC error: ftok");
	int semid = semget(semkey,1,00666 | IPC_CREAT | IPC_EXCL);
	myassert(semid!=-1,"semaphore not created ");

	return semid;
}


//=============================================================================
void init_sem_orchestre_service(int semid, int val){
	int ret_val = semctl(semid,0, SETVAL, val);
   	myassert(ret_val!=-1,"semaphore not Init ");
}
//============================================================
void prendre_sem_client_service(int semId){
  struct sembuf opsm ={0,-1,0};
  ret_v=semop(semId,&opsm,0);
  myassert(ret_v != -1,"operation prendre de semaphore echouee\n");
}
//============================================================
void vendre_sem_client_service(int semId){  
  struct sembuf opsm ={0,+1,0};
  ret_v=semop(semId,&opsm,0);
  myassert(ret_v != -1,"operation Vendre de semaphore echouee\n");
}

//=============================================================================
void destruct_sem_orchestre_service(int semid){
	semctl(semid , 0,IPC_RMID);
}

