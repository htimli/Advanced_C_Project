#ifndef SERVICE_ORCHESTRE_H
#define SERVICE_ORCHESTRE_H

#define PROJECT_ID_SERVICE 6

// Ici toutes les communications entre l'orchestre et les services :
// - le tube anonyme pour que l'orchestre envoie des données au service
// - le sémaphore pour que  le service indique �  l'orchestre la fin
//   d'un traitements

typedef struct {
	int fdRead;
	int fdWrite;
}tubeA;

typedef struct {
	char * namesc;
	char * namecs;
}tubeN;


char * prefixName(char * basename);
//gestion tube annonyme
void create_tubeA_orchestre_service(tubeA *tb);


//=============================================================================
void write_tubeA_orchestre_service(tubeA tb,void * buff, int size);
//=============================================================================
void read_tubeA_orchestre_service(int t,void * buff, int size);
//gestion tube nommé  // l'orch cree des tube nommée et les donne au service et client
//tubes nommés  - com entre client service

void create_tubeN_client_service(char * basename);
void destruct_tubeN(char * basename);
//a refaire create tubeN service -> client


//gestion semaphore 

int create_sem_orchestre_service();
//=============================================================================
void init_sem_orchestre_service(int semid, int val);
//=============================================================================
void destruct_sem_orchestre_service(int semid);
//=============================================================================
void vendre_sem_client_service(int semId);
//=============================================================================
void prendre_sem_client_service(int semId);



#endif