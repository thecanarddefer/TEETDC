#include <stdio.h>
#include <elf.h>
#include <stdlib.h>


// if latabledessections est vide : e_shnum = 0
// sinon : 
// e_shentsize * e_shnum = Taille de la table des sections 
// e_shoff : pour pouvoir se positionner ou bon nous semble 
void printSh_Type(Elf32_Word leMot){
    switch(leMot){
        case  0 :   printf("type de la section: Section header table entry unused \n");break;
        case  1 :   printf("type de la section: Program data\n");break;
        case  2 :   printf("type de la section: Symbol table\n");break;
        case  3 :   printf("type de la section: String table\n");break;
        case  4 :   printf("type de la section: Relocation entries with addends \n");break;
        case  5 :   printf("type de la section: Symbol hash table \n");break;
        case  6 :   printf("type de la section: Dynamic linking information \n");break;
        case  7 :   printf("type de la section: Notes \n");break;
        case  8 :   printf("type de la section: Program space with no data (bss) \n");break;
        case  9 :   printf("type de la section: Relocation entries, no addends \n");break;
        case  10:   printf("type de la section: Reserved \n");break;
        case  11:   printf("type de la section: Dynamic linker symbol table \n");break;
        case  14:   printf("type de la section: Array of constructors \n");break;
        case  15:   printf("type de la section: Array of destructors \n");break;
        case  16:   printf("type de la section: Array of pre-constructors \n");break;
        case  17:   printf("type de la section: Section group \n");break;
        case  18:   printf("type de la section: Extended section indeces \n");break;
        case  19:   printf("type de la section: Number of defined types \n");break;
        default :   printf("(non Traité)type de la Section: %d",leMot);break;
    }
}

void printFlagz(Elf32_Word leMot){
    switch(leMot){
        case (1 << 0)   :   printf("Writable");break;
        case (1 << 1)   :   printf("Occupies memory during execution");break;
        case (1 << 2)   :   printf("Executable");break;
        case (1 << 4)   :   printf("Might be merged");break;
        case (1 << 5)   :   printf("Contains nul-terminated strings");break;
        case (1 << 6)   :   printf("`sh_info' contains SHT index");break;
        case (1 << 7)   :   printf("Preserve order after combining");break;
        case (1 << 8)   :   printf("Non-standard OS specific handling required");break;
        case (1 << 9)   :   printf("Section is member of a group");break;
        case (1 << 10)  :   printf("Section hold thread-local data.");break;
        case (1 << 11)  :   printf("Section with compressed data.");break;
        case 0x0ff00000 :   printf("OS-specific.");break;
        case 0xf0000000 :   printf("Processor-specific");break;
        case (1 << 30)  :   printf("Special ordering requirement(Solaris)"); break;
        case (1U << 31) :   printf("Section is excluded unless referenced or allocated (Solaris)");break;
    }
}

void afficherTableSectionHeader(FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader){
    fseek(fp,header.e_shoff,SEEK_SET);
    for(int i=0;i<header.e_shnum;i++){
        sheader[i]=(Elf32_Shdr*)malloc(sizeof(Elf32_Shdr));
        fread(sheader[i],1, sizeof(Elf32_Shdr), fp);
    }
    for(int i=0;i<header.e_shnum;i++){
        printf("numero de la section: %d \n",i+1);
        printf("nom de la section: %d \n",sheader[i]->sh_name);
        printSh_Type(sheader[i]->sh_type);
        printFlagz(sheader[i]->sh_flags);
        printf("@ virtuel pendant exec de la section: %d \n",sheader[i]->sh_addr);
        printf("offset de la section: %d \n",sheader[i]->sh_offset);
        printf("taille de la section: %d \n",sheader[i]->sh_size);
        printf("link vers autre section: %d \n",sheader[i]->sh_link);
        printf("informations de la section: %d \n",sheader[i]->sh_info);
        printf("allignement de l'@ de la section: %d \n",sheader[i]->sh_addralign);
        printf("taille de la section si elle a une table: %d \n",sheader[i]->sh_entsize);
	printf("\n");
    }
    return ;
}
void freeSHDR(Elf32_Shdr * sheader){
    for (int i=0;i<sizeof(sheader)/sizeof(Elf32_Shdr);i++){
        free(&sheader[i]);
    }
}

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
  printf("Adresse du point d'entrée : 0x%x\n", header->e_entry);
  //e_flags
  printf("Flags : 0x%x\n", header->e_flags);
  //e_ehsize
  printf("Taille de l'entête : %d octets\n", header->e_ehsize);
  //e_phoff
  if(header->e_phoff == 0){
    printf("Le fichier n'a pas de program header table\n" );
  }else{
    printf("Debut des entetes de programme : %d\n",header->e_phoff);
  }
  //e_shoff
  if(header->e_shoff == 0){
    printf("Le fichier n'a pas de section header table\n" );
  }else{
    printf("Debut des entetes de sections: %d\n",header->e_shoff);
    printf("Taille de la table des sections : %d octets\n",header->e_shnum * header->e_shentsize);
    printf("Nombre d'entrées : %d\n",header->e_shnum);
    printf("Taille de l'entête de sections : %d octets\n",header->e_shentsize);
  }
  //table de nom des sections
  if(header->e_shstrndx == SHN_UNDEF) printf("Pas de table de nom des sections");
  else printf("Index d'entrée de la table des nom de sections : %d\n", header->e_shstrndx);
  return header;
}

int indice_section(char nom,Elf32_Ehdr header,Elf32_Shdr *sheader){
	int i=0;	
	while ((nom != sheader[i].sh_name)&&(i< header.e_shnum)){
		i++;
	}
	if (nom == sheader[i].sh_name){
		return i;
	}
	else {
		return-1;
	}
}



void affichage_contenu_section (int num,Elf32_Ehdr header,Elf32_Shdr** sheader , FILE * f){
	num--;
	if ((num >= header.e_shnum)||(num <0)){
		printf("erreur section inexistante \n");
	}
	else {
		if (sheader[num]->sh_size==0){
			printf("section vide\n");
			printf("\n");}
		else {
		uint8_t contenu[sheader[num]->sh_size];
		fseek(f,sheader[num]->sh_offset,SEEK_SET);
		fread(contenu, 1,sheader[num]->sh_size, f);
		for (int i=0; i<sheader[num]->sh_size; i++){
			printf("%02X ",contenu[i]);
		}
		printf("\n");}
	}
}

	
int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Il faut 1 arguments");
		exit(0);
  	}
  	FILE * src = fopen(argv[1], "r");
  	if(src){
		Elf32_Ehdr * header;
		printf("\n");
		printf(" /// HEADER ELF /// \n");
		printf("\n");
    		header = elf_read_entete(src);
		Elf32_Shdr *sheader[header->e_shnum];
		printf("\n");
		printf(" /// section HEADER TABLE /// \n");
		printf("\n");
		afficherTableSectionHeader(src,*header,sheader);
		printf("\n");
		printf(" /// section ELF /// \n");
		printf("\n");
		for (int i=0; i<10;i++){
			printf("section %d \n",i);
			affichage_contenu_section(i,*header,sheader,src);}
		
  	} else {
   		printf("bad file");
  	}
}
