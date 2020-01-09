#include "../partie1.h"

int main(int argc,char * argv[]){
    if(argc != 2){
		printf("Il faut 1 arguments");
		exit(0);
  	}
  	FILE * src = fopen(argv[1], "r");
    Elf32_Ehdr * header;
    header = elf_read_entete(src);
    afficherheaderELF(header);
    freeHeader(header);
}