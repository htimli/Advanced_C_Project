#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "service_orchestre.h"
#include "client_service.h"
#include <unistd.h>

typedef struct
{
    char *s;
    int size;
} Data;

typedef struct
{
    char *r; 
    int size;
} Results;

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
void compression_service_receiveDataData(char *name,Data *data)/* tubes,*/ /* autre chose ? */
{
    recieve_client_service(name,&(data->size),sizeof(int));
    data->s=malloc((data->size+1)*sizeof(char));
    
    recieve_client_service(name,data->s,((data->size)+1)*sizeof(char));
}

// fonction de traitement des données
void compression_service_computeResult(Data *data,Results *res )/* autre chose ? */
{
    char c = data->s[0];
	int cpt=1;
	res->r =malloc(100*sizeof(char));
	char tmp[2]; 
    for (int i=1;i<(data->size+1);i++){
        if(c==data->s[i])	
            cpt++;
        else
        {	
        	sprintf(tmp,"%d",cpt);
            res->r=strcat(res->r,tmp);
            res->r=strncat(res->r,&c,1);
            c=data->s[i];
            cpt=1;
        }
    }
    res->size=strlen(res->r);
}

// fonction d'envoi du résultat
void compression_service_sendResult(char *name,Results *res/* tubes,*/ /* autre chose ? */)
{
    open_tubeNRead(name);
    send_client_service(name,&(res->size),sizeof(int));
    send_client_service(name,(res->r),(res->size+1)*sizeof(char));
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
    char *service_client=argv[3];
    char *client_service=argv[4];
    
    Data data;
    Results res;
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
    
//=========================================================================
        char s[100];  
        printf("[service_compression]: lancé \n");
        read_tubeA_orchestre_service(fd_tube,s,sizeof(char)*(strlen("code1")+1));
        printf("[service_compression] code : %s \n",s);
        read_tubeA_orchestre_service(fd_tube,s,sizeof(char)*(strlen("mdpI")+1));
        printf("[service_compression] MDP : %s \n",s);

        printf("[service_compression] j'envoi --> au client un code acceptation \n");

        send_service_client(service_client,"code_acceptation",sizeof(char)* ( strlen("code_acceptation")+1 ));

    
        compression_service_receiveDataData(client_service,&data);
        compression_service_computeResult(&data,&res);
        compression_service_sendResult(service_client,&res);

        vendre_sem_client_service(sem2Id);
    // libération éventuelle de ressources
    }
    return EXIT_SUCCESS;
}
