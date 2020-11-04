#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "service_orchestre.h"
#include "client_service.h"
#include <unistd.h>

typedef struct
{
    float n1;
    float n2;
} Data;

typedef struct
{
    float n;
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
void somme_service_receiveDataData(char *name,Data *data)/* tubes,*/ /* autre chose ? */
{
    recieve_client_service(name,&(data->n1),sizeof(float));
    recieve_client_service(name,&(data->n2),sizeof(float));

    printf("%f %f \n ",data->n1,data->n2 );
}

// fonction de traitement des données
void somme_service_computeResult(Data *data,Results *res)/* autre chose ? */
{
    res->n= data->n1 + data ->n2 ;
}

// fonction d'envoi du résultat
void somme_service_sendResult(char *name,Results *res)/* tubes,*/ /* autre chose ? */
{
    send_service_client(name,&(res->n),sizeof(float));
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
int main(int argc, char * argv[])
{
    if (argc != 5)
        usage(argv[0], "nombre paramètres incorrect");

    // initialisations diverses
    //<clé_sémaphore> <fd_tube_anonyme> ""<tube_service_vers_client> <tube_client_vers_ service>\n",
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
//==================================================================
        char s[100];  
        printf("[service_somme]: lancé \n");
        read_tubeA_orchestre_service(fd_tube,s,sizeof(char)*(strlen("code1")+1));
        printf("[service_somme] code : %s \n",s);
        read_tubeA_orchestre_service(fd_tube,s,sizeof(char)*(strlen("mdpI")+1));
        printf("[service_somme] MDP : %s \n",s);
        
        printf("[service_somme] j'envoi --> au client un code acceptation \n");
        
        
        send_service_client(service_client,"code_acceptation",sizeof(char)* ( strlen("code_acceptation")+1 ));
        

        somme_service_receiveDataData(client_service,&data);

        somme_service_computeResult(&data,&res);
        printf("[Service] resultat obtenu est : %.2f \n",res.n);
        somme_service_sendResult(service_client,&res);

        vendre_sem_client_service(sem2Id);
    }

    // libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}
