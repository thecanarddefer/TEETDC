#include <stdio.h>
#include <elf.h>
#include <stdlib.h>



void  afficherRelTables(int nbreloc,Elf32_Rel ** relTable, Elf32_Ehdr header,Elf32_Sym ** STable,char * strTab);


void  afficherRelaTables(int nbreloc,Elf32_Rela ** relaTable, Elf32_Ehdr header);

void getRelTable (FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader,Elf32_Sym ** STable,char * strTab);

