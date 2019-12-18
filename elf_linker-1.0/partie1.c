#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie1.h"


void printTypeELF(Elf32_Half e_type ){
  	printf("Type de fichier ELF : ");
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


void printE_Machine(Elf32_Half e_machine){
  	printf("Machine: ");
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

void printE_Ident(unsigned char * e_ident){
	switch(e_ident[EI_CLASS]){
		case ELFCLASS32:printf("Classe : 32bits \n");
			break;
		case ELFCLASS64:printf("Classe : 64bits \n");
			break;
		default: printf("Classe : Invalide\n");
	}
	switch(e_ident[EI_DATA]){
		case ELFDATA2LSB:printf("Données : compléments à 2, little endian \n");
			break;
		case ELFDATA2MSB:printf("Données : compléments à 2, big endian \n");
			break;
		default: printf("Données : Invalide\n");
	}
}

void printE_Magique(unsigned char * e_ident){
	printf("Magique : %x ",e_ident[EI_MAG0]);
  	printf("%x ", e_ident[EI_MAG1]);
  	printf("%x ", e_ident[EI_MAG2]);
  	printf("%x \n", e_ident[EI_MAG3]);
}

void printE_Version(Elf32_Word e_version){
	if(e_version == EV_CURRENT){
    	printf("Current version : %d \n",e_version);
  	}else if(e_version == EV_NONE){
    	printf("Invalid ELF version : %d \n",e_version);
  	}
}

void printE_Entry(Elf32_Addr e_entry){
  	printf("Adresse du point d'entrée : 0x%x\n", e_entry);
}

void printE_Flags(Elf32_Word e_flags){
	printf("Flags : 0x%x\n", e_flags);
}

void printE_Ehsize(Elf32_Half e_ehsize){
  	printf("Taille de l'entête : %d octets\n",e_ehsize);
}

void printE_Phoff(Elf32_Off e_phoff){
  	if(e_phoff == 0){
    	printf("Le fichier n'a pas de program header table\n" );
  	}else{
    	printf("Debut des entetes de programme : %d\n",e_phoff);
  	}
}

void printE_Shoff(Elf32_Ehdr * header){
	if(header->e_shoff == 0){
    	printf("Le fichier n'a pas de section header table\n" );
  	}else{
    	printf("Debut des entetes de sections: %d\n",header->e_shoff);
    	printf("Taille de la table des sections : %d octets\n",header->e_shnum * header->e_shentsize);
    	printf("Nombre d'entrées : %d\n",header->e_shnum);
    	printf("Taille de l'entête de sections : %d octets\n",header->e_shentsize);
  }
}

void printIndexTableNomSection(Elf32_Half e_shstrndx){
  	if(e_shstrndx == SHN_UNDEF){
	 	printf("Pas de table de nom des sections");}
  	else {
		printf("Index d'entrée de la table des nom de sections : %d\n",e_shstrndx);}
}

void afficherheaderELF(Elf32_Ehdr * header){
	//Magique
	printE_Magique(header->e_ident);
  	//e_ident
  	printE_Ident(header->e_ident);
	//e_type
	printTypeELF(header->e_type );
  	//e_machine
  	printE_Machine(header->e_machine);
  	//e_version
  	printE_Version(header->e_version);
  	//e_entry 
	printE_Entry(header->e_entry);
  	//e_flags
	printE_Flags(header->e_flags);
  	//e_ehsize
	printE_Ehsize(header->e_ehsize);
  	//e_phoff
	printE_Phoff(header->e_phoff);
  	//e_shoff
	printE_Shoff(header);
  	//table de nom des sections
	printIndexTableNomSection(header->e_shstrndx);
}

Elf32_Ehdr * elf_read_entete(FILE * src){
  	Elf32_Ehdr * header = malloc(sizeof(Elf32_Ehdr));
  	fread(header,1, sizeof(Elf32_Ehdr), src);
  	afficherheaderELF(header);
  	return header;
}

void freeHeader(Elf32_Ehdr * header){
	free(header);
}

