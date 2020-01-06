#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie1.h"


void printTypeELF(Elf32_Half e_type ){ // switch pour trouver le type de fichier
  	printf("Type de fichier ELF : 				 ");
	switch(e_type){
    	case ET_NONE: printf("No file type\n");
      		break;
    	case ET_REL: printf("Relocatable file\n");
      		break;
    	case ET_EXEC: printf("Executable file\n");
      		break;
    	case ET_DYN: printf("Shared object file\n");
      		break;
    	case ET_CORE: printf("Corefile\n");
      		break;
    	default: printf("Cas non traite\n");
  }
}


void printE_Machine(Elf32_Half e_machine){ // switch pour trouver le type de machine
  	printf("Machine: 					 ");
	switch(e_machine){
    	case EM_ARM: printf("ARM\n");
      		break;
    	case EM_MMA: printf("Fujitsu MMA Multimedia Accelerator\n");
      		break;
    	case EM_ST9PLUS: printf("STMicroelectronics ST9+ 8/16 mc\n");
      		break;
    	case EM_X86_64: printf("AMD x86-64 architecture\n");
      		break;
    	default: printf("Cas non traite\n");
  }
}

void printE_Ident(unsigned char * e_ident){ // switch pour trouver la classe du fichier 
	printf("Classe : 					 ");
	switch(e_ident[EI_CLASS]){
		case ELFCLASS32:printf("ELF32 \n");
			break;
		case ELFCLASS64:printf("ELF64 \n");
			break;
		default: printf("Invalide\n");
	}
	printf("Données : 					 ");
	switch(e_ident[EI_DATA]){ // switch pour trouver le type d'écriture des données 
		case ELFDATA2LSB:printf("compléments à 2, little endian \n");
			break;
		case ELFDATA2MSB:printf("compléments à 2, big endian \n");
			break;
		default: printf("Invalide\n");
	}
}

void printE_Magique(unsigned char * e_ident){
	printf("Magique : 	 ");
	for (int i=0 ;i<EI_NIDENT; i++){ // boucle affichant l'intégralité de l'e_ident
  		printf("%02x ", e_ident[i]);
  	}
  	printf("\n");
}

void printE_Version(Elf32_Word e_version){
	printf("Version : 					 ");
	if(e_version == EV_CURRENT){ // affiche la version
    	printf("%d (current)\n",e_version);
  	}else if(e_version == EV_NONE){
    	printf("%d Invalid ELF\n",e_version);
  	}
}

void printE_Entry(Elf32_Addr e_entry){
	printf("Adresse du point d'entrée : 			 ");
  	printf("0x%x\n", e_entry); // affiche l'adresse de l'entrée
}

void printE_Flags(Elf32_Word e_flags){
	printf("Flags : 					 ");
	printf("0x%x\n", e_flags); // affiche les flags
}

void printE_Ehsize(Elf32_Half e_ehsize){
  	printf("Taille de l'entête : 				 ");
  	printf("%d octets\n",e_ehsize);// affiche la taille de l'entête
}

void printE_Phoff(Elf32_Off e_phoff){
    	printf("Debut des entetes de programme : 		 ");
    	printf("%d (octets dans fichier)\n",e_phoff); // affiche le début des entétes de programme
  	
}

void printE_Phensize(Elf32_Half e_phentsize){
	printf("Taille de la table des programme : 		 ");
	printf("%d octets\n",e_phentsize);// affiche la taille de la table des programmes
}  	

void printE_Phnum(Elf32_Half e_phnum){
	printf("Nombre d'entetes de programme : 		 ");
	printf("%d\n",e_phnum); // affiche le nombre d'entete des programmes
}

void printE_Shoff(Elf32_Off e_shoff){
    	printf("Debut des entetes de section: 			 ");
    	printf("%d (octets dans fichier)\n",e_shoff);// affiche le début de l'entête de section
}

void printE_Shensize(Elf32_Half e_shentsize){
	printf("Taille de la table des sections : 		 ");
	printf("%d octets\n",e_shentsize); // affiche la taille de la table des section
}  	

void printE_Shnum(Elf32_Half e_shnum){
	printf("Nombre d'entetes de section : 			 ");
	printf("%d\n",e_shnum); // affiche le nombre des entête de section
}

void printIndexTableNomSection(Elf32_Half e_shstrndx){ // affiche l'index de l'entête des nom de section
	printf("Index d'entrée de la table des nom de sections : ");
  	if(e_shstrndx == SHN_UNDEF){
	 	printf("Pas de table de nom sections");}
  	else {
		printf("%d\n",e_shstrndx + 1);}
}

void afficherheaderELF(Elf32_Ehdr * header){ //affiche le header ELF
	//Magique
	printE_Magique(header->e_ident);
  	//e_ident
  	printE_Ident(header->e_ident);
  	//e_version
  	printE_Version(header->e_version);
	//e_type
	printTypeELF(header->e_type );
  	//e_machine
  	printE_Machine(header->e_machine);
  	//e_entry 
	printE_Entry(header->e_entry);
  	//e_phoff
	printE_Phoff(header->e_phoff);
  	//e_shoff
	printE_Shoff(header->e_shoff);
  	//e_flags
	printE_Flags(header->e_flags);
  	//e_ehsize
	printE_Ehsize(header->e_ehsize);
	//e_phentsize
	printE_Phensize(header->e_phentsize);
	//e_phnum
	printE_Phnum(header->e_phnum);
	//e_shentsize
	printE_Shensize(header->e_shentsize);
	//e_shnum
	printE_Shnum(header->e_shnum);
  	//table de nom des sections
	printIndexTableNomSection(header->e_shstrndx);
}

Elf32_Ehdr * elf_read_entete(FILE * src){
  	Elf32_Ehdr * header = malloc(sizeof(Elf32_Ehdr));// allocation du header
  	fread(header,1, sizeof(Elf32_Ehdr), src); // lecture du header
  	afficherheaderELF(header);
  	return header;
}

void freeHeader(Elf32_Ehdr * header){
	free(header); // libère le header
}

