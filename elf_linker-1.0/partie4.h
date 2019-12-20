#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

void printInfoSymType(unsigned char info);
	//permet de print le type du symbole

void readSymTab(FILE * fp, Elf32_Shdr** sheader, int symTabNum,Elf32_Sym ** STable, char * strTab);
	//permet de lire la table des symbole

void printInfoSymVisi(unsigned char info);
	//permet de print la visibilite du symbole

void printInfoSymBind(unsigned char info);
	//permet de print le lien du symbole

int findSymTab(Elf32_Shdr** sheader, int max);
	//permet de trouver la table de symbole

void afficherTabSymb(Elf32_Sym ** STable, Elf32_Shdr** sheader, int symTabNum,char * strTab);
	//permet d'afficher la table des symbole

int findStrTabSym(Elf32_Shdr** sheader, Elf32_Ehdr header);
	//permet de trouver la string table 
