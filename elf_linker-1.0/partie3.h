#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

int indice_section(char* nom,Elf32_Ehdr header,Elf32_Shdr **sheader,FILE* fp);
	//Donnée :nom de la section recherchée (nom), l'entête de fichier (header), pointeur de la table des entête de section, et le fichier source (fp)
	//Fonction :permet de trouver une section a partir de son nom
	//Retour:l'indice de la section ou -1 si elle n'existe pas

void affichage_contenu_section (int num,Elf32_Ehdr header,Elf32_Shdr** sheader , FILE * f);
	//Donnée :indice de section (num), l'entête de fichier (header), pointeur de la table des entête de section, et le fichier source (fp)
	//Fonction :permet d'afficher le contenue d'une section
	//Retour:none

void alignement(int i);
	//Donnée :position curseur dans le contenu de la section (i)
	//Fonction :permet de faire l'alignement de l'affichage
	//Retour:none

void affiche_val_ASCII(uint8_t * contenu,int j,int i);
	//Donnée :le contenue d'une section sous forme de tableau (contenu),le nombre d'octet a traiter (j),position curseur dans le contenu de la section (i)
	//Fonction :permet d'afficher les j dernier octet lu du contenu en caractere ascii si il n'y as pas de caractère elle affiche un '.' 
	//Retour:none


