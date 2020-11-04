
#define _XOPEN_SOURCE
#include "client_service.h"



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



char * prefixtubeN(char * basename){
	char *mot = malloc(sizeof(char)*50);
	strcpy(mot,"/tmp/");
    strcat(mot,basename);
	return mot;
}

//=============================================================================
int open_tubeNRead(char *nom_tubeN){

	int fd = open( prefixtubeN(nom_tubeN) , O_RDONLY,777);
	myassert(fd!=-1," open_tube N Read failed");	
	return fd;
}


//=============================================================================
int open_tubeNwrite(char *nom_tubeN){	
	int fd = open( prefixtubeN(nom_tubeN) , O_WRONLY,777);
	myassert(fd!=-1," open_tube N Write failed");
	return fd;
}


//=============================================================================
void recieve_client_service(char *nom_tubeN, void * buff, int size){

	int fd = open_tubeNRead(nom_tubeN);

	int ret_val = read(fd,buff,size);
   	myassert(ret_val!=-1,"Read failed - recieve data from client to service ");
}

//=============================================================================
void send_client_service(char *nom_tubeN, void * buff, int size){

	int fd = open_tubeNwrite(nom_tubeN);

	int ret_val = write(fd,buff,size);
   	myassert(ret_val!=-1,"Write failed - send data from client to service ");
}


//=============================================================================
void recieve_service_client(char *nom_tubeN, void * buff, int size){
	int fd = open_tubeNRead(nom_tubeN);

	int ret_val= read(fd,buff,size);
   	myassert(ret_val!=-1,"Read failed - receive data from Service to client ");	

}


//=============================================================================
void send_service_client(char *nom_tubeN, void * buff, int size){
	int fd = open_tubeNwrite(nom_tubeN);

	int ret_val= write(fd,buff,size);
   	myassert(ret_val!=-1,"Write failed - Send data from Service to client ");	
}



//=============================================================================
void destruct_tubeN_client_service(char * nom_tubeN){
	unlink( prefixtubeN(nom_tubeN) );
}


//=============================================================================
void destruct_tubeN_service_client(char * nom_tubeN){
	unlink( prefixtubeN(nom_tubeN) );
}