#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"
#include "partie4.h"
#include "partie5.h"

int isMergeable(Elf32_Word type){
    switch(type){
        case(SHT_NULL): return 1;
        case(SHT_PROGBITS): return 1;
        case(SHT_SYMTAB): return 1;
        case(SHT_STRTAB): return 1;
        case(SHT_REL): return 1;
    }
    return 0;
}
void ecrireSheader(Elf32_Shdr ** sheader, FILE * dest, int nombreDeSection){
    fseek(dest, 0, SEEK_END);
    // printf("pos ecriture sheader: %ld\n",ftell(dest)+52);
    for(int i = 0; i < nombreDeSection;i++){
        fwrite(sheader[i], sizeof(Elf32_Shdr), 1, dest);
    }
}

void ecrireHeader(Elf32_Ehdr * header1, FILE * dest){
    fseek(dest, 0, SEEK_SET);
    fwrite(header1, header1->e_ehsize, 1, dest);
}

void ecrireSectionSymbole(FILE * dest, Elf32_Sym ** symTab, int nbSymbole){
    // printf("pos ecriture section sym: %ld\n",ftell(dest)+52);

    for(int i = 0; i < nbSymbole;i++){
        fwrite(symTab[i], sizeof(Elf32_Sym), 1, dest);
    }
}

void ecrireSectionStringSymbole(FILE * dest, char * strTabSym, int tailleSTRTAB){
    // printf("pos ecriture string sec sym: %ld\n",ftell(dest)+52);
    fwrite(strTabSym, tailleSTRTAB, 1, dest);
}

void ecrireSection(FILE * dest, FILE * src, Elf32_Shdr ** sheader, int indice){
    // printf("pos ecriture section num %d %ld Size : %d\n",indice, ftell(dest)+52, sheader[indice]->sh_size);
    uint8_t contenu[sheader[indice]->sh_size];
    fseek(src, sheader[indice]->sh_offset, SEEK_SET);
    fread(contenu, 1,sheader[indice]->sh_size, src);
    fwrite(contenu, sheader[indice]->sh_size, 1, dest);
}

void ecrireSectionMergeable(FILE * dest, FILE * src1, FILE * src2, Elf32_Shdr ** sheader1, Elf32_Shdr ** sheader2, int indice1, int indice2){
    ecrireSection(dest, src1, sheader1, indice1);
    ecrireSection(dest, src2, sheader2, indice2);
}
void printSheader(Elf32_Shdr ** sheaderFin, int count){
    for(int i = 0; i < count;i++){
        printf("Num : %d   ", i);
        printf("sizze : %d  ",sheaderFin[i]->sh_size);

        printf("offset : %d\n",sheaderFin[i]->sh_offset);
    }
}


