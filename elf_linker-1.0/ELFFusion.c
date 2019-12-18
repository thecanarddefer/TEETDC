#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

int mergeSheader(Elf32_Shdr ** sheader1, Elf32_Ehdr header1, Elf32_Shdr ** sheader2, Elf32_Ehdr header2, Elf32_Shdr ** sheaderFin){
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
                sheaderFin[i]->sh_size+=sheader2[j]->sh_size;
                sheaderFin[i]->sh_offset+=decalage;
                decalage+=sheader2[j]->sh_size;
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
            j++
        }
        if(j==header1.e_shnum){
            sheaderFin[i+header1.e_shnum]=sheader2[i];
            sheaderFin[i+header1.e_shnum]->sh_offset=decalage;
            decalage+=sheader2[i]->sh_size;
            sheaderFin[i+header1.e_shnum]->sh_name+=sheader1[header1.e_shstrndx]->sh_size;
            count++;
        }
    }

    return count;
}
Elf32_Ehdr createElfHeader(Elf32_Shdr ** sheaderFin, Elf32_Ehdr header1, int nombreDeSection){
    Elf32_Ehdr headerFin = header1;
    headerFin.e_shnum = nombreDeSection;
    headerFIn.e_shoff = sheaderFin[nombreDeSection]->sh_offset + sheaderFin[nombreDeSection]->sh_size;
    return headerFin;
}
bool isMergeable(Elf32_Word type){
    switch(type){
        case(SHT_NULL): return true;
        case(SHT_PROGBITS): return true;
        case(SHT_SYMTAB): return true;
        case(SHT_STRTAB): return true;
    }
    return false;
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
    char* strTabSection1 = malloc(sheader1[header1.e_shstrndx]->sh_size);
    strTabSection = createStrTab(sheader1, fp, header1.e_shstrndx);
    char* strTabSection2 = malloc(sheader2[header2.e_shstrndx]->sh_size);
    strTabSection = createStrTab(sheader2, fp, header2.e_shstrndx);

    //Recuperer la table Symbole et String table des symboles 2 fichiers d'entrées
    //fichier 1
    int symTabNum=findSymTab(sheader,header1->e_shnum);
    if(symTabNum < header1->e_shnum){
        int numSec = findStrTabSym(sheader1,*header1);
        char* strTab1 = malloc(sheader1[numSec]->sh_size);
        strTab1 = createStrTab(sheader1, src1, numSec);
        Elf32_Sym * symTable1[sheader1[symTabNum]->sh_size/sizeof(Elf32_Sym)];
        readSymTab(src1, sheader1, symTabNum, symTable1, strTab1);
    }else{
        printf("il n'y a pas de table des symboles \n");
    }
    //fichier 2
    symTabNum=findSymTab(sheader,header2->e_shnum);
    if(symTabNum < header2->e_shnum){
        int numSec2 = findStrTabSym(sheader2,*header2);
        char* strTab2 = malloc(sheader2[numSec2]->sh_size);
        strTab2 = createStrTab(sheader2, src2, numSec2);
        Elf32_Sym * symTable2[sheader2[symTabNum]->sh_size/sizeof(Elf32_Sym)];
        readSymTab(src2, sheader2, symTabNum, symTable2, strTab2);
    }else{
        printf("il n'y a pas de table des symboles \n");
    }

    //Fusion des sections
    Elf32_Shdr * sheaderFinTmp[header1->e_shnum + header2->e_shnum];
    int nombreDeSection=mergeSheader(sheader1, *header1, sheader2, *header2, sheaderFinTmp);
    Elf32_Shdr * sheaderFin[nombreDeSection];
    for(int i=0; i<nombreDeSection;i++){
        sheaderFin[i]=sheaderFinTmp[i];
    }
    //Creation du ELFHeader
    createElfHeader(sheaderFin, *header1, nombreDeSection);
}