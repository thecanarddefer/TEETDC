#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"
// if latabledessections est vide : e_shnum = 0
// sinon :
// e_shentsize * e_shnum = Taille de la table des sections
// e_shoff : pour pouvoir se positionner ou bon nous semble







int findSymTab(Elf32_Shdr** sheader, int max){
    int num=0;
    while(sheader[num]->sh_type!=2 && num< max){
        num++;
    }
    return num;
}
int findStrTabSym(Elf32_Shdr** sheader, Elf32_Ehdr header){
    int num=0;
    while(sheader[num]->sh_type!=3 || num==header.e_shstrndx){
        num++;
    }
    return num;
}



////////////////// Partie 5 ////////////////////
   void afficherInfosRel(Elf32_Word leMot){
        printf("Index de la table des symboles : %d\n",ELF32_R_SYM(leMot));
        printf("Type de relocation a appliquer : %d\n",ELF32_R_TYPE(leMot));
        printf("Info %x\n",leMot);
    }

 

void  afficherRelTables(int nbreloc,Elf32_Rel ** relTable, Elf32_Ehdr header){
            int j=0;
            while (j<nbreloc){
            
            //Elf32_Addr r_offset
            printf("Relocation numéro : %d",j);
            printf("\tOffset : %x\n",relTable[j]->r_offset);
            //Elf32_word r_info
            //printf("\tinfo : %d\n",laTable->relTable[i].r_info);
            afficherInfosRel(relTable[j]->r_info);
            j++;}
        
    }


void  afficherRelaTables(int nbreloc,Elf32_Rela ** relaTable, Elf32_Ehdr header){
            int j=0;
            while (j<nbreloc){
            
            //Elf32_Addr r_offset
            printf("Relocation numéro : %d",j);
            printf("\tOffset : %x\n",relaTable[j]->r_offset);
            //Elf32_word r_info
            //printf("\tinfo : %d\n",laTable->relTable[i].r_info);
            afficherInfosRel(relaTable[j]->r_info);
            j++;}
        
    }

void getRelTable (FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader){
  	int nbreloc=0,nbreloca=0;
    int compteurRel=0 ,compteurRela=0;
    for(int i=0;i<header.e_shnum;i++){
        switch (sheader[i]->sh_type){
            case 9 : compteurRel++;nbreloc=(sheader[i]->sh_size/sizeof(Elf32_Rel));break;
            case 4 : compteurRela++;nbreloca=(sheader[i]->sh_size/sizeof(Elf32_Rela));break;
            default: break;
        }
    }

	

    printf("compteurs : %d %d \n",compteurRel,compteurRela);
    Elf32_Rel * relTable[compteurRel][nbreloc];
    Elf32_Rela * relaTable[compteurRela][nbreloca];

    for(int i=0;i<compteurRel;i++){
        for (int j=0;j<nbreloc;j++){
            relTable[i][j] = (Elf32_Rel*)malloc(sizeof(Elf32_Rel));
            }
    }

    for(int i=0;i<compteurRela;i++){
        for (int j=0;j<nbreloca;j++){
            relaTable[i][j] = (Elf32_Rela*)malloc(sizeof(Elf32_Rela));
            }
    }


    compteurRel=0;
    compteurRela = 0;

    for(int i=0;i<header.e_shnum;i++){
        switch (sheader[i]->sh_type){
            case 9 :
                fseek(fp,sheader[i]->sh_offset,SEEK_SET);
                for (int j=0; j<nbreloc ;j++){
                    fread(relTable[compteurRel][j], 1,sizeof(Elf32_Rel),fp);
                    }
                compteurRel++;
                break;
            
            case 4 : 
                fseek(fp,sheader[i]->sh_offset,SEEK_SET);
                for (int j=0; j<nbreloca ;j++){
                    fread(relaTable[compteurRela][j], 1,sizeof(Elf32_Rela),fp);
                    }
                compteurRela++;
                break;

            default: 
                break;
        }
    }
    for (int i=0;i<compteurRel;i++){
        printf(" \n");
        printf("table des relocation %d\n",i);
        printf(" \n");
        afficherRelTables(nbreloc, relTable[i],header);}
    for (int i=0;i<compteurRela;i++){
        printf(" \n");
        printf("table des relocation avec addent %d\n",i);
        printf(" \n");
        afficherRelaTables(nbreloca, relaTable[i],header);}
}

////////////////////////////////////////////////

void printInfoSym(unsigned char info){
  switch(ELF32_ST_BIND(info)){
    case 0: printf("LOCAL");break;
    case 1: printf("GLOBAL");break;
    default:printf("AUTRE");break;
  }
  printf("\t");
  switch(ELF32_ST_TYPE(info)){
    case 0: printf("NOTYPE");break;
    case 1: printf("OBJECT");break;
    case 2: printf("FUNC");break;
    case 3: printf("SECTION");break;
    case 4: printf("FILE");break;
    case 5: printf("COMMON");break;
    default:printf("AUTRE");break;
  }
  printf("\t");
}

void readSymTab(FILE * fp, Elf32_Shdr** sheader, int symTabNum,Elf32_Sym ** STable, char * strTab){
    fseek(fp,sheader[symTabNum]->sh_offset,SEEK_SET);
    printf("Num\t");
    printf("Valeur\t");
    printf("Tail\t");
    printf("Lien\t");
    printf("Type\t");
    printf("Ndx\t");
    printf("Nom\t\n");
    for(int i=0; i<sheader[symTabNum]->sh_size/sizeof(Elf32_Sym);i++){
        STable[i]=(Elf32_Sym*)malloc(sizeof(Elf32_Sym));
        fread(STable[i],1, sizeof(Elf32_Sym), fp);
        printf("%d\t",i);
        printf("0x%X\t",STable[i]->st_value);
        printf("%d\t",STable[i]->st_size);
        printInfoSym(STable[i]->st_info);
        //printf("autre du symbole : %d\n",STable[i]->st_other);
        if(STable[i]->st_shndx == 0) printf("UND\t");
        else printf(" %d\t",STable[i]->st_shndx);
        int j = STable[i]->st_name;
        while(strTab[j] != '\0'){
          printf("%c", strTab[j]);
          j++;
        }
        printf("\n");
    }
}

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
      Elf32_Shdr *sheader[header->e_shnum];
      printf("\n");
      printf(" /// section HEADER TABLE /// \n");
      printf("\n");

      afficherTableSectionHeader(src,*header,sheader);
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
        char* strTab = malloc(sheader[numSec]->sh_size);
        strTab = createStrTab(sheader, src, numSec);
        Elf32_Sym * STable[sheader[symTabNum]->sh_size/sizeof(Elf32_Sym)];
        readSymTab(src, sheader, symTabNum, STable, strTab);
      }else{
        printf("il n'y a pas de table des symboles \n");

      }
  	  getRelTable(src,*header,sheader);
	char nom[20]=".rel.text";
	affichage_contenu_section(indice_section(nom,*header,sheader,src),*header,sheader,src);
    } else {
   		printf("bad file");
  	}


}
