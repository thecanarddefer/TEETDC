#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"
#include "partie4.h"
#include "partie5.h"

int isMergeable(Elf32_Word type);
    //Donnée : un type de section "type"
	//Fonction : savoir si une section est d'un type qu'il faut fusionner
	//Retour: 1 ou 0
    

//////////// Fonctions d'écriture dans le nouveau fichier

void ecrireSheader(Elf32_Shdr ** sheader, FILE * dest, int nombreDeSection);
  

void ecrireHeader(Elf32_Ehdr * header1, FILE * dest);
   

void ecrireSectionSymbole(FILE * dest, Elf32_Sym ** symTab, int nbSymbole);
    

void ecrireSectionStringSymbole(FILE * dest, char * strTabSym, int tailleSTRTAB);
    
void ecrireSection(FILE * dest, FILE * src, Elf32_Shdr ** sheader, int indice);
   

void ecrireSectionMergeable(FILE * dest, FILE * src1, FILE * src2, Elf32_Shdr ** sheader1, Elf32_Shdr ** sheader2, int indice1, int indice2);
    
////////////////////////////////////////////////////////////

void printSheader(Elf32_Shdr ** sheaderFin, int count);
    

char * getNomSymboles(Elf32_Word name,char* strTab, int taille);
    //Donnée : un nom de symbole ou de section sous forme d'indice (name), une table de chaine de caractère (strTab), taille de la table de caractère
	//Fonction : obtenir la chaîne de caractères à partir de l'indice dans la table de chaîne
	//Retour: une chaîne
  
int memeString(char* s1, char * s2);
    //Donnée : 2 chaînes de caractères s1 et s2
	//Fonction : savoir si les 2 chaînes sont identiques
	//Retour: 1 ou 0
  
void mergeRelocationTable(Elf32_Shdr ** sheader1,Elf32_Shdr ** sheader2, int indice1, int indice2,char* strTabSection1, char* strTabSection2, FILE * src1, FILE * dest, reloc **relTable, int* nouveauxIndices,  int compteurRel, int nbSection1, int * indicesFich1, reloc **relTable1, int compteurRel1);
    //Donnée : 2 chaînes de caractères s1 et s2
	//Fonction : savoir si les 2 chaînes sont identiques
	//Retour: 1 ou 0

int mergeSheader(Elf32_Shdr ** sheader1, Elf32_Ehdr header1, Elf32_Shdr ** sheader2, Elf32_Ehdr header2, Elf32_Shdr ** sheaderFin, int nbSymbole, int tailleSTRTAB, Elf32_Sym ** symTab, char * strTabSym, FILE * dest, FILE * src1, FILE * src2, char* strTabSection1, char* strTabSection2, int* nouveauxIndices, reloc **relTable,  int compteurRel, int * indicesFich1, reloc **relTable1,  int compteurRel1, int lastLocalSym);
  
int mergeSymAndStr(char* strTab1,char* strTab2,Elf32_Sym ** symTable1,Elf32_Sym ** symTable2, char * strTabFin, Elf32_Sym ** symTableFin, int t1, int t2, int * compteurSTRTAB, int * count, int* nouveauxIndices, int * indicesFich1);

Elf32_Ehdr createElfHeader(Elf32_Shdr ** sheaderFin, Elf32_Ehdr header1, int nombreDeSection);
    //Donnée : le header du fichier 1, la table des section du fichier fusionné sheaderFin et le nombre de sections final
	//Fonction : creer le Header du fichier final
	//Retour: un header au format Elf32_Ehdr
