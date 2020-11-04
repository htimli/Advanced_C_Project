#ifndef CLIENT_ORCHESTRE_H
#define CLIENT_ORCHESTRE_H

// Ici toutes les communications entre l'orchestre et les clients :
// - le sémaphore pour que 2 clients ne conversent pas en même
//   temps avec l'orchestre
// - les deux tubes nommés pour la communication bidirectionnelle

void create_tubeN_client_orchestre();
//============================================================
void create_tubeN_orchestre_client();
//============================================================
int create_sem_orchestre_client();
//============================================================
int recup_sem_orchestre_client();
//=============================================================
void prendre(int semId);
//=============================================================
void vendre(int semId);
//============================================================
void init_sem_client_orchestre(int semId);
//============================================================
void envoi_tubeN_orchestre_client(void *data , int size);
//============================================================
void reception_tubeN_orchestre_client(void *data , int size);
//============================================================
void reception_tubeN_client_orchestre(void *data ,int size);
    
//===========================================================
void envoi_tubeN_client_orchestre(void *data ,int size);

//============================================================
void destruction_tubeN_client_orchestre();
//============================================================
void destruction_tubeN_orchestre_client();
//============================================================
void destruction_sem_client_orchestre(int semId);

#endif
