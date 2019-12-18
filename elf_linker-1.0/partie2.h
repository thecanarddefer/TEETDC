#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

void printKeyFlags();
	// affiche la description des flags

void printSh_Type(Elf32_Word leMot);
	// affiche le type d'une section
    
void printFlagz(Elf32_Word leMot);
	// affiche les flags d'une section

void afficherTableSectionHeader(FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader);
	// lis la table des section header

void printTableSection(FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader);
	// affiche la table des section header

char * createStrTab(Elf32_Shdr** sheader, FILE * fp, int num);
	// permet de lire un section header	

void freeSHDR(Elf32_Shdr ** sheader);
	// libere la table des section header
