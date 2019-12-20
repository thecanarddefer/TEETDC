#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "partie2.h"
#include "partie5.h"




void printTypeReloc(Elf32_Word r_info){
	printf("R_ARM_");
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
 

void  afficherRelTables(int nbreloc,Elf32_Rel ** relTable, Elf32_Ehdr header,Elf32_Sym ** STable,char * strTab){
	printf(" Offset\t\t");
    	printf("Info\t\t");
    	printf("Type\t\t\t");
    	printf("Sym.val\t\t");
    	printf("Sym.nom\t\n");
        for (int j=0; j<nbreloc;j++){
            	//r_offset
            	printf("%08X \t",relTable[j]->r_offset);
		// r_info
            	printf("%08X \t",relTable[j]->r_info);
		// type
		printTypeReloc(relTable[j]->r_info);
		//sym.val
		printf("0x%08X\t",STable[ELF32_R_SYM(relTable[j]->r_info)]->st_value);
		//sym.nom
		int k = STable[ELF32_R_SYM(relTable[j]->r_info)]->st_name;
        	while(strTab[k] != '\0'){
          		printf("%c", strTab[k]);
          		k++;
        		}
        	printf("\n");
            //Elf32_word r_info
            //printf("\tinfo : %d\n",laTable->relTable[i].r_info);
           	}
        
    }


void  afficherRelaTables(int nbreloc,Elf32_Rela ** relaTable, Elf32_Ehdr header){
	int j=0;
	while (j<nbreloc){
            
            //Elf32_Addr r_offset
            	printf("Relocation numéro : %d",j);
            	printf("\tOffset : %x\n",relaTable[j]->r_offset);
            //Elf32_word r_info
            //printf("\tinfo : %d\n",laTable->relTable[i].r_info);
            	j++;
	}
        
}

void getRelTable (FILE * fp, Elf32_Ehdr header, Elf32_Shdr ** sheader,Elf32_Sym ** STable,char * strTab){
  	int nbreloc=0,nbreloca=0;
    	int compteurRel=0 ,compteurRela=0;
    	for(int i=0;i<header.e_shnum;i++){
        	switch (sheader[i]->sh_type){
            		case 9 : compteurRel++;nbreloc=(sheader[i]->sh_size/sizeof(Elf32_Rel));break;
            		case 4 : compteurRela++;nbreloca=(sheader[i]->sh_size/sizeof(Elf32_Rela));break;
            		default: break;
        	}
    	}


    	Elf32_Rel * relTable[compteurRel][nbreloc];
    	Elf32_Rela * relaTable[compteurRela][nbreloca];

    	for(int i=0;i<compteurRel;i++){
        	for (int j=0;j<nbreloc;j++){
            		relTable[i][j] = (Elf32_Rel*)malloc(sizeof(Elf32_Rel));
           	 }
    	}

    	for(int i=0;i<compteurRela;i++){
        	for (int j=0;j<nbreloca;j++){
           		relaTable[i][j] = (Elf32_Rela*)malloc(sizeof(Elf32_Rela));
            	}
    	}


    	compteurRel=0;
    	compteurRela = 0;

    	for(int i=0;i<header.e_shnum;i++){
        	switch (sheader[i]->sh_type){
            		case 9 :
                		fseek(fp,sheader[i]->sh_offset,SEEK_SET);
                		for (int j=0; j<nbreloc ;j++){
                    			fread(relTable[compteurRel][j], 1,sizeof(Elf32_Rel),fp);
                    		}
				printf(" \n");
        			printf("relocation section '");
				char* strTabSection = malloc(sheader[header.e_shstrndx]->sh_size);
    				strTabSection = createStrTab(sheader, fp, header.e_shstrndx);
				int j = sheader[i]->sh_name;
        			while(strTabSection[j] != '\0'){
          				printf("%c", strTabSection[j]);
          				j++;
        			}
				printf("' offset : 0x%x et contient %d entree",sheader[i]->sh_offset,nbreloc);
        			printf(" \n");
        			afficherRelTables(nbreloc, relTable[compteurRel],header,STable,strTab);
                		compteurRel++;
                		break;
            
            /*case 4 : 
                fseek(fp,sheader[i]->sh_offset,SEEK_SET);
                for (int j=0; j<nbreloca ;j++){
                    fread(relaTable[compteurRela][j], 1,sizeof(Elf32_Rela),fp);
                    }
                compteurRela++;
                break;*/

            		default: 
                		break;
        	}
    	}
    /*for (int i=0;i<compteurRela;i++){
        printf(" \n");
        printf("table des relocation avec addent %d\n",i);
        printf(" \n");
        afficherRelaTables(nbreloca, relaTable[i],header);
	}*/
	if (compteurRel==0 && compteurRela==0){
		printf("il n'y a pas de relocation dans ce fichier\n");
	}
	
}






