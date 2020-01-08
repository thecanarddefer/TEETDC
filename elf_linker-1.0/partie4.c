#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie4.h"




int findSymTab(Elf32_Shdr** sheader, int max){
    	int num=0;
    	while(sheader[num]->sh_type!=SHT_SYMTAB && num< max){//boucle jusqu'à la section SYMTAB 
        	num++;
    	}
    	return num;
}

int findStrTabSym(Elf32_Shdr** sheader, Elf32_Ehdr header){
    	int num=0;
    	while(sheader[num]->sh_type!=SHT_STRTAB || num==header.e_shstrndx){//boucle jusqu'à la section STRTAB 
        	num++;
    	}
    	return num;
}

void printInfoSymType(unsigned char info){
  	switch(ELF32_ST_TYPE(info)){// affiche le type de symbole 
    		case STT_NOTYPE : printf("NOTYPE");break;
    		case STT_OBJECT: printf("OBJECT");break;
    		case STT_FUNC: printf("FUNC");break;
    		case STT_SECTION: printf("SECTION");break;
    		case STT_FILE: printf("FILE");break;
    		case STT_COMMON: printf("COMMON");break;
    		case STT_TLS: printf("TLS");break;
    		case STT_NUM: printf("NUM");break;
    		case STT_HIOS: printf("HIOS");break;
    		case STT_LOPROC: printf("LOPROC");break;
    		case STT_HIPROC: printf("HIPROC");break;
    		default:printf("AUTRE");break;
  	}
}


void printInfoSymBind(unsigned char info){
  	switch(ELF32_ST_BIND(info)){// affiche type de lien des symboles
    		case STB_LOCAL: printf("LOCAL");break;
    		case STB_GLOBAL: printf("GLOBAL");break;
    		case STB_WEAK: printf("WEAK");break;
    		case STB_NUM: printf("NUM");break;
    		case STB_HIOS: printf("HIOS");break;
    		case STB_LOPROC: printf("LOPROC");break;
    		case STB_HIPROC: printf("HIPROC");break;
    		default:printf("AUTRE");break;
  	}
}

void printInfoSymVisi(unsigned char info){
  	switch(ELF32_ST_VISIBILITY(info)){// affiche la visibilité du symbole
    		case STV_DEFAULT: printf("DEFAULT\t");break;
    		case STV_INTERNAL: printf("INTERNAL");break;
    		case STV_HIDDEN: printf("HIDDEN\t");break;
    		case STV_PROTECTED: printf("PROTECTED");break;
    		default:printf("AUTRE\t");break;
  	}
}

void afficherTabSymb(Elf32_Sym ** STable, Elf32_Shdr** sheader, int symTabNum,char * strTab){
    	printf("Num:\t");// affiche les colonnes de la table des symboles
    	printf("Valeur\t\t");
    	printf("Tail\t");
    	printf("Type\t");
    	printf("Lien\t");
	printf("Vis\t\t");
    	printf("Ndx\t");
    	printf("Nom\t\n");
    	for(int i=0; i<sheader[symTabNum]->sh_size/sizeof(Elf32_Sym);i++){//boucle sur les symboles
		// num
       		printf("%d:\t",i);
		// valeur
        	printf("%08X\t",STable[i]->st_value);
		// taille
        	printf("%d\t",STable[i]->st_size);
		// type
        	printInfoSymType(STable[i]->st_info);
		printf("\t");
		// lien
        	printInfoSymBind(STable[i]->st_info);
		printf("\t");
		// visi
		printInfoSymVisi(STable[i]->st_info);
		printf("\t");
        	// nxd
        	if(STable[i]->st_shndx == 0) {
			printf("UND\t");
		}
        	else {
		printf(" %d\t",STable[i]->st_shndx);
		}
		// nom
        	int j = STable[i]->st_name;
        	while(strTab[j] != '\0'){ // affiche le nom du symbole
          		printf("%c", strTab[j]);
          		j++;
        		}
        	printf("\n");
    	}
}

void readSymTab(FILE * fp, Elf32_Shdr** sheader, int symTabNum,Elf32_Sym ** STable, char * strTab){
   	fseek(fp,sheader[symTabNum]->sh_offset,SEEK_SET);
    	for(int i=0; i<sheader[symTabNum]->sh_size/sizeof(Elf32_Sym);i++){//boucle sur les symboles
        	STable[i]=(Elf32_Sym*)malloc(sizeof(Elf32_Sym));// alloue l'espace du symbole
        	fread(STable[i],1, sizeof(Elf32_Sym), fp);// lit le symbole
    	}
}

void freeSymTab(Elf32_Sym ** STable,Elf32_Shdr** sheader,int symTabNum){
	for(int i=0; i<sheader[symTabNum]->sh_size/sizeof(Elf32_Sym);i++){
		free(STable[i]);}//libere le symbole
}




