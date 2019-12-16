#include <stdlib.h>
#include <stdio.h>
#include <elf.h>


// if latabledessections est vide : e_shnum = 0
// sinon : 
// e_shentsize * e_shnum = Taille de la table des sections 
// e_shoff : pour pouvoir se positionner ou bon nous semble  
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
        printf("type de la section: %d \n",sheader[i]->sh_type);
        printf("drapeau de la section: %d \n",sheader[i]->sh_flags);
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
        free(sheader[i]);
    }
}

void printSh_Name(Elf32_Word leMot){
    
}