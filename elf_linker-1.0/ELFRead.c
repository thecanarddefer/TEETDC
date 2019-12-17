#include <stdio.h>
#include <elf.h>
#include <stdlib.h>


// if latabledessections est vide : e_shnum = 0
// sinon :
// e_shentsize * e_shnum = Taille de la table des sections
// e_shoff : pour pouvoir se positionner ou bon nous semble
void printSh_Type(Elf32_Word leMot){
    switch(leMot){
        case  0 :   printf("Section header table entry unused ");break;
        case  1 :   printf("Program data                      ");break;
        case  2 :   printf("Symbol table                      ");break;
        case  3 :   printf("String table                      ");break;
        case  4 :   printf("Relocation entries with addends   ");break;
        case  5 :   printf("Symbol hash table                 ");break;
        case  6 :   printf("Dynamic linking information       ");break;
        case  7 :   printf("Notes                             ");break;
        case  8 :   printf("Program space with no data (bss)  ");break;
        case  9 :   printf("Relocation entries, no addends    ");break;
        case  10:   printf("Reserved                          ");break;
        case  11:   printf("Dynamic linker symbol table       ");break;
        case  14:   printf("Array of constructors             ");break;
        case  15:   printf("Array of destructors              ");break;
        case  16:   printf("Array of pre-constructors         ");break;
        case  17:   printf("Section group                     ");break;
        case  18:   printf("Extended section indeces          ");break;
        case  19:   printf("Number of defined types           ");break;
        default :   printf("Non traité : %X             ",leMot);break;
        printf("\t");
    }
}

void printFlagz(Elf32_Word leMot){
    switch(leMot){
        case 1 << 0     :   printf("Writable                                  ");break;
        case 1 << 1     :   printf("Occupies memory during execution          ");break;
        case 1 << 2     :   printf("Executable                                ");break;
        case 1 << 4     :   printf("Might be merged                           ");break;
        case 1 << 5     :   printf("Contains nul-terminated strings           ");break;
        case 1 << 6     :   printf("`sh_info' contains SHT index              ");break;
        case 1 << 7     :   printf("Preserve order after combining            ");break;
        case 1 << 8     :   printf("Non-standard OS specific handling required");break;
        case 1 << 9     :   printf("Section is member of a group              ");break;
        case 1 << 10    :   printf("Section hold thread-local data.           ");break;
        case 1 << 11    :   printf("Section with compressed data.             ");break;
        case 0x0ff00000 :   printf("OS-specific.                              ");break;
        case 0xf0000000 :   printf("Processor-specific                        ");break;
        default: printf("%d                                                ",leMot);
        printf("\t");
    }
}
char * createStrTab(Elf32_Shdr** sheader, FILE * fp, int num){
    char * strTab = malloc(sheader[num]->sh_size);
    fseek(fp,sheader[num]->sh_offset,SEEK_SET);
    fread(strTab, sheader[num]->sh_size/sizeof(char), sizeof(char), fp);
    return strTab;
}
void afficherTableSectionHeader(FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader){
    printf("Num\t");
    printf("Nom\t\t\t");
    printf("Type\t\t\t\t");
    printf("@virt\t");
    printf("Off\t");
    printf("Tail\t");
    printf("Link\t");
    printf("Info\t");
    printf("Align\t");
    printf("TailTab\t");
    printf("Flag\n");
    fseek(fp,header.e_shoff,SEEK_SET);
    for(int i=0;i<header.e_shnum;i++){
        sheader[i]=(Elf32_Shdr*)malloc(sizeof(Elf32_Shdr));
        fread(sheader[i],1, sizeof(Elf32_Shdr), fp);
    }
    char* strTabSection = malloc(sheader[header.e_shstrndx]->sh_size);
    strTabSection = createStrTab(sheader, fp, header.e_shstrndx);
    for(int i=0;i<header.e_shnum;i++){
        printf("%d \t",i+1);
        int j = sheader[i]->sh_name;
        while(strTabSection[j] != '\0'){
          printf("%c", strTabSection[j]);
          j++;
        }
        while(j<sheader[i]->sh_name+20){
          printf(" ");
          j++;
        }
        printf("\t");
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
int findSymTab(Elf32_Shdr** sheader, int max){
    int num=0;
    while(sheader[num]->sh_type!=2 && num< max){
        num++;
    }
    return num;
}
int findStrTabSym(Elf32_Shdr** sheader, Elf32_Ehdr header){
    int num=0;
    while(sheader[num]->sh_type!=3 || num==header.e_shstrndx){
        num++;
    }
    return num;
}


void printInfoSym(unsigned char info){
  switch(ELF32_ST_BIND(info)){
    case 0: printf("LOCAL");break;
    case 1: printf("GLOBAL");break;
    default:printf("AUTRE");break;
  }
  printf("\t");
  switch(ELF32_ST_TYPE(info)){
    case 0: printf("NOTYPE");break;
    case 1: printf("OBJECT");break;
    case 2: printf("FUNC");break;
    case 3: printf("SECTION");break;
    case 4: printf("FILE");break;
    case 5: printf("COMMON");break;
    default:printf("AUTRE");break;
  }
  printf("\t");
}

void readSymTab(FILE * fp, Elf32_Shdr** sheader, int symTabNum,Elf32_Sym ** STable, char * strTab){
    fseek(fp,sheader[symTabNum]->sh_offset,SEEK_SET);
    printf("Num\t");
    printf("Valeur\t");
    printf("Tail\t");
    printf("Lien\t");
    printf("Type\t");
    printf("Ndx\t");
    printf("Nom\t\n");
    for(int i=0; i<sheader[symTabNum]->sh_size/sizeof(Elf32_Sym);i++){
        STable[i]=(Elf32_Sym*)malloc(sizeof(Elf32_Sym));
        fread(STable[i],1, sizeof(Elf32_Sym), fp);
        printf("%d\t",i);
        printf("0x%X\t",STable[i]->st_value);
        printf("%d\t",STable[i]->st_size);
        printInfoSym(STable[i]->st_info);
        //printf("autre du symbole : %d\n",STable[i]->st_other);
        if(STable[i]->st_shndx == 0) printf("UND\t");
        else printf(" %d\t",STable[i]->st_shndx);
        int j = STable[i]->st_name;
        while(strTab[j] != '\0'){
          printf("%c", strTab[j]);
          j++;
        }
        printf("\n");
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
        affichage_contenu_section(i,*header,sheader,src);
      }
      printf("\n");
      printf(" /// Affichage des symboles /// \n");
      printf("\n");
      int symTabNum=findSymTab(sheader,header->e_shnum);
      if(symTabNum < header->e_shnum){
        int numSec = findStrTabSym(sheader,*header);
        //char strTab[sheader[numSec]->sh_size/sizeof(char)] = createStrTab(sheader, src, numSec);
        char* strTab = malloc(sheader[numSec]->sh_size);
        strTab = createStrTab(sheader, src, numSec);
        Elf32_Sym * STable[sheader[symTabNum]->sh_size/sizeof(Elf32_Sym)];
        readSymTab(src, sheader, symTabNum, STable, strTab);
      }else{
        printf("il n'y a pas de table des symboles \n");
      }
  	} else {
   		printf("bad file");
  	}


}
