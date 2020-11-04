#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "client_service.h"
#include "client_compression.h"


/*----------------------------------------------*
 * fonctions appelables par le client
 *----------------------------------------------*/

// fonction d'envoi des données du client au service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv et en déduit les données à envoyer
void client_max_sendData(char *client_service,int argc, char * argv[])
{
    // par exemple argv[2] est le nom du fichier contenant la suite
    // de nombres
    int size =  argc  - 2;
      send_client_service(client_service,&size,sizeof(int));
    float val;
    for(int i=2; i< argc ; i++){
        val=atof(argv[i]);
        send_client_service(client_service,&val,sizeof(float));
        printf("[client max] j'ai envoyé %f \n",val);
    }
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_max_receiveResult(char *service_client, int argc, char * argv[])
{
    // par exemple on décide d'afficher le résultat et argv[3] contient
    // une chaine à afficher avant le résultat

    float res;

    recieve_service_client(service_client,&res,sizeof(float));
    printf("[client_service_max] Valeur max obtenu est : %.2f \n",res);
    
}
