/*
 * Indications (à respecter) :
 * - Les erreurs sont gérées avec des assert ; les erreurs traitées sont :
 *    . appel trop tôt ou trop tard d'une méthode (cf. config.h)
 *    . fichier de configuration inaccessible
 *    . une position erronnée
 * - Le fichier (si on arrive à l'ouvrir) est considéré comme bien
 *   formé sans qu'il soit nécessaire de le vérifier
 *
 * Un code minimal est fourni et permet d'utiliser le module "config" dès
 * le début du projet ; il faudra le remplacer par l'utilisation du fichier
 * de configuration.
 * Il est inutile de faire plus que ce qui est demandé
 *
 * Dans cette partie vous avez le droit d'utiliser les entrées-sorties
 * de haut niveau (fopen, fgets, ...)
 */

// TODO include des .h système

#include "myassert.h"

#include "config.h"
#include <string.h>
#include <stdio.h>
// TODO Structure de données ici
#define maxNbservices 100

typedef struct {
  int nbServices;
  bool isOpen[maxNbservices];
  char exeName[100][maxNbservices];
}infoService;


static bool isCalled = false ;
static infoService data ;


void config_init(const char *filename)
{
    char s[100];
    isCalled=true;
     FILE *f = fopen(filename, "r");
	myassert(f!=NULL,"fopen Failed !");
	
 	fscanf(f,"%d",&data.nbServices);
	
	//data->open = malloc(sizeof(char *) * data->nbServices);
	//data->nameServices = malloc(sizeof(char *) * data->nbServices);

	int i=0;
	while (fscanf(f,"%s %s",s,data.exeName[i])!=EOF && i<data.nbServices){
        if(strcmp(s,"ouvert")==0)
           data.isOpen[i]=true;
		i++;
	}

	fclose(f);	
}

void config_exit()
{
    // TODO erreur si la fonction est appelée avant config_init
    myassert(isCalled ==true,"fct config_exit est appelee avant config_init");
    // TODO code vide par défaut, à remplacer
    data.nbServices=0;
    for(int i=0;i<data.nbServices;i++){
      data.isOpen[i]=false;
      strcpy(data.exeName[i]," ");
    }
    isCalled=false;
}

int config_getNbServices()
{
    // erreur si la fonction est appelée avant config_init
    // erreur si la fonction est appelée après config_exit
    myassert(isCalled ==true,"fct config_getNbServices appelee avt init ou exit");
    // code par défaut, à remplacer
    return data.nbServices;
}

bool config_isServiceOpen(int pos)
{
    // TODO erreur si la fonction est appelée avant config_init
    // TODO erreur si la fonction est appelée après config_exit
    // TODO erreur si "pos" est incorrect
    myassert(isCalled ==true,"fct config_isServiceOpen appelee avt init ou exit");
    myassert(pos>0 && pos<=data.nbServices,"pos<0 or pos>data.nbServices");
    // TODO code par défaut, à remplacer
    //bool open[] = {true, false, true};
    return data.isOpen[pos-1];
}

const char * config_getExeName(int pos)
{
    // TODO erreur si la fonction est appelée avant config_init
    // TODO erreur si la fonction est appelée après config_exit
    myassert(isCalled ==true,"fct config_getExeName appelee avt init ou exit");
    // TODO erreur si "pos" est incorrect
    //myassert(pos>0 && pos<data.nbServices,"pos<0 or pos>data.nbServices");

    // TODO code par défaut, à remplacer
    /*
    const char * names[] = {
        "SERVICES/service_somme",
        "SERVICES/service_compression",
        "SERVICES/service_max"
    };
    */
    return data.exeName[pos-1];
}
