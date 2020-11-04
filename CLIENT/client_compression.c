#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
void client_compression_sendData(char * client_service, int argc, char * argv[])
{
    // par exemple argv[2] est le nom du fichier contenant le texte
    // à envoyer pour compression
    if(argc>1){
        char *data=argv[2];
        int sizedata=strlen(data);
        send_client_service(client_service,&sizedata,sizeof(int));
        send_client_service(client_service,data,(sizedata+1)*sizeof(char));
    }   
    
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_compression_receiveResult(char * service_client, int argc, char * argv[])
{
    // par exemple on décide de sauvegarder le résultat dans un fichier et
    // argv[3] est le nom du fichier où écrire le texte compressé
    if(argc==4)
        printf("i don't write on files yet %s",argv[3]);
        
    if(argc>2){
        int sizeRes;
        recieve_service_client(service_client,&sizeRes,sizeof(int));
        char *res = malloc((sizeRes+1)*sizeof(char));
        recieve_service_client(service_client,res,(sizeRes+1)*sizeof(char));
        printf("[client] resultat obtenu est ==> %s \n",res);
    }
}
