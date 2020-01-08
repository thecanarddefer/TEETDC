#include "../partie1.h"
#include "../partie2.h"
#include "../partie3.h"

int main(int argc,char * argv[]){
	if(argc != 3){
		printf("Il faut 1 arguments");
		exit(0);
	}
	FILE * src = fopen(argv[1], "r");
	Elf32_Ehdr * header;
	header = elf_read_entete(src);
	Elf32_Shdr *sheader[header->e_shnum];//création du sheader
	lectureTableSectionHeader(src,*header,sheader);//remplissage  du sheader
	int i;
	switch (*argv[2]) {
		case '0': i=0;break;	
		case '1': i=1;break;
		case '2': i=2;break;
		case '3': i=3;break;
		case '4': i=4;break;
		case '5': i=5;break;
		case '6': i=6;break;
		case '7': i=7;break;
		case '8': i=8;break;
		case '9': i=9;break;
		default : i=-1;
	}
	affichage_contenu_section( i,*header,sheader,src);
	freeHeader(header);
	freeSHDR(sheader);//libère le sheader
}
