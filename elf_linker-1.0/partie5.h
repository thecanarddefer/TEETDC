#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

typedef struct {//structure de la table de relocation
	int nbreloc;//nombre de relocation de la table
	int ind_sect;//indice de la table dans les section
	int dest;//indice de la section visée
	Elf32_Rel * tab[1000];//information de relocation
}  reloc;

void  afficherRelTables(reloc * relTable, Elf32_Ehdr header,Elf32_Sym ** STable,char * strTab);
	//Donnée :table de relocation (relTable), l'entête du fichier (header), la table des symbole (STable), la table des nom de symbole (strTab)
	//Fonction :afficher les relocation d'une table
	//Retour:None

int nombre_reloc (Elf32_Ehdr header,Elf32_Shdr ** sheader);

	//Donnée : l'entête du fichier (header), table des entête de section (sheader)
	//Fonction :compter le nombre de table de relocation
	//Retour:le nombre de table de relocation(compteurRel)

void getRelTable (FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader,Elf32_Sym ** STable,char * strTab,int compteurRel,reloc ** relTable);

	//Donnée : le fichier source (fp),l'entête du fichier (header), le pointeur de la table des section (sheader), le pointeur de la table des symbole (STable),la table des nom de symbole(strTab), le nombre de table de relocation (compteurRel), le pointeur de la table de relocation (relTable)
	//Fonction :lis les table de relocation
	//Retour:none

void printTypeReloc(Elf32_Word r_info);

	//Donnée :les information de relocation (r_info)
	//Fonction :afficher les information de relocation 
	//Retour:None

void info_reloc (Elf32_Ehdr header,Elf32_Shdr ** sheader,reloc ** relTable,FILE * fp);

	//Donnée :l'entête du fichier (header), le pointeur de la table des section (sheader),le pointeur de la table de relocation (relTable), le fichier source (fp)
	//Fonction :lis les information d'une table de relocation
	//Retour:None

void affiche_toute_reloc(FILE * fp,reloc ** relTable, Elf32_Ehdr header, Elf32_Shdr**sheader,Elf32_Sym ** STable,char * strTab,int compteurRel);

	//Donnée : le fichier source (fp),le pointeur de la table de relocation (relTable),l'entête du fichier (header), le pointeur de la table des section (sheader), le pointeur de la table des symbole (STable),la table des nom de symbole(strTab), le nombre de table de relocation (compteurRel), 
	//Fonction :affiche toutes les tables de relocation
	//Retour:none

void freeReloc (reloc ** relTable, int compteurRel);

	//Donnée :le pointeur de la table de relocation (relTable), le nombre de table de relocation (compteurRel)
	//Fonction :libère la table de relocation
	//Retour:None





