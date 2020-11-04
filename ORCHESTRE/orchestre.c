#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "client_orchestre.h"
#include "service_orchestre.h"


//pipe
#include <sys/types.h>
#include <sys/stat.h>
//sem
#include <sys/ipc.h>
#include <sys/sem.h>

//anonyme pipe
#include <unistd.h>

#include <fcntl.h>  
#include "myassert.h"

#include <string.h>

#include <sys/wait.h>

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <fichier config>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

void launch(const char *basename, int semid, int fd_tubeA_reader, char * tubeN_sc , char * tubeN_cs )
{
    char *argv[6];
	argv[0]=malloc(sizeof(char)*(strlen(basename)+1));
    strcpy(argv[0],basename);

	argv[1] = malloc(sizeof(char)*10);	
    sprintf(argv[1],"%d",semid);

    argv[2] = malloc(sizeof(char)*10);
	sprintf(argv[2],"%d",fd_tubeA_reader);
   
    argv[3] = tubeN_sc;
	argv[4] = tubeN_cs;
	argv[5] = NULL;

	char link[50];
	strcpy(link,"../");
	strcat(link,basename);

	int ret =execv(link,argv);
	myassert(ret!=-1,"service non executé");
}




int main(int argc, char * argv[])
{
    if (argc != 2)
        usage(argv[0], "nombre paramètres incorrect");

	// Pour la communication avec les clients
    // - création de 2 tubes nommés pour converser avec les clients ++
    // - création d'un sémaphore pour que deux clients ne
    //   ne communiquent pas en même temps avec l'orchestre ---
	create_tubeN_orchestre_client();
	create_tubeN_client_orchestre();

	int semId =create_sem_orchestre_client();
	init_sem_client_orchestre(semId);
	
    // lancement des services, avec pour chaque service :

    // - création d'un tube anonyme pour converser (orchestre vers service)              ++
    // - un sémaphore pour que le service préviene l'orchestre de la
    //   fin d'un traitement 
    // - création de deux tubes nommés pour les communications entre                     ++
    //   les clients et le service
	int sem2Id =create_sem_orchestre_service();
	init_sem_orchestre_service(sem2Id,1);

	config_init(argv[1]);
	int nbservice= config_getNbServices();
	printf("=== > Nb service : %d \n", nbservice);

	for(int i=1; i<=nbservice; i++){

		printf("%d. %s \n",i,config_getExeName(i));
		
	}

	tubeA * listeA= malloc(sizeof(tubeA)*nbservice);
	tubeN * listeN= malloc(sizeof(tubeN)*nbservice);

	char append[2];

	char * cli_service = malloc(sizeof(char)*50);
	char * service_cli = malloc(sizeof(char)*50);
	
	for(int i=0; i<nbservice; i++){
		strcpy(cli_service,"client_service");
	    strcpy(service_cli,"service_client");

		//Creation de tube annonyme
		create_tubeA_orchestre_service(&listeA[i]);

	    //creation de tube nommmé
		sprintf(append,"%d",i);
		strcat(cli_service,append);
		strcat(service_cli,append);
		
		
		listeN[i].namecs = malloc(sizeof(char)*50);
		listeN[i].namesc = malloc(sizeof(char)*50);
		strcpy(listeN[i].namecs ,cli_service);
		strcpy(listeN[i].namesc ,service_cli);

		printf("tubeN  :  %s \n ",listeN[i].namecs);
		printf("tubeN  :  %s \n ",listeN[i].namesc);

		create_tubeN_client_service(listeN[i].namecs);
		create_tubeN_client_service(listeN[i].namesc);
	}




    while (true)
    {
        // attente d'une demande de service du client
		printf("Attente : \n");
		int c;
		sleep(3);
		reception_tubeN_client_orchestre(&c,sizeof(int));
		c=c-1;
		printf("****** fdReadA orch_Service : %d \n,",listeA[c].fdRead);
        // détecter la fin des traitements lancés précédemment via
        // les sémaphores dédiés (attention on n'attend pas la
        // fin des traitement, on note juste ceux qui sont finis)
		if(semctl(sem2Id,0,GETVAL)==1)
			printf("Aucun traitement en ce moment \n");

		if(c ==-1){
			envoi_tubeN_orchestre_client("code_acceptation",sizeof(char)*(strlen("code_acceptation")+1));
			break;
		}else if(!config_isServiceOpen(c+1)){
			printf("erreur service non ouvert");	
		}else{
			int res = fork();
			if(res==0){
			//	donner  nom_service,  semid,   fdread       , service-client , client - service 
				launch(config_getExeName(c+1),sem2Id,listeA[c].fdRead, listeN[c].namesc , listeN[c].namecs );
			}else{
			
			write_tubeA_orchestre_service(listeA[c],"codeI", sizeof(char)*(strlen("code1")+1));
			write_tubeA_orchestre_service(listeA[c],"1234", sizeof(char)*(strlen("1234")+1));

			envoi_tubeN_orchestre_client("code_acceptation",sizeof(char)*(strlen("code_acceptation")+1));
			envoi_tubeN_orchestre_client("1234",sizeof(char)*5);
			
			envoi_tubeN_orchestre_client(listeN[c].namecs,sizeof(char)*(strlen(listeN[c].namecs)+1));
			envoi_tubeN_orchestre_client(listeN[c].namesc,sizeof(char)*(strlen(listeN[c].namesc)+1));
			
			char recu[10];
			reception_tubeN_client_orchestre(recu,sizeof(char)*5);
			printf("[Orchestre] confirmation client : %s\n",recu);
			
			}
	
		}
		// analyse de la demande du client
		// si ordre de fin
		//     retour d'un code d'acceptation
		//     sortie de la boucle
		// sinon si service non ouvert
		//     retour d'un code d'erreur
		// sinon si service déjà en cours de traitement
		//     retour d'un code d'erreur
		// sinon
		//     génération d'un mot de passe
		//     envoi d'un code de travail au service (via le tube anonyme)
		//     envoi du mot de passe au service (via le tube anonyme)

		//     envoi au client d'un code d'acceptation (via le tube nommé)
		//     envoi du mot de passe au client (via le tube nommé)
		//     envoi des noms des tubes nommés au client (via le tube nommé)

		// attente d'un accusé de réception du client

		
    }
	printf("attente de la fin des traitements en cours (via les sémaphores)\n");
    // attente de la fin des traitements en cours (via les sémaphores)
	prendre_sem_client_service(sem2Id);
    // envoi à chaque service d'un code de fin

    // attente de la terminaison des processus services
	wait(NULL);
    // destruction des tubes
	for(int i=0;i<nbservice;i++){
		destruct_tubeN(listeN[i].namecs);
		destruct_tubeN(listeN[i].namesc);
	}
	destruction_sem_client_orchestre(semId);
	destruct_sem_orchestre_service(sem2Id);
    return EXIT_SUCCESS;
}
