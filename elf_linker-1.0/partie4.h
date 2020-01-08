#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

void printInfoSymType(unsigned char info);
	//Donnée : l'information sur le symbole sous forme binaire (info)
	//Fonction :permet de print le type du symbole
	//Retour:none

void printInfoSymVisi(unsigned char info);
	//Donnée : l'information sur le symbole sous forme binaire (info)
	//Fonction :permet de print la visibilite du symbole
	//Retour:none
	

void printInfoSymBind(unsigned char info);
	//Donnée : l'information sur le symbole sous forme binaire (info)
	//Fonction :permet de print le lien du symbole
	//Retour:none
	

void readSymTab(FILE * fp, Elf32_Shdr** sheader, int symTabNum,Elf32_Sym ** STable, char * strTab);
	//Donnée : le fichier source (fp), le pointeur de la table des section (sheader),l'indice de la table des symbole (symTabNum), le pointeur de la table des symbole (STable),la table des nom de symbole(strTab)
	//Fonction :permet de lire la table des symbole
	//Retour:none
	
	

int findSymTab(Elf32_Shdr** sheader, int max);
	//Donnée : le pointeur de la table des section (sheader),le nombre total des section (max)
	//Fonction :permet de trouver la table de symbole
	//Retour:l'indice de la table des symboles
	

void afficherTabSymb(Elf32_Sym ** STable, Elf32_Shdr** sheader, int symTabNum,char * strTab);
	//permet d'afficher la table des symboles

int findStrTabSym(Elf32_Shdr** sheader, Elf32_Ehdr header);
	//Donnée : le pointeur de la table des section (sheader),l'entête de fichier (header)
	//Fonction :permet de trouver la table des noms de symbole
	//Retour:l'indice de la table des nom de symbole

void freeSymTab(Elf32_Sym ** STable,Elf32_Shdr** sheader,int symTabNum);

