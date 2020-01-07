#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie2.h"
#include "partie3.h"
#include "partie5.h"




void printTypeReloc(Elf32_Word r_info){
	printf("R_ARM_");// affiche le type de relocation
	switch(ELF32_R_TYPE(r_info)){
    		case R_ARM_NONE: 	printf("NONE		");break;//0
    		case R_ARM_PC24: 	printf("PC24		");break;
    		case R_ARM_ABS32: 	printf("ABS32		");break;
    		case R_ARM_REL32: 	printf("REL32		");break;
    		case R_ARM_PC13: 	printf("PC13		");break;
    		case R_ARM_ABS16: 	printf("ABS16		");break;//5
    		case R_ARM_ABS12: 	printf("ABS12		");break;
		case R_ARM_THM_ABS5: 	printf("THM_ABS5	");break;
    		case R_ARM_ABS8: 	printf("ABS8		");break;
    		case R_ARM_SBREL32: 	printf("SBREL32		");break;
    		case R_ARM_THM_PC22: 	printf("THM_PC22	");break;//10

    		case R_ARM_THM_PC8: 	printf("THM_PC8		");break;
    		case R_ARM_AMP_VCALL9: 	printf("AMP_VCALL9	");break;
    		case R_ARM_THM_SWI8: 	printf("THM_SWI8	");break;
    		case R_ARM_XPC25: 	printf("XPC25		");break;//15
    		case R_ARM_THM_XPC22: 	printf("THM_XPC22	");break;
		case R_ARM_TLS_DTPMOD32:printf("TLS_DTPMOD32	");break;
    		case R_ARM_TLS_DTPOFF32:printf("TLS_DTPOFF32	");break;
    		case R_ARM_TLS_TPOFF32: printf("TLS_TPOFF32	");break;
    		case R_ARM_COPY: 	printf("COPY		");break;//20

    		case R_ARM_GLOB_DAT: 	printf("GLOB_DAT	");break;
    		case R_ARM_JUMP_SLOT: 	printf("JUMP_SLOT	");break;
    		case R_ARM_RELATIVE: 	printf("RELATIVE	");break;
    		case R_ARM_GOTOFF: 	printf("GOTOFF		");break;
    		case R_ARM_GOTPC: 	printf("GOTPC		");break;//25
    		case R_ARM_GOT32: 	printf("GOT32		");break;
		case R_ARM_PLT32: 	printf("PLT32		");break;
    		case R_ARM_CALL: 	printf("CALL		");break;
    		case R_ARM_JUMP24: 	printf("JUMP24		");break;
    		case R_ARM_THM_JUMP24: 	printf("THM_JUMP24	");break;//30

    		default:		printf("AUTRE		");break;
	}
} 
 

void  afficherRelTables(reloc * relTable, Elf32_Ehdr header,Elf32_Sym ** STable,char * strTab){
	printf(" Offset\t\t");//affiche les colonne de la table des relocations
    	printf("Info\t\t");
    	printf("Type\t\t\t");
    	printf("Sym.val\t\t");
    	printf("Sym.nom\t\n");
        for (int j=0; j<relTable->nbreloc;j++){//boucle sur les relocation
            	//r_offset
            	printf("%08X \t",relTable->tab[j]->r_offset);
		// r_info
            	printf("%08X \t",relTable->tab[j]->r_info);
		// type
		printTypeReloc(relTable->tab[j]->r_info);
		//sym.val
		printf("0x%08X\t",STable[ELF32_R_SYM(relTable->tab[j]->r_info)]->st_value);
		//sym.nom
		int k = STable[ELF32_R_SYM(relTable->tab[j]->r_info)]->st_name;
        	while(strTab[k] != '\0'){// affiche le nom du symbole
          		printf("%c", strTab[k]);
          		k++;
        		}
        	printf("\n");
           	}
        
    }



int nombre_reloc (Elf32_Ehdr header,Elf32_Shdr ** sheader){
	// comptage des table de relocation( compteurRel)
	int compteurRel=0;
    	for(int i=0;i<header.e_shnum;i++){
        	switch (sheader[i]->sh_type){
            		case 9 : compteurRel++;break;
            		default: break;
        	}
    	}
	return compteurRel;
}

void info_reloc (Elf32_Ehdr header,Elf32_Shdr ** sheader,reloc ** relTable,FILE * fp){
	int k=0 ;
	for(int i=0;i<header.e_shnum;i++){//boucle sur les sections
        	switch (sheader[i]->sh_type){//cherche celle de relocation
            		case 9 : relTable[k]->nbreloc=(sheader[i]->sh_size/sizeof(Elf32_Rel));
				relTable[k]->ind_sect=i;//met a jour leur nombres et leur index
				char nom[20];
				char* strTabSection = createStrTab(sheader, fp, header.e_shstrndx);
				int j = sheader[i]->sh_name + 4 ,g=0;
				while(strTabSection[j] != '\0'){ // cherche le nom de la section visée
					nom[g]=strTabSection[j];
	  				j++;
					g++;
				}
				relTable[k]->dest=indice_section(nom,header,sheader,fp)-1;//trouve l'indice de la section visée
				k++;break;
            		default: break;
        	}
    	}

}

void affiche_toute_reloc(FILE * fp,reloc ** relTable, Elf32_Ehdr header, Elf32_Shdr **sheader,Elf32_Sym ** STable,char * strTab,int compteurRel){
	for(int l=0;l<compteurRel;l++){//boucle sur les tables de relocation
		printf(" \n");//affiche les information sur la table de relocation
		printf("relocation section '");
		char* strTabSection = createStrTab(sheader, fp, header.e_shstrndx);
		int j = sheader[relTable[l]->ind_sect]->sh_name;
		while(strTabSection[j] != '\0'){ // affiche le nom de la section de relocation
			printf("%c", strTabSection[j]);
			j++;
		}
		printf("' offset : 0x%x et contient %d entree",sheader[relTable[l]->ind_sect]->sh_offset,relTable[l]->nbreloc);
		printf(" \n");

		afficherRelTables(relTable[l],header,STable,strTab);//affiche les info sur les relocations
	}
	printf(" \n");
}

void getRelTable (FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader,Elf32_Sym ** STable,char * strTab,int compteurRel, reloc ** relTable){
	
	// allocation des tables de relocation
	
   	for(int i=0;i<compteurRel;i++){
            		relTable[i] = (reloc *)malloc(sizeof(reloc));
    	}
	if (compteurRel==0 ){//s'il n'y a pas de relocation
		printf("il n'y a pas de relocation dans ce fichier\n");
	}
	
	// lecture des relocation
	else { 
		info_reloc(header,sheader,relTable,fp);
	    	for(int l=0;l<compteurRel;l++){//boucle sur les table de relocation
			
        		fseek(fp,sheader[relTable[l]->ind_sect]->sh_offset,SEEK_SET);
			
        		for (int j=0; j<(relTable[l]->nbreloc);j++){ // lit les relocations
				relTable[l]->tab[j]=malloc(sizeof(Elf32_Rel));
            			fread(relTable[l]->tab[j], 1,sizeof(Elf32_Rel),fp);
            		}
			
		      
	    	}
		affiche_toute_reloc(fp,relTable,header,sheader,STable,strTab,compteurRel);
	}	
	
}






