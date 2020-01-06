#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

void printKeyFlags();
	//Donnée :none
	//Fonction :affiche la description des flags
	//Retour:None

void printSh_Type(Elf32_Word leMot);
	//Donnée :le type d'une section sous forme binaire (Elf32_Word)
	//Fonction :affiche le type d'une section
	//Retour:None
	
    
void printFlagz(Elf32_Word leMot);
	//Donnée :les flags d'une section sous forme binaire (Elf32_Word)
	//Fonction :affiche les flags d'une section
	//Retour:None
	

void lectureTableSectionHeader(FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader);
	//Donnée :le fichier source (fp), l'entete du fichier (header), le pointeur du tableau des entete de section (sheader)
	//Fonction :lis la table des section header
	//Retour:None


void printTableSection(FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader);
	//Donnée :le fichier source (fp), l'entete du fichier (header), le pointeur du tableau des entete de section (sheader)
	//Fonction :affiche la table des section header
	//Retour:None
	

char * createStrTab(Elf32_Shdr** sheader, FILE * fp, Elf32_Half num);
	//Donnée :le pointeur du tableau des entete de section (sheader),le fichier source (fp), l'index de la table des noms de section du tableau des entetes de section (num)
	//Fonction :creer la table des noms de section
	//Retour: la table des noms de section (strTab)


void freeSHDR(Elf32_Shdr ** sheader);
	//Donnée :le pointeur du tableau des entete de section (sheader)
	//Fonction :libere la table des section header
	//Retour:None
	
