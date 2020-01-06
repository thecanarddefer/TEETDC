#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie2.h"


char * createStrTab(Elf32_Shdr** sheader, FILE * fp, Elf32_Half num){
    char * strTab = malloc(sheader[num]->sh_size);// alloue la table des noms de section
    fseek(fp,sheader[num]->sh_offset,SEEK_SET);
    fread(strTab, sheader[num]->sh_size/sizeof(char), sizeof(char), fp);// lecture de la table
    return strTab;
}

void printTableSection(FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader){
    printf("Num\t");// affichage des colonnes de la table des sections
    printf("Nom\t\t\t");
    printf("Type\t\t\t");
    printf("@virt\t");
    printf("Off\t");
    printf("Tail\t");
    printf("Link\t");
    printf("Info\t");
    printf("Align\t");
    printf("TailTab\t");
    printf("Flag\n");
    char* strTabSection = createStrTab(sheader, fp, header.e_shstrndx);// recuperation de la table des noms de section
    for(int i=0;i<header.e_shnum;i++){// boucle sur chaque section
        printf("%d \t",i+1);
        int j = sheader[i]->sh_name;
        while(strTabSection[j] != '\0'){ // affiche le nom de la section
          printf("%c", strTabSection[j]);
          j++;
        }
        while(j<sheader[i]->sh_name+20){ // maintient l'indentation
          printf(" ");
          j++;
        }
        printf("\t");//affichage des données de chaque colonne
        printSh_Type(sheader[i]->sh_type);
        printf("%d \t",sheader[i]->sh_addr);
        printf("%X \t",sheader[i]->sh_offset);
        printf("%d \t",sheader[i]->sh_size);
        printf("%d \t",sheader[i]->sh_link);
        printf("%d \t",sheader[i]->sh_info);
        printf("%d \t",sheader[i]->sh_addralign);
        printf("%d \t",sheader[i]->sh_entsize);
        printFlagz(sheader[i]->sh_flags);
        
	      printf("\n");
    }
	printKeyFlags();
	free(strTabSection); // libere la table des noms de section
}

void printKeyFlags(){
	printf("Key to flag :\n");// affiche la définition des abréviations de flag
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
  	printf("  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
  	printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
  	printf("  y (noread), p (processor specific)\n");
}

void lectureTableSectionHeader(FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader){
    fseek(fp,header.e_shoff,SEEK_SET);
    for(int i=0;i<header.e_shnum;i++){//boucle sur chaque section
        sheader[i]=(Elf32_Shdr*)malloc(sizeof(Elf32_Shdr));// alloue l'espace pour l'entete de section
        fread(sheader[i],1, sizeof(Elf32_Shdr), fp);// lit l'entete de section
    }
	printTableSection(fp,header,sheader);
    return ;
}
void freeSHDR(Elf32_Shdr ** sheader){
    for (int i=0;i<sizeof(*sheader)/sizeof(Elf32_Shdr);i++){//boucle sur le tableau d'espace allouer
        free(sheader[i]); // libere l'entete de section
    }
}

void printSh_Type(Elf32_Word leMot){
    switch(leMot){ // affichage du type de section
        case  SHT_NULL :         printf("NULL                    ");break; //Section header table entry unused
        case  SHT_PROGBITS :     printf("PROGBITS                ");break;//Program data
        case  SHT_SYMTAB :       printf("SYMTAB                  ");break;//Symbol table 
        case  SHT_STRTAB :       printf("STRTAB                  ");break;//String table
        case  SHT_RELA :         printf("RELA                    ");break;//Relocation entries with addends
        case  SHT_HASH :         printf("HASH                    ");break;//Symbol hash table
        case  SHT_DYNAMIC :      printf("DYNAMIC                 ");break;//Dynamic linking information 
        case  SHT_NOTE :         printf("NOTE                    ");break;//Notes 
        case  SHT_NOBITS :       printf("NOBITS                  ");break;//Program space with no data (bss)
        case  SHT_REL :          printf("REL                     ");break;//Relocation entries, no addends
        case  SHT_SHLIB:         printf("SHLIB                   ");break;//Reserved 
        case  SHT_DYNSYM:        printf("DYNSYM                  ");break;//Dynamic linker symbol table 
        case  SHT_INIT_ARRAY:    printf("INIT_ARRAY              ");break;//Array of constructors
        case  SHT_FINI_ARRAY:    printf("FINI_ARRAY              ");break;//Array of destructors
        case  SHT_PREINIT_ARRAY: printf("PREINIT_ARRAY           ");break;//Array of pre-constructors
        case  SHT_GROUP:         printf("GROUP                   ");break;//Section group
        case  SHT_SYMTAB_SHNDX:  printf("SYMTAB_SHNDX            ");break;//Extended section indeces 
        case  SHT_NUM:           printf("NUM                     ");break;//Number of defined types
        default :                printf("Non traité : %X   ",leMot);break;//Non traité
        printf("\t");
    }
}

void printFlagz(Elf32_Word leMot){
	if (leMot & SHF_WRITE){ // affichage des flags
		printf("W");}
	if (leMot & SHF_ALLOC) {
		printf("A");}
	if (leMot & SHF_EXECINSTR) {
		printf("X");}
	if (leMot & SHF_MERGE) {
		printf("M");}
	if (leMot & SHF_STRINGS){
		printf("S");}
	if (leMot & SHF_INFO_LINK) {
		printf("I");}
	if (leMot & SHF_LINK_ORDER) {
		printf("L");}
	if (leMot & SHF_OS_NONCONFORMING) {
		printf("O");}
	if (leMot & SHF_GROUP){
		printf("G");}
	if (leMot & SHF_TLS) {
		printf("T");}
	if (leMot & SHF_COMPRESSED) {
		printf("C");}
	if (leMot & SHF_MASKOS) {
		printf("o");}
	if (leMot & SHF_MASKPROC) {
		printf("p");}
	if (leMot & SHF_ORDERED) {
		printf("y");}
	if (leMot & SHF_EXCLUDE) {
		printf("E");}
	printf("\t");
    
}
