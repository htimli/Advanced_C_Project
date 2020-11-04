#define _XOPEN_SOURCE
#include "myassert.h"

#include "client_orchestre.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include "myassert.h"                         
#include <string.h>
#define PROJET_ID1 5

#include <stdio.h>


static int  ret_v ;
  
//============================================================
void create_tubeN_client_orchestre(){
  unlink("/tmp/client_orchestre");
  ret_v=mkfifo("/tmp/client_orchestre",00777); 
  myassert(ret_v!= -1,"creation echouee de client_orchestre");
}  


//============================================================
void create_tubeN_orchestre_client(){
  unlink("/tmp/orchestre_client");
  ret_v=mkfifo("/tmp/orchestre_client",00777); 
  myassert(ret_v!= -1,"creation echouee d orchestre_client");
}  


//============================================================
int create_sem_orchestre_client(){
    key_t maClee =ftok("/tmp",PROJET_ID1);
    myassert(maClee!=-1,"ftok echouee");
    int sem1Id = semget(maClee,1,0666|IPC_CREAT|IPC_EXCL);   
    myassert(sem1Id != -1,"creation de sem_client_orchestre echouee");  
    return sem1Id;
}
// a revoir apres : peut etre semid passe en parametres d'entrés
//============================================================
int recup_sem_orchestre_client(){  
	key_t maClee =ftok("/tmp",PROJET_ID1);
    myassert(maClee!=-1,"ftok echouee");
    int semId = semget(maClee,1,0);   
    myassert(semId != -1,"recuperation de semaphore un echouee");
	return semId;
}
//=============================================================
void prendre(int semId){
	struct sembuf op = {0,-1,0}; 
  	ret_v =semop(semId,&op,1);
    myassert(ret_v!=-1,"fonction prendre echouee");
}
//==============================================================
void vendre(int semId){
	struct sembuf op = {0,1,0}; 
  	ret_v =semop(semId,&op,1);
    myassert(ret_v!=-1,"fontion vendre echouee");
}

//============================================================
void init_sem_client_orchestre(int semId){
    ret_v=semctl(semId,0,SETVAL,1);
    myassert(ret_v != -1,"initialisation du sem_client_orchestre un echouee"); 
}    

//============================================================
void envoi_tubeN_orchestre_client(void *data , int size){
    int fd=open("/tmp/orchestre_client",O_WRONLY,0641);
    myassert(fd !=-1," ouverture fichier orchestre_client echouee");
    ret_v=write(fd,data,size);
    myassert(ret_v != -1,"ecriture tubeN_orchestre_client echouee");
    //close(fd);
}                 
//============================================================
void reception_tubeN_orchestre_client(void *data , int size){
    int fd=open("/tmp/orchestre_client",O_RDONLY,0641);
    myassert(fd !=-1," ouverture fichier orchestre_client echouee");
    ret_v=read(fd,data,size);
    myassert(ret_v != -1,"reception tubeN_orchestre_client echouee");

    //close(fd);
}    
//============================================================
void reception_tubeN_client_orchestre(void *data ,int size){
    int fd2=open("/tmp/client_orchestre",O_RDONLY,0641);
    myassert(fd2 !=-1," ouverture fichier client_orchestre echouee");
    ret_v=read(fd2,data,size);
    myassert(ret_v != -1,"lecture tubeN_client_orchestre  echouee");
    //close(fd2);
}    
//===========================================================
void envoi_tubeN_client_orchestre(void *data ,int size){
	printf("avant : \n");
    int fd2=open("/tmp/client_orchestre",O_WRONLY,0641);
    myassert(fd2 !=-1," ouverture fichier client_orchestre echouee");
    printf("avant :2 \n");
	ret_v=write(fd2,data,size);
    myassert(ret_v != -1,"ecriture tubeN_client_orchestre  echouee");
    printf("avant : 3 \n");
	//close(fd2);
}    
//============================================================
void prendre_sem_client_orchestre(int semId){
  struct sembuf opsm ={0,-1,0};
  ret_v=semop(semId,&opsm,0);
  myassert(ret_v != -1,"operation prendre de semaphore echouee\n");
}
//============================================================
void vendre_sem_client_orchestre(int semId){
  struct sembuf opsm ={0,+1,0};
  ret_v=semop(semId,&opsm,0);
  myassert(ret_v != -1,"operation prendre de semaphore echouee\n");
}
//============================================================
void destruction_tubeN_client_orchestre(){
  unlink("/tmp/client_orchestre");
}
//============================================================
void destruction_tubeN_orchestre_client(){
  unlink("/tmp/orchestre_client");
}
//============================================================
void destruction_sem_client_orchestre(int semId){
  semctl(semId,0,IPC_RMID);;
}
