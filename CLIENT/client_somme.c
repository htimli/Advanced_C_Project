#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "client_service.h"
#include "client_somme.h"


/*----------------------------------------------*
 * fonctions appelables par le client
 *----------------------------------------------*/

// fonction d'envoi des données du client au service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv et en déduit les données à envoyer
void client_somme_sendData(char * client_service,int argc, char * argv[])
{

    // par exemple argv[2] et argv[3] contiennent les deux nombres
    // à envoyer (pour les sommer)
    if(argc>3){
        float t[2];
        t[0]=atof(argv[2]);
        t[1]=atof(argv[3]);

        send_client_service(client_service,t,2*sizeof(float));
    }
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_somme_receiveResult(char*  service_client, int argc, char * argv[])
{
    // par exemple on décide d'afficher le résultat et argv[4] contient
    // une chaine à afficher avant le résultat
    if(argc>=4){
        float res;

        recieve_service_client(service_client,&res,sizeof(float));
        printf("[user] %s \n",argv[4]);
        printf("[client] resultat obtenu est : %.2f \n",res);
    }

}
