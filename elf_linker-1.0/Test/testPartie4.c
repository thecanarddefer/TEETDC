#include "../partie1.h"
#include "../partie2.h"
#include "../partie4.h"

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
	int symTabNum=findSymTab(sheader,header->e_shnum);//l'indice de la table de symbole
	int numSec = findStrTabSym(sheader,*header);//l'indice de la table des nom de symbole
	char* strTab = createStrTab(sheader, src, numSec);//crée la table des noms de symbole
	Elf32_Sym * STable[sheader[symTabNum]->sh_size/sizeof(Elf32_Sym)];//création de la table des symbole
	readSymTab(src, sheader, symTabNum, STable, strTab);//lis la table des symbole
	afficherTabSymb(STable,sheader,symTabNum,strTab);//affiche la table des symbole
	freeSymTab(STable,sheader,symTabNum);//libère la table des symbole
	freeHeader(header);
	freeSHDR(sheader);//libère le sheader
}
