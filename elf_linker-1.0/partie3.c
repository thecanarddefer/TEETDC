#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include <string.h>
#include "partie2.h"
#include "partie3.h"


int indice_section(char* nom,Elf32_Ehdr header,Elf32_Shdr **sheader,FILE* fp){
	char* strTabSection =(char *) malloc(sheader[header.e_shstrndx]->sh_size);
	strTabSection = createStrTab(sheader, fp, header.e_shstrndx);
    int i=0;
	int iden=0;
    while ((!iden )&&(i< header.e_shnum)){
		iden=1;
		int j = sheader[i]->sh_name,k=0;
        while(strTabSection[j+1] != '\0' && iden ){
			if (strTabSection[j]!=nom[k]){
				iden=0;}
          	j++;
			k++;
        }
        i++;
    }
    if (iden){
		free(strTabSection);
        return i;
    }
    else {
		free(strTabSection);
        return-1;
    }
}
	

void affichage_contenu_section (int num,Elf32_Ehdr header,Elf32_Shdr** sheader , FILE * f){
    printf("section %d \n",num);
	printf("\n");
    num--;
    if ((num >= header.e_shnum)||(num <0)){
        printf("erreur section inexistante \n");
        printf("\n");
    }
    else {
		printf("nom de section : ");
		char* strTabSection = malloc(sheader[header.e_shstrndx]->sh_size);
    		strTabSection = createStrTab(sheader, f, header.e_shstrndx);
		int j = sheader[num]->sh_name;
        	while(strTabSection[j] != '\0'){
          		printf("%c", strTabSection[j]);
          		j++;
        	}
		printf("\n");
        	if (sheader[num]->sh_size==0){
			printf("\n");
            		printf("section vide\n");
            		printf("\n");
		}
        	else {
		    uint8_t contenu[sheader[num]->sh_size];
		    fseek(f,sheader[num]->sh_offset,SEEK_SET);
		    fread(contenu, 1,sheader[num]->sh_size, f);
			for (int i=0; i<sheader[num]->sh_size; i++){
				if (i%16==0){
					printf("\n");
					printf("  0x%08x ",i);}
				if (i%4==0){
					printf(" ");}
				printf("%02X",contenu[i]);
			}
		    printf("\n");
			printf("\n");	
					
		}
    }
 
}
