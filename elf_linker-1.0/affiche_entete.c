#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
// typedef struct {
//   unsigned char  e_ident[EI_NIDENT];
//   Elf32_Half e_type;
//   Elf32_Half e_machine;
//   Elf32_Word e_version;
//   Elf32_Addr e_entry;
//   Elf32_Off e_phoff;
//   Elf32_Off e_shoff;
//   Elf32_Word e_flags;
//   Elf32_Half e_ehsize;
//   Elf32_Half e_phentsize;
//   Elf32_Half e_phnum;
//   Elf32_Half e_shentsize;
//   Elf32_Half e_shnum;
//   Elf32_Half e_shstrndx;
// } Elf32_Ehdr;

void elf_read_entete(FILE * src){
  Elf32_Ehdr header;
  fread(&header,1, sizeof(Elf32_Ehdr), src);
  printf("%c \n", header.e_ident[1]);
  printf("%c\n", header.e_ident[2]);
  printf("%c\n", header.e_ident[3]);
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
