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
	
void affiche_val_ASCII(uint8_t * contenu,int j,int i){
	for (int e=j; e>0;e--){
		if (contenu[i-e]<='~' && contenu[i-e]>='!'	){//affiche le caractère ascii si il est défini 
			printf("%c",contenu[i-e]);
		}
		else {
			printf(".");//sinon affiche .
		}
	}
}

void alignement(int i){
	if (i%16!=0){
		for(int j=(16-i%16)*2;j>0;j--){//indique le nombre d'espace a fficher pour s'aligner
			printf(" ");
			if (j%8==0){//les espace entre les blocs
				printf(" ");
			}
		}
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
		printf("Vidange hexadécimale de la section « ");
		char* strTabSection = createStrTab(sheader, f, header.e_shstrndx);// recuperation de la table des noms de section
		int j = sheader[num]->sh_name;
        	while(strTabSection[j] != '\0'){//affichage du nom de section
          		printf("%c", strTabSection[j]);
          		j++;
        	}
		printf(" » :");
		printf("\n");
        	if (sheader[num]->sh_size==0){//affiche si la section est vide
			printf("\n");
            		printf("section vide\n");
            		printf("\n");
		}
        	else {//sinon affiche le contenu de la section
		    uint8_t contenu[sheader[num]->sh_size];
		    fseek(f,sheader[num]->sh_offset,SEEK_SET);//replace le curseure sur le début de section
		    fread(contenu, 1,sheader[num]->sh_size, f);//lis la section
			int i=0;
			while ( i<sheader[num]->sh_size){//boucle sur la taille de section
				if (i%16==0){//si on est en bout de ligne
					if (i!=0){// si on est a la fin de ligne
						printf(" ");
						affiche_val_ASCII(contenu,16,i);
					}
					printf("\n");// si on est en début de ligne
					printf("  0x%08x ",i); //affiche l'adresse de l'octet
				}
				if (i%4==0){//si on est a une fin de bloc(4 octets)
					printf(" ");
				}
				printf("%02X",contenu[i]);//affiche l'octet sous forme hexadécimale
				i++;
			}
			printf(" ");
			int r=i%16;//nombre d'octets de la dernière ligne
			if (r==0){r=16;}
			alignement(i);
			affiche_val_ASCII(contenu,r,i);
		    printf("\n");
		    printf("\n");	
					
		}
		free(strTabSection);//libere la table des noms de section
    }
	
}
