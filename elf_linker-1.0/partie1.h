#include <stdio.h>
#include <elf.h>
#include <stdlib.h>


void printTypeELF(Elf32_Half e_type );
	//Donnée :le type de fichier sous forme binaire (Elf32_Half)
	//Fonction :permet de print le type du fichier ELF
	//Retour:None

void printE_Machine(Elf32_Half e_machine);
	//Donnée :le type de machine sous forme binaire (Elf32_Half)
	//Fonction :permet de print le type d'architecture du fichier
	//Retour:None

void printE_Ident(unsigned char * e_ident);
	//Donnée :le e_ident sous forme de tableau binaire
	//Fonction :permet de print le format de fichier
	//Retour:None


void printE_Magique(unsigned char * e_ident);
	//Donnée :le e_ident sous forme de tableau binaire
	//Fonction :permet de print si c'est un ELF
	//Retour:None


void printE_Version(Elf32_Word e_version);
	//Donnée :la version sous forme binaire (Elf32_Word)
	//Fonction :permet de print la version
	//Retour:None
	

void printE_Entry(Elf32_Addr e_entry);
	//Donnée :le point d'entrée sous forme binaire (Elf32_Addr)
	//Fonction :permet de print l'adresse du point d'entree
	//Retour:None


void printE_Flags(Elf32_Word e_flags);
	//Donnée :les flags sous forme binaire (Elf32_Word)
	//Fonction :permet de print les flags
	//Retour:None
	

void printE_Ehsize(Elf32_Half e_ehsize);
	//Donnée :la taille de l'entête sous forme binaire (Elf32_Half)
	//Fonction :permet de print la taille de l'entete
	//Retour:None
	

void printE_Phoff(Elf32_Off e_phoff);
	//Donnée :le debut de la table de l'entête des programmes sous forme binaire (Elf32_Off)
	//Fonction : permet de print le debut de la table des entetes de programme 
	//Retour:None
	

void printE_Phnum(Elf32_Half e_phnum);
	//Donnée :le nombre d'entetes des programmes sous forme binaire (Elf32_Half)
	//Fonction : permet de print le nombre d'entete de programme
	//Retour:None
	

void printE_Phensize(Elf32_Half e_phentsize);
	//Donnée :la taille de la table des entetes des programmes sous forme binaire (Elf32_Half)
	//Fonction : permet de print la taille de la table des entete de programme
	//Retour:None
	

void printE_Shoff(Elf32_Off e_shoff);
	//Donnée :le debut de la table des entetes de section sous forme binaire (Elf32_Off)
	//Fonction : permet de print le debut de la table des entetes de sections, la Taille de la table des sections,
		  // le nombre d'entrées et la taille de l'entête de sections 

	//Retour:None

	
void printIndexTableNomSection(Elf32_Half e_shstrndx);
	//Donnée :l'entrée de la table des noms de section sous forme binaire (Elf32_Half)
	//Fonction :permet de print l'index d'entrée de la table des nom de sections
	//Retour:None



void printE_Shnum(Elf32_Half e_shnum);
	//Donnée :le nombre des entetes de section sous forme binaire (Elf32_Half)
	//Fonction : permet de print le nombre d'entete de section
	//Retour:None


void printE_Shensize(Elf32_Half e_shentsize);
	//Donnée :la taille de la table des entetes de section sous forme binaire (Elf32_Half)
	//Fonction : permet de print la taille de la table des entete de section
	//Retour:None

	

void afficherheaderELF(Elf32_Ehdr * header);
	//Donnée :le pointeur header (Elf32_Ehdr)
	//Fonction :permet d'afficher le header ELF
	//Retour:None



Elf32_Ehdr * elf_read_entete(FILE * src);
	//Donnée :le pointeur du fichier source (FILE)
	//Fonction :permet de lire le header ELF du ficher src
	//Retour: le header
	

void freeHeader(Elf32_Ehdr * header);
	//Donnée :le pointeur du header (Elf32_Ehdr)
	//Fonction :libere l'espace de header
	//Retour: None







	
