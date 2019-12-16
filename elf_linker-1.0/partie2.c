#include <stdlib.h>
#include <stdio.h>
#include <elf.h>


// if latabledessections est vide : e_shnum = 0
// sinon : 
// e_shentsize * e_shnum = Taille de la table des sections 
// e_shoff : pour pouvoir se positionner ou bon nous semble 
void printSh_Type(Elf32_Word leMot){
    switch(leMot){
        case  0 :   printf("type de la section: Section header table entry unused \n");
        case  1 :   printf("type de la section: Program data\n");
        case  2 :   printf("type de la section: Symbol table\n");
        case  3 :   printf("type de la section: String table\n");
        case  4 :   printf("type de la section: Relocation entries with addends \n");
        case  5 :   printf("type de la section: Symbol hash table \n");
        case  6 :   printf("type de la section: Dynamic linking information \n");
        case  7 :   printf("type de la section: Notes \n");
        case  8 :   printf("type de la section: Program space with no data (bss) \n");
        case  9 :   printf("type de la section: Relocation entries, no addends \n");
        case  10:   printf("type de la section: Reserved \n");
        case  11:   printf("type de la section: Dynamic linker symbol table \n");
        case  14:   printf("type de la section: Array of constructors \n");
        case  15:   printf("type de la section: Array of destructors \n");
        case  16:   printf("type de la section: Array of pre-constructors \n");
        case  17:   printf("type de la section: Section group \n");
        case  18:   printf("type de la section: Extended section indeces \n");
        case  19:   printf("type de la section: Number of defined types \n");
        default :   printf("(non Traité)type de la Section: %d",leMot);
    }
}

void printFlagz(Elf32_Word leMot){
    switch(leMot){
        case (1 << 0)   :   printf("Writable");
        case (1 << 1)   :   printf("Occupies memory during execution");
        case (1 << 2)   :   printf("Executable");
        case (1 << 4)   :   printf("Might be merged");
        case (1 << 5)   :   printf("Contains nul-terminated strings");
        case (1 << 6)   :   printf("`sh_info' contains SHT index");
        case (1 << 7)   :   printf("Preserve order after combining");
        case (1 << 8)   :   printf("Non-standard OS specific handling required");
        case (1 << 9)   :   printf("Section is member of a group");
        case (1 << 10)  :   printf("Section hold thread-local data.");
        case (1 << 11)  :   printf("Section with compressed data.");
        case 0x0ff00000 :   printf("OS-specific.");
        case 0xf0000000 :   printf("Processor-specific");
        case (1 << 30)  :   printf("Special ordering requirement(Solaris)"); 
        case (1U << 31) :   printf("Section is excluded unless referenced or allocated (Solaris)");
    }
}

Elf32_Shdr * afficherTableSectionHeader(FILE * fp, Elf32_Ehdr header){
    fseek(fp,header.e_shoff,SEEK_SET);
    Elf32_Shdr* sheader[header.e_shnum];
    for(int i=0;i<header.e_shnum;i++){
        sheader[i]=(Elf32_Shdr*)malloc(sizeof(Elf32_Shdr));
        fread(&sheader[i],1, sizeof(Elf32_Shdr), fp);
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
    }
    return *sheader;
}
void freeSHDR(Elf32_Shdr * sheader){
    for (int i=0;i<sizeof(sheader)/sizeof(Elf32_Shdr);i++){
        free(&sheader[i]);
    }
}



