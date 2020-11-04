#ifndef CLIENT_SERVICE_H
#define CLIENT_SERVICE_H

// Ici toutes les communications entre les services et les clients :
// - les deux tubes nommés pour la communication bidirectionnelle
//gestion tube nommé
//tubes nommés  - com entre client service

//=============================================================================
int open_tubeNRead(char *nom_tubeN);

//=============================================================================
int open_tubeNwrite(char *nom_tubeN);


//=============================================================================
void recieve_client_service(char *nom_tubeN, void * buff, int size);
//=============================================================================
void send_client_service(char *nom_tubeN, void * buff, int size);

//=============================================================================
void send_service_client(char *nom_tubeN, void * buff, int size);
//=============================================================================
void recieve_service_client(char *nom_tubeN, void * buff, int size);

//=============================================================================
void destruct_tubeN_client_service();
//=============================================================================
void 	destruct_tubeN_service_client();
#endif
