#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include <string.h>
#include "partie2.h"
#include "partie3.h"


int indice_section(char* nom,Elf32_Ehdr header,Elf32_Shdr **sheader,FILE* fp){
	char* strTabSection = createStrTab(sheader, fp, header.e_shstrndx);// recuperation de la table des noms de section
	int i=0;// compteur (section)
	int iden=0;// booleen de la concordance du nom de section rechercher et de celui en cour de comparaison
	while ((!iden )&&(i< header.e_shnum)){//boucle sur les section jusqu'à trouver une concordance ou avoir fais tout les nom de section
		iden=1;//passe a vrai momentanémant pour entrer dans la boucle suivante
		int j = sheader[i]->sh_name,k=0;//j=indice du nom
		while(strTabSection[j+1] != '\0' && iden ){//boucle sur le nom
			if (strTabSection[j]!=nom[k]){//test si les noms corresponde sinon iden passe à faux
				iden=0;}
          		j++;
			k++;
		}
		i++;
	}
	if (iden){//s'il y a eu concordance
		free(strTabSection);//libere la table des noms de section
        	return i-1;
	}
	else {//s'il n'y a pas eu concordance
		free(strTabSection);//libere la table des noms de section
        	return-1;
	}
}
	

void affichage_contenu_section (int num,Elf32_Ehdr header,Elf32_Shdr** sheader , FILE * f){
    printf("section %d \n",num);//affiche le numeros de section
	printf("\n");
    if ((num >= header.e_shnum)||(num <0)){//si section inexistante
        printf("erreur section inexistante \n");
        printf("\n");
    }
    else {//sinon
		printf("nom de section : ");
		char* strTabSection = createStrTab(sheader, f, header.e_shstrndx);// recuperation de la table des noms de section
		int j = sheader[num]->sh_name;
        	while(strTabSection[j] != '\0'){//affichage du nom de section
          		printf("%c", strTabSection[j]);
          		j++;
        	}
		printf("\n");
        	if (sheader[num]->sh_size==0){//affiche si la section est vide
			printf("\n");
            		printf("section vide\n");
            		printf("\n");
		}
        	else {//sinon affiche le contenu de la section
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
		free(strTabSection);//libere la table des noms de section
    }
	
}
