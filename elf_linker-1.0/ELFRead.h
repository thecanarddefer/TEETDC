#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"
////////////////// Partie 4 ////////////////////

void printInfoSym(unsigned char info);

void readSymTab(FILE * fp, Elf32_Shdr** sheader, int symTabNum,Elf32_Sym ** STable, char * strTab);




int findSymTab(Elf32_Shdr** sheader, int max);


int findStrTabSym(Elf32_Shdr** sheader, Elf32_Ehdr header);

////////////////////////////////////////////////
////////////////// Partie 5 ////////////////////
void afficherInfosRel(Elf32_Word leMot);

 

void  afficherRelTables(int nbreloc,Elf32_Rel ** relTable, Elf32_Ehdr header);


void  afficherRelaTables(int nbreloc,Elf32_Rela ** relaTable, Elf32_Ehdr header);

void getRelTable (FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader);

////////////////////////////////////////////////
int main(int argc, char *argv[]);















