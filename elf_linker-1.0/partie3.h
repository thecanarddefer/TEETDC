#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

int indice_section(char* nom,Elf32_Ehdr header,Elf32_Shdr **sheader,FILE* fp);
	//permet de trouver une section a partir de son nom

void affichage_contenu_section (int num,Elf32_Ehdr header,Elf32_Shdr** sheader , FILE * f);
	//permet d'afficher le contenue d'une section


