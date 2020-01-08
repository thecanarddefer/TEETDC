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
	if(argc != 2){// vérification des argument
		printf("Il faut 1 arguments");
		exit(0);
	}
	FILE * src = fopen(argv[1], "r");
	if(src){// si le fichier est correctement ouvert
		Elf32_Ehdr * header;
		printf("\n");
		printf(" /// HEADER ELF /// \n");
		printf("\n");
		header = elf_read_entete(src);//création du header
		Elf32_Shdr *sheader[header->e_shnum];//création du sheader
		printf("\n");
		printf(" /// section HEADER TABLE /// \n");
		printf("\n");
		lectureTableSectionHeader(src,*header,sheader);//remplissage et affichage du sheader
		printf("\n");
		printf(" /// section ELF /// \n");
		printf("\n");
		for (int i=0; i<10;i++){//boucle sur les 10 première section
			affichage_contenu_section(i,*header,sheader,src);
		}
		printf("\n");
		printf(" /// Affichage des symboles /// \n");
		printf("\n");
		int symTabNum=findSymTab(sheader,header->e_shnum);//l'indice de la table de symbole
		if(symTabNum < header->e_shnum){
			int numSec = findStrTabSym(sheader,*header);//l'indice de la table des nom de symbole
			char* strTab = createStrTab(sheader, src, numSec);//crée la table des noms de symbole
			Elf32_Sym * STable[sheader[symTabNum]->sh_size/sizeof(Elf32_Sym)];//création de la table des symbole
			readSymTab(src, sheader, symTabNum, STable, strTab);//lis et affiche la table des symbole
			printf("\n");
			printf(" /// Affichage des relocation /// \n");
			printf("\n");
			int compteurRel=nombre_reloc(*header,sheader);//le nombre de section de relocation
			reloc *relTable[compteurRel];
			getRelTable(src,*header,sheader,STable,strTab,compteurRel,relTable );//crée le tableau des tables de relocation et les affiche
			freeReloc (relTable,compteurRel);//libère les tables de relocation
			freeSymTab(STable,sheader,symTabNum);//libère la table des symbole
		}
		else{
			printf("il n'y a pas de table des symboles \n");
		}
		freeHeader(header);//libère le header
		freeSHDR(sheader);//libère le sheader
		//affichage_contenu_section(indice_section(nom,*header,sheader,src),*header,sheader,src);
	} 
	else {
		printf("bad file");
	}
}











