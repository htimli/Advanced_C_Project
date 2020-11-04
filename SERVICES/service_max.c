#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "service_orchestre.h"
#include "client_service.h"

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string.h>

#include <pthread.h>
#include "myassert.h"

#define NB_THREAD 3 

typedef struct
{
    int a,b;
    float *tab;
    int size;
} Data;

int semid;
struct sembuf ops ={0,1,0};
struct sembuf opsm ={0,-1,0};

float res=12;

void * search_max(void * arg){
    
    semop(semid,&opsm,0);

    Data *pt=(Data *)arg;

    for(int i=pt->a ; i<=pt->b ; i++){
        if(pt->tab[i] > res)
            res=pt->tab[i];
    }
    printf("val de res : %f \n",res);
    semop(semid,&ops,0);
    
    
    return NULL;
}

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <clé_sémaphore> <fd_tube_anonyme> "
            "<tube_service_vers_client> <tube_client_vers_ service>\n",
            exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


/*----------------------------------------------*
 * fonctions appelables par le service
 *----------------------------------------------*/

// fonction de réception des données
void max_service_receiveDataData(char *name,float *t,int *size)/* tubes,*/ /* autre chose ? */
{  
    recieve_client_service(name,size,sizeof(int));
    printf("[Service max] j'ai recu size  %d \n",*size);
    t=malloc((*size)*sizeof(float));
    for(int i =0 ;i<(*size);i++){
        recieve_client_service(name,&(t[i]),sizeof(float));
        printf("[Service max] j'ai recu %f \n",t[i]);
    }

    
    send_service_client("service_client2",&res,sizeof(float));
    for(int i=0; i<*size; i++)
        printf("%.2f \n",t[i]);
}

// fonction de traitement des données
void max_service_computeResult(float tab[],int *size/* autre chose ? */)
{
    semid = semget(IPC_PRIVATE,1,IPC_CREAT | IPC_EXCL | 00777 );
    semctl(semid, 0, SETVAL,1);
    
      for(int i=0; i<*size; i++)
        printf("==  %.2f \n",tab[i]);

    pthread_t id_thread[NB_THREAD];

    Data data[NB_THREAD];
    //initialisation 

 
    int taille= *size;
    for(int i=0;i<NB_THREAD;i++){
        data[i].size=taille;
        data[i].tab=malloc((taille)*sizeof(float));
        for(int j=0 ;j<taille ;j++){
            
            printf(" j=%d ,  size  %f   \n ",j,tab[j]);
            data[i].tab[j]=tab[j];            
        }
    }

    data[0].a=0;
    data[0].b=data[0].size/3;
    
    data[1].a=(data[0].size)/3;
    data[1].b=(data[0].size)*2/3;

    data[2].a=(data[0].size)*2/3;
    data[2].b=data[0].size;
    
    
    for(int i=0;i<NB_THREAD;i++){
	    int ret_v = pthread_create(&id_thread[i],NULL,search_max,(int *)&data[i]); 
        myassert(ret_v == 0,"erreur cration thread !");
    }

    
    
	for(int i=0;i<NB_THREAD;i++){
        int ret_v= pthread_join(id_thread[i],NULL);
        myassert(ret_v == 0,"erreur lors de l attente du fils du thread !");
    }    
}

// fonction d'envoi du résultat
void max_service_sendResult(char *name){
    printf("[service_max send result] Valeur max obtenu est : %.2f \n",res);
    send_service_client(name,&res,sizeof(float));
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
int main(int argc, char * argv[])
{
    if (argc != 5)
        usage(argv[0], "nombre paramètres incorrect");

    // initialisations diverses
    int sem2Id = atoi(argv[1]);
    int fd_tube =atoi(argv[2]);
    char * service_client = argv[3];
    char * client_service = argv[4];
    //char *service_client=argv[3];
    //char *client_service=argv[4];
  //  Data data;
    while (true)
    {
        // attente d'un code de l'orchestre (via tube anonyme)
        // si code de fin
        //    sortie de la boucle
        // sinon
        //    réception du mot de passe de l'orchestre
        //    attente du mot de passe du client
        //    si mot de passe incorrect
        //        envoi au client d'un code d'erreur
        //    sinon
        //        envoi au client d'un code d'acceptation
        //        réception des données du client (une fct par service)
        //        calcul du résultat (une fct par service)
        //        envoi du résultat au client (une fct par service)
        //        attente de l'accusé de réception du client
        //    modification du sémaphore pour prévenir l'orchestre de la fin
         char s[100];  
        printf("[service_max]: lancé \n");
        read_tubeA_orchestre_service(fd_tube,s,sizeof(char)*(strlen("code1")+1));
        printf("[service_max] code : %s \n",s);
        read_tubeA_orchestre_service(fd_tube,s,sizeof(char)*(strlen("mdpI")+1));
        printf("[service_max] MDP : %s \n",s);
        
        printf("[service_max] j'envoi --> au client un code acceptation \n");
        send_service_client(service_client,"code_acceptation",sizeof(char)* ( strlen("code_acceptation")+1 ));
        
        float *tab=NULL;
        int sizeTab;
        max_service_receiveDataData(client_service,tab,&sizeTab);
        max_service_computeResult(tab,&sizeTab);
        printf("\n   valeur de t %f \n" ,tab[0]); 
        max_service_sendResult(service_client);

        vendre_sem_client_service(sem2Id);
    }

    // libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}