char * getNomSymboles(Elf32_Word name,char* strTab, int taille){
    int compteur =name;
    char nom[taille];
    int taillefinale=0;
    while(strTab[compteur]!='\0'){
        nom[taillefinale]=strTab[compteur];
        taillefinale++;
        compteur++;
    }
    char * nomFinal=malloc(taillefinale+2);
    for(int i=0; i<taillefinale;i++){
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
// void mergeRelocationTable(Elf32_Shdr ** sheader1,Elf32_Shdr ** sheader2, indice1, indice2, char * strTabSymFinale, char * strTabSym2, char* strTabSection1, FILE * src1, FILE * src2 ){
//     //via indice 1 copier src1 dest

//     //sheader2->indice->sh_name->strTabSection1 trouver taille sheader1
//     //iterer sur sheader2 indice2 : ajouter taille sheader1 a l'offset , trouver le numero de symbole
//     //


// }

int mergeSheader(Elf32_Shdr ** sheader1, Elf32_Ehdr header1, Elf32_Shdr ** sheader2, Elf32_Ehdr header2, Elf32_Shdr ** sheaderFin, int nbSymbole, int tailleSTRTAB, Elf32_Sym ** symTab, char * strTabSym, FILE * dest, FILE * src1, FILE * src2, char* strTabSection1, char* strTabSection2, int* nouveauxIndices){
    int count = 0;
    //int decalage = 0;
    int offset = 52; //test avec un offset set a la taille du header
    int j;
    for(int i=0; i<header1.e_shnum;i++){
        
        if(isMergeable(sheader1[i]->sh_type)){
            j=0;

            while(j<header2.e_shnum &&!memeString(getNomSymboles(sheader1[i]->sh_name, strTabSection1, 50),getNomSymboles(sheader2[j]->sh_name, strTabSection2, 50))){
                j++;
            }
            if(j!=header2.e_shnum){
               // printf("sh_link1=%d\n",sheader1[i]->sh_link);
                sheaderFin[i]=sheader1[i];//Attention PE mauvaise copie!!!!!!!!!!!!!!!!!!!!!!!!!!
                //printf("sh_link2=%d\n",sheaderFin[i]->sh_link);
                int tmpsize =sheader1[i]->sh_size;
                

                if(sheaderFin[i]->sh_type==SHT_SYMTAB){
                    sheaderFin[i]->sh_size= sizeof(Elf32_Sym)*nbSymbole;
                    //decalage+= sizeof(Elf32_Sym)*nbSymbole-tmpsize;
                    ecrireSectionSymbole(dest,symTab, nbSymbole);

                }else if(sheaderFin[i]->sh_type==SHT_STRTAB && i!= header1.e_shstrndx){
                    sheaderFin[i]->sh_size=tailleSTRTAB;
                    //decalage+= tailleSTRTAB-tmpsize;
                    ecrireSectionStringSymbole(dest, strTabSym, tailleSTRTAB);
                }
                else{
                    // printf("????????????? size sec merg 1 : %d size sec merge 2 : %d\n",sheaderFin[i]->sh_size, sheader2[j]->sh_size);
                    //decalage+=sheader2[j]->sh_size;
                    ecrireSectionMergeable(dest, src1, src2, sheader1, sheader2, i, j);
                    sheaderFin[i]->sh_size+=sheader2[j]->sh_size;
                }     
                
                //gestion de l'allign
                if(sheader1[i]->sh_addralign<sheader2[j]->sh_addralign){
                    sheaderFin[i]->sh_addralign=sheader2[j]->sh_addralign;
                }
                
                if(sheaderFin[i]->sh_addralign>1){
                    if (sheaderFin[i]->sh_size % sheaderFin[i]->sh_addralign!=0){
                        sheaderFin[i]->sh_size+= sheaderFin[i]->sh_addralign - (sheaderFin[i]->sh_size % sheaderFin[i]->sh_addralign);
                    }
                }
            }
            else{
                sheaderFin[i]=sheader1[i];
                ecrireSection(dest, src1, sheader1, i);
            }
        }else{
            sheaderFin[i]=sheader1[i];
            ecrireSection(dest, src1, sheader1, i);
        }
        sheaderFin[i]->sh_offset=offset;
        // printf("offset : %d\n", offset);
        offset+=sheaderFin[i]->sh_size;
        
        count++;
        
    }
    //decalage = sheaderFin[header1.e_shnum - 1]->sh_offset + sheaderFin[header1.e_shnum - 1]->sh_size;
    for(int i=0; i<header2.e_shnum;i++){
        j=0;
        while(sheader2[i]->sh_name =!sheaderFin[j]->sh_name && j<header1.e_shnum){
            j++;
        }
        if(j==header1.e_shnum){
            sheaderFin[count]=sheader2[i];
            sheaderFin[count]->sh_offset=offset;
            //decalage+=sheader2[i]->sh_size;
            sheaderFin[count]->sh_name+=sheader1[header1.e_shstrndx]->sh_size;
            if(sheaderFin[count]->sh_type==SHT_REL ){
                sheaderFin[count]->sh_link=header1.e_shstrndx;
            }
            ecrireSection(dest, src2, sheader2, i);
            count++;
            offset+=sheaderFin[i]->sh_size;
        }
    }
    //printTableSection(src1,header1,sheaderFin);
    // printSheader(sheaderFin, count);
    return count;
}

void mergeSymAndStr(char* strTab1,char* strTab2,Elf32_Sym ** symTable1,Elf32_Sym ** symTable2, char * strTabFin, Elf32_Sym ** symTableFin, int t1, int t2, int * compteurSTRTAB, int * count, int* nouveauxIndices){
    (*compteurSTRTAB)=1;
    strTabFin[0]='\0';
    for(int i = 0; i < t1;i++){
        symTableFin[i] = symTable1[i];
        int l= symTableFin[i]->st_name;
        symTableFin[i]->st_name=(*compteurSTRTAB);
        while(strTab1[l]!='\0'){
            strTabFin[(*compteurSTRTAB)]=strTab1[l];
            l++;
            (*compteurSTRTAB)++;
            

        }
        
            strTabFin[(*compteurSTRTAB)]='\0';
            (*compteurSTRTAB)++;
        
        

    }
    
    (*count)=t1;
    int k = 0;
    for(int j = 0;j < t2;j++){
        if(ELF32_ST_BIND(symTable2[j]->st_info)==1){
            while(k<t1 && !memeString(getNomSymboles(symTable2[j]->st_name, strTab2, t2), getNomSymboles(symTableFin[k]->st_name, strTabFin, t1))){
                k++;   
            }
            if(k==t1){
                symTableFin[(*count)]=symTable2[j];
                int l= symTableFin[(*count)]->st_name;
                symTableFin[(*count)]->st_name=(*compteurSTRTAB);
                while(strTab2[l]!='\0'){
                    strTabFin[(*compteurSTRTAB)]=strTab2[l];
                    l++;
                    (*compteurSTRTAB)++;
                }
                strTabFin[(*compteurSTRTAB)]='\0';
                nouveauxIndices[j]=(*count);
                printf("count : %d\n", *count);
                fflush(stdout);
                (*compteurSTRTAB)++;
                (*count)++;
            } else{
                //// Proble Type Fusioné les Fonctions Quand global ???

                if(symTable2[j]->st_shndx != STN_UNDEF && symTableFin[k]->st_shndx != STN_UNDEF){
                    printf("Line 2346 : ERROR : 2 global variables have same name");
                    exit(0);
                } else if(symTable2[j]->st_shndx != STN_UNDEF && symTableFin[k]->st_shndx == STN_UNDEF){
                    int tmp =symTableFin[k]->st_name;
                    symTableFin[k]=symTable2[j];
                    symTableFin[k]->st_name =tmp;
                }
                nouveauxIndices[j]=k;

            }

            k=0;
        }
        else{
            symTableFin[(*count)]=symTable2[j];
            int l= symTableFin[(*count)]->st_name;
            symTableFin[(*count)]->st_name=(*compteurSTRTAB);
            while(strTab2[l]!='\0'){
                strTabFin[(*compteurSTRTAB)]=strTab2[l];
                l++;
                (*compteurSTRTAB)++;
            }
            strTabFin[(*compteurSTRTAB)]='\0';
            nouveauxIndices[j]=(*count);
            (*compteurSTRTAB)++;
            (*count)++;
        }
    }
}

Elf32_Ehdr createElfHeader(Elf32_Shdr ** sheaderFin, Elf32_Ehdr header1, int nombreDeSection){
    Elf32_Ehdr headerFin = header1;
    headerFin.e_shnum = nombreDeSection;
    headerFin.e_shoff = sheaderFin[nombreDeSection-1]->sh_offset + sheaderFin[nombreDeSection-1]->sh_size;
    // printf("OFFSET = %d",sheaderFin[nombreDeSection-1]->sh_offset);
    // printf("SIZE   = %d",sheaderFin[nombreDeSection-1]->sh_size);

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
    for(int i=0; i<52; i++){
        fputc(' ',dest);
    }
    //Recuperer les Headers des 2 fichiers d'entrées
    Elf32_Ehdr * header1 = elf_read_entete(src1);
    Elf32_Ehdr * header2 = elf_read_entete(src2);

    //Recuperer les Sections Headers des 2 fichiers d'entrées
    Elf32_Shdr * sheader1[header1->e_shnum];
    Elf32_Shdr * sheader2[header2->e_shnum];
    lectureTableSectionHeader(src1,*header1,sheader1);
    lectureTableSectionHeader(src2,*header2,sheader2);

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
    int nouveauxIndices[symt2];
    mergeSymAndStr(strTab1, strTab2, symTable1, symTable2,  strTabFin, symTableFin, symt1, symt2,&tailleSTRTAB,&nbSymboles, nouveauxIndices);
    for(int k = 0; k < symt2;k++){
        printf("%d : %d\n", k, nouveauxIndices[k]);
    }
    //Fusion des sections
  
    Elf32_Shdr * sheaderFinTmp[header1->e_shnum + header2->e_shnum];
    
    int nombreDeSection=mergeSheader(sheader1, *header1, sheader2, *header2, sheaderFinTmp,nbSymboles,tailleSTRTAB, symTableFin, strTabFin, dest, src1, src2, strTabSection1, strTabSection2, nouveauxIndices);
    // printf("ca_sort\n");
    fflush(stdout);
    Elf32_Shdr * sheaderFin[nombreDeSection];
    for(int i=0; i<nombreDeSection;i++){
        sheaderFin[i]=sheaderFinTmp[i];
    }
    // printf("pos avant sheader : %ld\n",ftell(dest));
    ecrireSheader(sheaderFin, dest, nombreDeSection);
    fseek(dest,0,SEEK_END);
    // printf("pos fin sheader : %ld\n",sizeof(sheaderFin));
    //Creation du ELFHeader
    Elf32_Ehdr headerFin = createElfHeader(sheaderFin, *header1, nombreDeSection);
    ecrireHeader(&headerFin, dest);
    fseek(dest,0,SEEK_END);
    // printf("pos fin sheader : %ld\n",ftell(dest));
    // printf("taille : %ld",sizeof(Elf32_Shdr));
    

    ////////////
    fclose(src1);
    fclose(src2);
    fclose(dest);
}