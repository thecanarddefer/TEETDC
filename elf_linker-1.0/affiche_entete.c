#include <stdlib.h>
#include <stdio.h>
#include <elf.h>

Elf32_Ehdr * elf_read_entete(FILE * src){
  Elf32_Ehdr * header = malloc(sizeof(Elf32_Ehdr));
  fread(header,1, sizeof(Elf32_Ehdr), src);
  //Magique
  printf("Magique : %x ",header->e_ident[EI_MAG0]);
  printf("%x ", header->e_ident[EI_MAG1]);
  printf("%x ", header->e_ident[EI_MAG2]);
  printf("%x \n", header->e_ident[EI_MAG3]);

  //e_ident
  if(header->e_ident[EI_CLASS] == 0) printf("Classe : Invalide\n");
  else if (header->e_ident[EI_CLASS] == 1) printf("Classe : 32bits \n");
  else if (header->e_ident[EI_CLASS] == 2) printf("Classe : 64bits \n");

  if(header->e_ident[EI_CLASS] == 0) printf("Données : Invalide\n");
  else if (header->e_ident[EI_DATA] == 1) printf("Données : compléments à 2, little endian \n");
  else if (header->e_ident[EI_CLASS] == 2) printf("Données : compléments à 2, big endian \n");

  //e_type
  printf("Type de fichier ELF : ");
  switch(header->e_type){
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
    default: printf("Cas non traite");
  }
  //e_machine
  printf("Machine: ");
  switch(header->e_machine){
    case EM_ARM: printf("ARM");
      break;
    case EM_MMA: printf("Fujitsu MMA Multimedia Accelerator");
      break;
    case EM_ST9PLUS: printf("STMicroelectronics ST9+ 8/16 mc");
      break;
    case EM_X86_64: printf("AMD x86-64 architecture");
      break;
    default: printf("Cas non traite");
  }
  printf("\n");

  //e_version
  if(header->e_version == EV_CURRENT){
    printf("Current version : %d \n",header->e_version);
  }else if(header->e_version == EV_NONE){
    printf("Invalid ELF version : %d \n",header->e_version);
  }
  //e_entry a priori balek
  printf("Adresse du point d'entrée : 0x%lx\n", header->e_entry);
  //e_flags
  printf("Flags : 0x%x\n", header->e_flags);
  //e_ehsize
  printf("Taille de l'entête : %d octets\n", header->e_ehsize);
  //e_phoff
  if(header->e_phoff == 0){
    printf("Le fichier n'a pas de program header table\n" );
  }else{
    printf("Debut des entetes de programme : %ld\n",header->e_phoff);
  }
  //e_shoff
  if(header->e_shoff == 0){
    printf("Le fichier n'a pas de section header table\n" );
  }else{
    printf("Debut des entetes de sections: %ld\n",header->e_shoff);
    printf("Taille de la table des sections : %d octets\n",header->e_shnum * header->e_shentsize);
    printf("Nombre d'entrées : %d\n",header->e_shnum);
    printf("Taille de l'entête de sections : %d octets\n",header->e_shentsize);
  }
  //table de nom des sections
  if(header->e_shstrndx == SHN_UNDEF) printf("Pas de table de nom des sections");
  else printf("Index d'entrée de la table des nom de sections : %d\n", header->e_shstrndx);
  return header;
}

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("Il faut 1 arguments");
    exit(0);
  }
  FILE * source = fopen(argv[1], "r");
  if(source){
    elf_read_entete(source);
  } else {
    printf("bad file");
  }
}
