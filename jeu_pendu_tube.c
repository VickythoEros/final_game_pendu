#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

// les constatntes
#define R 0
#define W 1
#define len 15



// permet de voir si le mot saisi existe dans la chaine
int rechercheLettre(char lettre, char motSecret[], int lettreTrouvee[]);


// verifie si le joueur a gagné
int gagne(int lettreTrouvee[]);


// choisir un chiffre aleatoirement
int ligneChiffre();

// retourne la taile d'un mot
int tailleMot(char mot[]);



// main
int main(int argc, char* argv[])
{	

	// declarations des variables
	char dico[6][11] = {"BONJOUR","MONSIEUR","KONE","VAKARAMOKO","ESATIC","ABIDJAN"};
	
	int PversF[2],FversP[2];
	//char message[11] ;
	int message1[11];
	int tailleMotSecret;
	
	char lettre=0; // Stocke la lettre proposée par l'utilisateur (retour du scanf)
        char motSecret[11] ; // C'est le mot à trouver
	
       int i = 0; 
	char tableauLettres[26]; // contient toutes les lettres entrées par le fils
	
	strcpy(motSecret,dico[ligneChiffre()]); // copie le mot aleatoirement dans dico dans motSecret
        tailleMotSecret = tailleMot(motSecret); // retourne la taile du mot secret
	
	
	
	
	if(pipe(PversF) ==-1 ){ // si erreur lors de la creation du tube PversF
	
		puts("erreur lors de la creation du tube PversF\n");
		exit(0);
	}
	if( pipe(FversP)== -1 ){// si erreur lors de la creation du tube FversP
	
		puts("erreur lors de la creation du tube FversP\n");
		exit(0);
	}

	switch(fork()){ // creation du processus fils
	
	case -1:
		{
			puts("erreur de creation du processs fils\n");
			exit(1);
		};break;
	case 0:
		{	// le processus fils
				int message =0;
				
				
				srand(time(NULL));
				while(message != 1)
				{	close(PversF[W]); // on empeche le pere d'ecrire
					close(FversP[R]);// on empeche le fils de lire
				
					lettre = rand()%26+'A';
					sleep(3); // le fils attend pendant 3 secondes avant de repondre *** il reflechir  ****
					write(FversP[W],&lettre,sizeof(lettre) + 1);
					read(PversF[R],&message,1);
				
				}
					
				
		};break;
			
	default:
			{ // le processus pere
				
       			int lettreTrouvee[11] = {0};
       			int coupsRestants = 10;
       			
				while (coupsRestants > 0 && !gagne(lettreTrouvee))
				{ 	char saisir =0;
					int stop = 0;
				
					printf("\n\nIl vous reste %d coups a jouer", coupsRestants);
					printf("\nQuel est le mot secret ? ");


					for (i = 0 ; i < tailleMotSecret ; i++)
					{
					    if (lettreTrouvee[i]) 
						printf("%c", motSecret[i]); 
					    else
						printf("*"); 
					}	
					
					close(FversP[W]); //on empeche le fils de d'ecrire
					close(PversF[R]); // on empeche le pere de lire
				
					printf("\nProposez une lettre : ");
					read(FversP[R],&saisir,sizeof(saisir)+1) ;
					printf(" %c ",saisir);
					
					
					if (!rechercheLettre(saisir, motSecret, lettreTrouvee))
					    {
						coupsRestants--; 
					    }
					    
					if (coupsRestants > 0 && !gagne(lettreTrouvee))
					{  
					
						write(PversF[W],&stop, sizeof(stop) + 1);
					}
					else{
						stop = 1;
						write(PversF[W],&stop, sizeof(stop) + 1);
					}
					
				}
				
				
				if (gagne(lettreTrouvee))
			   	 printf("\n\nGagne ! Le mot secret etait bien : %s \n\n", motSecret);
				else
			   	 printf("\n\nPerdu ! Le mot secret etait : %s \n\n", motSecret);

				
			}
		}
	

    return 0;
}




// permet de voir si le mot saisi existe dans la chaine
int rechercheLettre(char lettre, char motSecret[], int lettreTrouvee[])
{
    int i = 0;
    int bonneLettre = 0;

    // On parcourt motSecret pour vérifier si la lettre proposée y est 
    for (i = 0 ; motSecret[i] != '\0' ; i++)
    {
        if (lettre == motSecret[i]) // Si la lettre y est
        {
            bonneLettre = 1; // On mémorise que c'était une bonne lettre
            lettreTrouvee[i] = 1; // On met à 1 la case du tableau de booléens correspondant à la lettre actuelle
        }
    }

    return bonneLettre;
}



// verifie si le joueur a gagné
int gagne(int lettreTrouvee[])
{
    int i = 0;
    int joueurGagne = 1;

    for (i = 0 ; i < 6 ; i++)
    {
        if (lettreTrouvee[i] == 0)
            joueurGagne = 0;
    }

    return joueurGagne;
}


// choisir un chiffre aleatoirement
int ligneChiffre()
{
	int choix ;
	
	// generation d'un nombre aletoire entre 1 et 6
	srand(time(NULL));
	choix = rand()%6; 
	
	return choix;
}


// retourne la taile d'un mot
int tailleMot(char mot[])
{
	int i ;
	for(i=0;mot[i] != '\0';i++){}
	
	return i;
}
