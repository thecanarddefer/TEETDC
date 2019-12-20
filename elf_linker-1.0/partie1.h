#include <stdio.h>
#include <elf.h>
#include <stdlib.h>


void printTypeELF(Elf32_Half e_type );
	//permet de print le type du fichier ELF


void printE_Machine(Elf32_Half e_machine);
	//permet de print le type d'architecture du fichier

void printE_Ident(unsigned char * e_ident);
	//permet de print le format de fichier

void printE_Magique(unsigned char * e_ident);
	//permet de print si c'est un ELF

void printE_Version(Elf32_Word e_version);
	//permet de print la version

void printE_Entry(Elf32_Addr e_entry);
	//permet de print l'adresse du point d'entree

void printE_Flags(Elf32_Word e_flags);
	//permet de print les flags

void printE_Ehsize(Elf32_Half e_ehsize);
	//permet de print la taille de l'entete

void printE_Phoff(Elf32_Off e_phoff);
	//permet de print le debut des entetes de programme 

void printE_Phnum(Elf32_Half e_phnum);
	//permet de print le nombre d'entete de programme

void printE_Phensize(Elf32_Half e_phentsize);
	//permet de print la taille des entete de programme

void printE_Shoff(Elf32_Off e_shoff);
	//permet de print le debut des entetes de sections, la Taille de la table des sections,
	// le nombre d'entrées et la taille de l'entête de sections 

void printIndexTableNomSection(Elf32_Half e_shstrndx);
	//permet de print l'index d'entrée de la table des nom de sections

void printE_Shnum(Elf32_Half e_shnum);
	//permet de print le nombre d'entete de section

void printE_Shensize(Elf32_Half e_shentsize);
	//permet de print la taille des entete de section

void afficherheaderELF(Elf32_Ehdr * header);
	//permet d'afficher le header ELF

Elf32_Ehdr * elf_read_entete(FILE * src);
	// permet de lire le header ELF du ficher src

void freeHeader(Elf32_Ehdr * header);
	// libere l'espace de header
