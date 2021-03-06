#include "../partie1.h"
#include "../partie2.h"

int main(int argc,char * argv[]){
	if(argc != 2){
		printf("Il faut 1 arguments");
		exit(0);
	}
	FILE * src = fopen(argv[1], "r");
	Elf32_Ehdr * header;
	header = elf_read_entete(src);
	Elf32_Shdr *sheader[header->e_shnum];//création du sheader
	lectureTableSectionHeader(src,*header,sheader);//remplissage  du sheader
	printTableSection(src,*header,sheader);//affichage du sheader
	freeHeader(header);
	freeSHDR(sheader);//libère le sheader
}
