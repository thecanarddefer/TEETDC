#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "ELFRead.h"
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"

int isMergeable(Elf32_Word type){
    switch(type){
        case(SHT_NULL): return 1;
        case(SHT_PROGBITS): return 1;
        case(SHT_SYMTAB): return 1;
        case(SHT_STRTAB): return 1;
    }
    return 0;
}

int mergeSheader(Elf32_Shdr ** sheader1, Elf32_Ehdr header1, Elf32_Shdr ** sheader2, Elf32_Ehdr header2, Elf32_Shdr ** sheaderFin, int nbSymbole, int tailleSTRTAB){
    int count = 0;
    int decalage = 0;
    int j;
    for(int i=0; i<header1.e_shnum;i++){
        if(isMergeable(sheader1[i]->sh_type)){
            j=0;
            while(sheader1[i]->sh_name =!sheader2[j]->sh_name && j<header2.e_shnum){
                j++;
            }
            if(j!=header2.e_shnum){
                sheaderFin[i]=sheader1[i];//Attention PE mauvaise copie!!!!!!!!!!!!!!!!!!!!!!!!!!
                int tmpsize =sheader1[i]->sh_size;
                sheaderFin[i]->sh_offset+=decalage;
                if(sheaderFin[i]->sh_type==SHT_SYMTAB){
                    sheaderFin[i]->sh_size= sizeof(Elf32_Sym)*nbSymbole;
                    decalage+= sizeof(Elf32_Sym)*nbSymbole-tmpsize;
                }else if(sheaderFin[i]->sh_type==SHT_STRTAB && i!= header1.e_shstrndx){
                    sheaderFin[i]->sh_size=tailleSTRTAB;
                    decalage+= tailleSTRTAB-tmpsize;
                }else{
                    sheaderFin[i]->sh_size+=sheader2[j]->sh_size;
                    decalage+=sheader2[j]->sh_size;
                }     
                //gestion de l'allign
                if(sheader1[i]->sh_addralign<sheader2[i]->sh_addralign){
                    sheaderFin[i]->sh_addralign=sheader2[i]->sh_addralign;
                }
                if (sheaderFin[i]->sh_size % sheaderFin[i]->sh_addralign!=0){
                    sheaderFin[i]->sh_size+= sheaderFin[i]->sh_addralign - (sheaderFin[i]->sh_size % sheaderFin[i]->sh_addralign);
                }
                //
               
                
                //gestion de l'allign info tailltab
            }
            else{
                sheaderFin[i]=sheader1[i];
            }
        }else{
            sheaderFin[i]=sheader1[i];
        }
        
        count++;
    }
    decalage = sheaderFin[header1.e_shnum - 1]->sh_offset + sheaderFin[header1.e_shnum - 1]->sh_size;
    for(int i=0; i<header2.e_shnum;i++){
        j=0;
        while(sheader2[i]->sh_name =!sheaderFin[j]->sh_name && j<header1.e_shnum){
            j++;
        }
        if(j==header1.e_shnum){
            sheaderFin[i+header1.e_shnum]=sheader2[i];
            sheaderFin[i+header1.e_shnum]->sh_offset=decalage;
            decalage+=sheader2[i]->sh_size;
            sheaderFin[i+header1.e_shnum]->sh_name+=sheader1[header1.e_shstrndx]->sh_size;
            if(sheaderFin[i+header1.e_shnum]->sh_type==SHT_REL ){
                sheaderFin[i+header1.e_shnum]->sh_link=header1.e_shstrndx;
            }
            count++;
        }
    }

    return count;
}
char * getNomSymboles(Elf32_Word name,char* strTab, int taille){
    char nom[taille];
    int taillefinale=0;
    while(strTab[name]!='\0'){
        nom[taillefinale]=strTab[name];
        taillefinale++;
        name++;
    }
    char * nomFinal=malloc(taillefinale+2);
    for(int i=0; i<=taillefinale;i++){
        nomFinal[i]=nom[i];
    }
    nomFinal[taillefinale+1] = '\0';
    return nomFinal;
}
int memeString(char* s1, char * s2){
    int i = 0;
    while(s1[i] == s2[i] && (s2[i] != '\0' || s1[i] != '\0')){
        i++;
    }
    if(s1[i] == s2[i]){
        return 1;
    }
    else {
        return 0;
    }
}

void mergeSymAndStr(char* strTab1,char* strTab2,Elf32_Sym ** symTable1,Elf32_Sym ** symTable2, char * strTabFin, Elf32_Sym ** symTableFin, int t1, int t2, int * compteurSTRTAB, int * count){
    *compteurSTRTAB=1;
    strTabFin[0]='\0';
    for(int i = 0; i < t1;i++){
        symTableFin[i] = symTable1[i];
        int l= symTableFin[i]->st_name;
        symTableFin[i]->st_name=*compteurSTRTAB;
        while(strTab1[l]!='\0'){
            strTabFin[*compteurSTRTAB]=strTab1[l];
            l++;
            *compteurSTRTAB++;
        }
        strTabFin[*compteurSTRTAB]='\0';
        *compteurSTRTAB++;
        

    }
    *count=t1;
    int k = 0;
    for(int j = 0;j < t2;j++){
        if(ELF32_ST_BIND(symTable2[j]->st_info)==1){
            while(k<t1 && !memeString(getNomSymboles(symTable2[j]->st_name, strTab2, t2), getNomSymboles(symTableFin[k]->st_name, strTab1, t1))){
                k++;
            }
            if(k==t1){
                symTableFin[*count]=symTable2[j];
                int l= symTableFin[*count]->st_name;
                symTableFin[*count]->st_name=*compteurSTRTAB;
                while(strTab2[l]!='\0'){
                    strTabFin[*compteurSTRTAB]=strTab2[l];
                    l++;
                    *compteurSTRTAB++;
                }
                strTabFin[*compteurSTRTAB]='\0';
                *compteurSTRTAB++;
                *count++;
            } else{
                if(symTable2[j]->st_shndx != STN_UNDEF && symTableFin[k]->st_shndx != STN_UNDEF){
                    printf("Line 2346 : ERROR : 2 global variables have same name");
                    exit(0);
                } else if(symTable2[j]->st_shndx != STN_UNDEF && symTableFin[k]->st_shndx == STN_UNDEF){
                    symTableFin[k]=symTable2[j];
                }
            }

            k=0;
        }
        else{
            symTableFin[*count]=symTable2[j];
            int l= symTableFin[*count]->st_name;
            symTableFin[*count]->st_name=*compteurSTRTAB;
            while(strTab2[l]!='\0'){
                strTabFin[*compteurSTRTAB]=strTab2[l];
                l++;
                *compteurSTRTAB++;
            }
            strTabFin[*compteurSTRTAB]='\0';
            *compteurSTRTAB++;
            *count++;
        }
    }
}




