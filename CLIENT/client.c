#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "client_orchestre.h"
#include "client_service.h"

#include "client_somme.h"
#include "client_compression.h"
#include "client_max.h"

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


static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <num_service> ...\n", exeName);
    fprintf(stderr, "        ... : les paramètres propres au service\n");
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    if (argc < 2)
        usage(argv[0], "nombre paramètres incorrect");

    int numService = strtol(argv[1], NULL, 10);

    // initialisations diverses

    // entrée en section critique pour communiquer avec l'orchestre
    int semId=recup_sem_orchestre_client();
    prendre(semId);
    // envoi à l'orchestre du numéro du service

    // attente code de retour
    // si code d'erreur
    //     afficher l'erreur
    //     sortie de la section critique
    // sinon
    //     récupération du mot de passe et des noms des 2 tubes
    //     envoi d'une accusé de réception à l'orchestre
    //     sortie de la section critique
    //     envoi du mot de passe au service
    //     attente de l'accusé de réception du service
    //     appel de la fonction d'envoi des données (une fct par service)
    //     appel de la fonction de réception du résultat (une fct par service)
    //     envoi d'un accusé de réception au service

    // libération éventuelle de ressources
    
		//==========================================================================
        
		envoi_tubeN_client_orchestre(&numService,sizeof(int));
			
        char s[100]; 	
        reception_tubeN_orchestre_client(s,sizeof(char)*(strlen("code_acceptation")+1));
        printf("[client] code A : %s \n",s);
    
        reception_tubeN_orchestre_client(s,(strlen("1234")+1)*sizeof(char));
        printf("[client] my mdp : %s \n",s);

        char client_service[100],service_client[100];
        
        reception_tubeN_orchestre_client(client_service ,sizeof(char)*(strlen("client_service0")+1));
        printf("[client] client_service : (%s)\n",client_service);

        reception_tubeN_orchestre_client(service_client ,sizeof(char)*(strlen("client_service0")+1));
        printf("[client] service_client : (%s)\n",service_client);
        
        printf("[client]j'envoi un accusÃ© de reception --> a l'orchestre\n");
        envoi_tubeN_client_orchestre("RECU",5*sizeof(char));
        
        vendre(semId);

        recieve_service_client(service_client,s,sizeof(char)* (strlen("code_acceptation")+1  )   );
        printf("-++-+-+-+ 0%s \n",s  );
        //converse client service
        printf("[client] j ai recu de la part de service: %s \n",s);
        if(numService==1){
            client_somme_sendData(client_service,argc,argv);
            client_somme_receiveResult(service_client,argc,argv);
        }
        else if (numService ==2) {
            client_compression_sendData(client_service,argc,argv);
            client_compression_receiveResult(service_client,argc,argv);
        } else if(numService ==3)
        {
            client_max_sendData(client_service,argc,argv);
            client_max_receiveResult(service_client,argc,argv);
        }
            

        // destruction_tubeN_client_orchestre();
        // destruction_tubeN_orchestre_client();

            	


    return EXIT_SUCCESS;
}
