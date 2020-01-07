#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"
#include "partie4.h"
#include "partie5.h"

// if latabledessections est vide : e_shnum = 0
// sinon :
// e_shentsize * e_shnum = Taille de la table des sections
// e_shoff : pour pouvoir se positionner ou bon nous semble






int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Il faut 1 arguments");
		exit(0);
  	}
  	FILE * src = fopen(argv[1], "r");
  	if(src){
      Elf32_Ehdr * header;
      printf("\n");
      printf(" /// HEADER ELF /// \n");
      printf("\n");
        header = elf_read_entete(src);
	afficherheaderELF(header);
      Elf32_Shdr *sheader[header->e_shnum];
      printf("\n");
      printf(" /// section HEADER TABLE /// \n");
      printf("\n");

      lectureTableSectionHeader(src,*header,sheader);
      printf("\n");
      printf(" /// section ELF /// \n");
      printf("\n");
      for (int i=0; i<10;i++){
        affichage_contenu_section(i,*header,sheader,src);
      }
      printf("\n");
      printf(" /// Affichage des symboles /// \n");
      printf("\n");
      int symTabNum=findSymTab(sheader,header->e_shnum);
      if(symTabNum < header->e_shnum){
        int numSec = findStrTabSym(sheader,*header);
        //char strTab[sheader[numSec]->sh_size/sizeof(char)] = createStrTab(sheader, src, numSec);
        char* strTab = createStrTab(sheader, src, numSec);
        Elf32_Sym * STable[sheader[symTabNum]->sh_size/sizeof(Elf32_Sym)];
        readSymTab(src, sheader, symTabNum, STable, strTab);
	printf("\n");
     	 printf(" /// Affichage des relocation /// \n");
      	printf("\n");
	int compteurRel=nombre_reloc(*header,sheader);
	reloc *relTable[compteurRel];

 	getRelTable(src,*header,sheader,STable,strTab,compteurRel,relTable );
	//printf("%x",relTable[0][2]);
	
      }else{
        printf("il n'y a pas de table des symboles \n");

      }
	char nom[20]=".rel.text";
	//affichage_contenu_section(indice_section(nom,*header,sheader,src),*header,sheader,src);
    } else {
   		printf("bad file");
  	}


}