Elf32_Ehdr createElfHeader(Elf32_Shdr ** sheaderFin, Elf32_Ehdr header1, int nombreDeSection){
    Elf32_Ehdr headerFin = header1;
    headerFin.e_shnum = nombreDeSection;
    headerFin.e_shoff = sheaderFin[nombreDeSection]->sh_offset + sheaderFin[nombreDeSection]->sh_size;
    return headerFin;
}

//////////////MAIN////////////////
int main(int argc, char *argv[]){
	if(argc != 4){
		printf("Il faut 3 arguments : fich1, fich2, dest");
		exit(0);
  	}
    FILE * src1 = fopen(argv[1], "r");
    FILE * src2 = fopen(argv[2], "r");
    FILE * dest = fopen(argv[3], "w");

    //Recuperer les Headers des 2 fichiers d'entrées
    Elf32_Ehdr * header1 = elf_read_entete(src1);
    Elf32_Ehdr * header2 = elf_read_entete(src2);

    //Recuperer les Sections Headers des 2 fichiers d'entrées
    Elf32_Shdr * sheader1[header1->e_shnum];
    Elf32_Shdr * sheader2[header2->e_shnum];
    afficherTableSectionHeader(src1,*header1,sheader1);
    afficherTableSectionHeader(src2,*header2,sheader2);

    //Recuperer les Section String table 2 fichiers d'entrées
    char* strTabSection1 = malloc(sheader1[header1->e_shstrndx]->sh_size);
    strTabSection1 = createStrTab(sheader1, src1, header1->e_shstrndx);
    char* strTabSection2 = malloc(sheader2[header2->e_shstrndx]->sh_size);
    strTabSection2 = createStrTab(sheader2, src2, header2->e_shstrndx);

    //Recuperer la table Symbole et String table des symboles 2 fichiers d'entrées
    //fichier 1
    int symTabNum=findSymTab(sheader1,header1->e_shnum);
    int numSec = 0;
    char* strTab1;
    // if(symTabNum < header1->e_shnum){
        numSec = findStrTabSym(sheader1,*header1);
        strTab1 = malloc(sheader1[numSec]->sh_size);
        strTab1 = createStrTab(sheader1, src1, numSec);
        Elf32_Sym * symTable1[sheader1[symTabNum]->sh_size/sizeof(Elf32_Sym)];
        readSymTab(src1, sheader1, symTabNum, symTable1, strTab1);
    // }else{
    //     printf("il n'y a pas de table des symboles \n");
    // }
    //fichier 2
    int symTabNum2=findSymTab(sheader2,header2->e_shnum);
    int numSec2 = 0;
    char* strTab2;
   // if(symTabNum2 < header2->e_shnum){
        numSec2 = findStrTabSym(sheader2,*header2);
        strTab2 = malloc(sheader2[numSec2]->sh_size);
        strTab2 = createStrTab(sheader2, src2, numSec2);
        Elf32_Sym * symTable2[sheader2[symTabNum2]->sh_size/sizeof(Elf32_Sym)];
        readSymTab(src2, sheader2, symTabNum2, symTable2, strTab2);
    // }else{
    //     printf("il n'y a pas de table des symboles \n");
    // }
    //Fusion de la table des symboles et de la strTab associé
    int symt1 = sheader1[symTabNum]->sh_size/sizeof(Elf32_Sym);
    int symt2 = sheader2[symTabNum2]->sh_size/sizeof(Elf32_Sym);
    char * strTabFin = malloc(sheader1[numSec]->sh_size + sheader2[numSec2]->sh_size);
    Elf32_Sym * symTableFin[sheader1[symTabNum]->sh_size/sizeof(Elf32_Sym) + sheader2[symTabNum2]->sh_size/sizeof(Elf32_Sym)];
    int nbSymboles;
    int tailleSTRTAB;
    mergeSymAndStr(strTab1, strTab2, symTable1, symTable2,  strTabFin, symTableFin, symt1, symt2,&tailleSTRTAB,&nbSymboles);

    //Fusion des sections
  
    Elf32_Shdr * sheaderFinTmp[header1->e_shnum + header2->e_shnum];
    int nombreDeSection=mergeSheader(sheader1, *header1, sheader2, *header2, sheaderFinTmp,nbSymboles,tailleSTRTAB);
    Elf32_Shdr * sheaderFin[nombreDeSection];
    for(int i=0; i<nombreDeSection;i++){
        sheaderFin[i]=sheaderFinTmp[i];
    }
    //Creation du ELFHeader
    

    createElfHeader(sheaderFin, *header1, nombreDeSection);
    fclose(src1);
    fclose(src2);
    fclose(dest);
}