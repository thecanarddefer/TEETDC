#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include "ELFFusion.h"

int isMergeable(Elf32_Word type){
  //Si les sections sont des trypes suivants, il faut les fusionner
  switch(type){
    case(SHT_NULL): return 1;
    case(SHT_PROGBITS): return 1;
    case(SHT_SYMTAB): return 1;
    case(SHT_STRTAB): return 1;
    case(SHT_REL): return 1;
  }
  return 0;
}
//////////// Fonctions d'écriture dans le nouveau fichier
void ecrireSheader(Elf32_Shdr ** sheader, FILE * dest, int nombreDeSection){
  fseek(dest, 0, SEEK_END);
  for(int i = 0; i < nombreDeSection;i++){
    fwrite(sheader[i], sizeof(Elf32_Shdr), 1, dest);
  }
}

void ecrireHeader(Elf32_Ehdr * header1, FILE * dest){
  fseek(dest, 0, SEEK_SET);
  fwrite(header1, header1->e_ehsize, 1, dest);
}

void ecrireSectionSymbole(FILE * dest, Elf32_Sym ** symTab, int nbSymbole){
  for(int i = 0; i < nbSymbole;i++){
    fwrite(symTab[i], sizeof(Elf32_Sym), 1, dest);
  }
}

void ecrireSectionStringSymbole(FILE * dest, char * strTabSym, int tailleSTRTAB){
  fwrite(strTabSym, tailleSTRTAB, 1, dest);
}

void ecrireSection(FILE * dest, FILE * src, Elf32_Shdr ** sheader, int indice){
  uint8_t contenu[sheader[indice]->sh_size];
  fseek(src, sheader[indice]->sh_offset, SEEK_SET);
  fread(contenu, 1,sheader[indice]->sh_size, src);
  fwrite(contenu, sheader[indice]->sh_size, 1, dest);
}

void ecrireSectionMergeable(FILE * dest, FILE * src1, FILE * src2, Elf32_Shdr ** sheader1, Elf32_Shdr ** sheader2, int indice1, int indice2){
  ecrireSection(dest, src1, sheader1, indice1);
  ecrireSection(dest, src2, sheader2, indice2);
}
//////////////////////////////

char * getNomSymboles(Elf32_Word name,char* strTab, int taille){
  int compteur =name;
  char nom[taille];
  int taillefinale=0;
  //On part de l'indice donné qui pointe dans la table des chaines
  //on rentre tous les caractères dans une nouvelle chaine tant qu'on ne rencontre pas le caractère final \0
  while(strTab[compteur]!='\0'){
    nom[taillefinale]=strTab[compteur];
    taillefinale++;
    compteur++;
  }
  char * nomFinal=malloc(taillefinale+2);
  for(int i=0; i<taillefinale;i++){
    nomFinal[i]=nom[i];
  }
  nomFinal[taillefinale+1] = '\0';
  return nomFinal;
}

int memeString(char* s1, char * s2){
  int i = 0;
  while(s1[i] == s2[i] && (s2[i] != '\0' || s1[i] != '\0')){
    i++;
  }
  if(s1[i] == s2[i]){
    return 1;
  }
  else {
    return 0;
  }
}
void mergeRelocationTable(Elf32_Shdr ** sheader1,Elf32_Shdr ** sheader2, int indice1, int indice2,char* strTabSection1, char* strTabSection2, FILE * src1, FILE * dest, reloc **relTable, int* nouveauxIndices,  int compteurRel, int nbSection1, int * indicesFich1, reloc **relTable1, int compteurRel1){
  //Fusion des tables de reimplantation
  //Gestion de la table du fichier source 1
  int count=0;
  //permet de trouver la table bonne table de relocation
  while(count< compteurRel1 && relTable1[count]->ind_sect!=indice1){
    count++;
  }
  //Mise à jour du paramètre r_info de chacune des reimplantations :
  //Il faut changer l'attribut SYM car la position des symboles a changé avec la fusion des tables de symboles
  for(int i=0; i< relTable1[count]->nbreloc;i++){
    int sym = indicesFich1[ELF32_R_SYM(relTable1[count]->tab[i]->r_info)];
    int type = ELF32_R_TYPE(relTable1[count]->tab[i]->r_info);
    relTable1[count]->tab[i]->r_info = ELF32_R_INFO(sym,type);
    fwrite(relTable1[count]->tab[i], sizeof(Elf32_Rel), 1, dest);
  }
  //Gestion de la table du fichier source 2
  count=0;
  int offsetAAjouter;
  while(count< compteurRel && relTable[count]->ind_sect!=indice2){
    count++;
  }
  //Il faut trouver la section référencée et connaître sa taille pour ajouter à l'offset
  int sectionCible = relTable[count]->dest;
  char * nomSect = getNomSymboles(sheader2[sectionCible]->sh_name,strTabSection2, 50);
  int j=0;
  while(j<nbSection1 &&!memeString(nomSect,getNomSymboles(sheader1[j]->sh_name, strTabSection1, 50))){
    j++;
  }
  offsetAAjouter= sheader1[j]->sh_size - sheader2[sectionCible]->sh_size;
  //Mise à jour du paramètre r_info de chacune des reimplantations :
  //Il faut changer l'attribut SYM car la position des symboles a changé avec la fusion des tables de symboles
  //Il faut également modifier le paramètre r_offset pour lui ajouter un décalage qui vaut la taille de la table référencée
  for(int i=0; i< relTable[count]->nbreloc;i++){
    relTable[count]->tab[i]->r_offset += offsetAAjouter;
    int sym = nouveauxIndices[ELF32_R_SYM(relTable[count]->tab[i]->r_info)];
    int type = ELF32_R_TYPE(relTable[count]->tab[i]->r_info);
    relTable[count]->tab[i]->r_info = ELF32_R_INFO(sym,type);
    fwrite(relTable[count]->tab[i], sizeof(Elf32_Rel), 1, dest);
  }

}

int mergeSheader(Elf32_Shdr ** sheader1, Elf32_Ehdr header1, Elf32_Shdr ** sheader2, Elf32_Ehdr header2, Elf32_Shdr ** sheaderFin, int nbSymbole, int tailleSTRTAB, Elf32_Sym ** symTab, char * strTabSym, FILE * dest, FILE * src1, FILE * src2, char* strTabSection1, char* strTabSection2, int* nouveauxIndices, reloc **relTable,  int compteurRel, int * indicesFich1, reloc **relTable1,  int compteurRel1, int lastLocalSym){
  int count = 0;
  int offset = 0; //test avec un offset set a la taille du header
  int j;
  // On itère sur les sections du fichier source 1
  for(int i=0; i<header1.e_shnum;i++){
    //Si la section est de type fusionnable, on lui applique des traitements
    if(isMergeable(sheader1[i]->sh_type)){
      j=0;
      //Permet de trouver la section qui a le même nom et qu'il faut fusionner dans le fichier source 2
      while(j<header2.e_shnum &&!memeString(getNomSymboles(sheader1[i]->sh_name, strTabSection1, 50),getNomSymboles(sheader2[j]->sh_name, strTabSection2, 50))){
        j++;
      }
      //Si on a trouvé une section avec un nom identique
      if(j!=header2.e_shnum){
        sheaderFin[i]=sheader1[i];
        int tmpsize =sheader1[i]->sh_size;
        //Cas section des symboles
        if(sheaderFin[i]->sh_type==SHT_SYMTAB){
          sheaderFin[i]->sh_size= sizeof(Elf32_Sym)*nbSymbole;
          sheaderFin[i]->sh_info = lastLocalSym;
          ecrireSectionSymbole(dest,symTab, nbSymbole);
          //Cas section des noms de symboles
        }else if(sheaderFin[i]->sh_type==SHT_STRTAB && i!= header1.e_shstrndx){
          sheaderFin[i]->sh_size=tailleSTRTAB;
          ecrireSectionStringSymbole(dest, strTabSym, tailleSTRTAB);
        }
        //Cas section de reimplantation
        else if(sheaderFin[i]->sh_type==SHT_REL){
          mergeRelocationTable(sheader1,sheader2,i,j,strTabSection1,strTabSection2, src1, dest, relTable, nouveauxIndices, compteurRel, header1.e_shnum , indicesFich1, relTable1, compteurRel1);
          sheaderFin[i]->sh_size+=sheader2[j]->sh_size;
        }
        else{
          //Autre cas de section à fusionenr
          ecrireSectionMergeable(dest, src1, src2, sheader1, sheader2, i, j);
          sheaderFin[i]->sh_size+=sheader2[j]->sh_size;
        }

        //gestion de l'allign
        if(sheader1[i]->sh_addralign<sheader2[j]->sh_addralign){
          sheaderFin[i]->sh_addralign=sheader2[j]->sh_addralign;
        }

        if(sheaderFin[i]->sh_addralign>1){
          if (sheaderFin[i]->sh_size % sheaderFin[i]->sh_addralign!=0){
            sheaderFin[i]->sh_size+= sheaderFin[i]->sh_addralign - (sheaderFin[i]->sh_size % sheaderFin[i]->sh_addralign);
          }
        }
      }
      else{
        //Cas d'une section à juste recopier si on a pas trouvé de section avec le même nom
        sheaderFin[i]=sheader1[i];
        ecrireSection(dest, src1, sheader1, i);
      }
    }else{
      //Cas d'une section à ne pas fusionner
      sheaderFin[i]=sheader1[i];
      ecrireSection(dest, src1, sheader1, i);
    }
    sheaderFin[i]->sh_offset=offset;
    if(offset==0){
      offset=52;
    }else{
      offset+=sheaderFin[i]->sh_size;
    }


    count++;

  }
  //On itère sur les sections du fichier 2
  //On ne veut écrire que les sections qui n'ont pas été traitée
  for(int i=0; i<header2.e_shnum;i++){
    j=0;
    while(sheader2[i]->sh_name =!sheaderFin[j]->sh_name && j<header1.e_shnum){
      j++;
    }
    //Si on tombe sur une section non traitée, on l'écrit
    if(j==header1.e_shnum){
      sheaderFin[count]=sheader2[i];
      sheaderFin[count]->sh_offset=offset;
      sheaderFin[count]->sh_name+=sheader1[header1.e_shstrndx]->sh_size;
      if(sheaderFin[count]->sh_type==SHT_REL ){
        sheaderFin[count]->sh_link=header1.e_shstrndx;
      }
      ecrireSection(dest, src2, sheader2, i);
      count++;
      offset+=sheaderFin[i]->sh_size;
    }
  }
  return count;

}
void writeSyms(Elf32_Sym ** symTab, Elf32_Sym ** symTabFin, char* strTab, char* strTabFin, int taille, int *compteurSTRTAB, int *count, int * indicesFich1, int glob){
  for(int i = 0; i < taille;i++){
    if((ELF32_ST_BIND(symTab[i]->st_info)!=1 && glob == 0) || (ELF32_ST_BIND(symTab[i]->st_info)==1 && glob == 1)){
      symTabFin[(*count)] = symTab[i];
      int l= symTabFin[(*count)]->st_name;

      symTabFin[(*count)]->st_name=(*compteurSTRTAB);
      while(strTab[l]!='\0'){
        strTabFin[(*compteurSTRTAB)]=strTab[l];
        l++;
        (*compteurSTRTAB)++;
      }
      strTabFin[(*compteurSTRTAB)]='\0';
      indicesFich1[i]=(*count);
      (*compteurSTRTAB)++;
      (*count)++;
    }
  }
}

int mergeSymAndStr(char* strTab1,char* strTab2,Elf32_Sym ** symTable1,Elf32_Sym ** symTable2, char * strTabFin, Elf32_Sym ** symTableFin, int t1, int t2, int * compteurSTRTAB, int * count, int* nouveauxIndices, int * indicesFich1){
  (*compteurSTRTAB)=1;
  strTabFin[0]='\0';
  //indicesFich1 et nouveauxIndice permettent de stocker l'ancien emplacement des symboles dans les table des symboles 1 et 2 avec le nouvel emplacement
  //Il faut d'abord écrire les symboles locaux puis les globaux
  //Ecriture des symboles locaux du premier fichier
  writeSyms(symTable1, symTableFin, strTab1, strTabFin, t1, compteurSTRTAB, count, indicesFich1, 0);

  //Ecriture des symboles locaux du deuxième fichier
  writeSyms(symTable2, symTableFin, strTab2, strTabFin, t2, compteurSTRTAB, count, nouveauxIndices, 0);

  int dernierlocal = (*count); // On enregistre la position du dernier symbole local
  //Ecriture des symboles globaux du premier fichier
  writeSyms(symTable1, symTableFin, strTab1, strTabFin, t1, compteurSTRTAB, count, indicesFich1, 1);

  //Ecriture des symboles globaux du deuxième fichier

  int k = 0;
  for(int j = 0;j < t2;j++){
    if(ELF32_ST_BIND(symTable2[j]->st_info)==1){
      while(k<(*count) && !memeString(getNomSymboles(symTable2[j]->st_name, strTab2, t2), getNomSymboles(symTableFin[k]->st_name, strTabFin, t1))){
        k++;
      }
      //Si on ne trouve pas de symbole global du même nom
      if(k==t1){
        symTableFin[(*count)]=symTable2[j];
        int l= symTableFin[(*count)]->st_name;
        symTableFin[(*count)]->st_name=(*compteurSTRTAB);
        while(strTab2[l]!='\0'){
          strTabFin[(*compteurSTRTAB)]=strTab2[l];
          l++;
          (*compteurSTRTAB)++;
        }
        strTabFin[(*compteurSTRTAB)]='\0';
        nouveauxIndices[j]=(*count);
        (*compteurSTRTAB)++;
        (*count)++;
      } else{
        //Si deux variables globales de même nom dans les 2 fichiers sont déclarées, l'édition échoue
        if(symTable2[j]->st_shndx != STN_UNDEF && symTableFin[k]->st_shndx != STN_UNDEF){
          printf("ERROR : 2 global variables have same name (double déclaration)");
          exit(0);
        //Si deux variables globales de même nom mais une seule est déclarée, on ne garde que celle-ci

        } else if(symTable2[j]->st_shndx != STN_UNDEF && symTableFin[k]->st_shndx == STN_UNDEF){
          int tmp =symTableFin[k]->st_name;
          symTableFin[k]=symTable2[j];
          symTableFin[k]->st_name =tmp;
        }
        nouveauxIndices[j]=k;

      }
      k=0;
    }
  }
  return dernierlocal;
}

Elf32_Ehdr createElfHeader(Elf32_Shdr ** sheaderFin, Elf32_Ehdr header1, int nombreDeSection){
  Elf32_Ehdr headerFin = header1;
  headerFin.e_shnum = nombreDeSection;
  headerFin.e_shoff = sheaderFin[nombreDeSection-1]->sh_offset + sheaderFin[nombreDeSection-1]->sh_size;
  return headerFin;
}

//////////////MAIN////////////////
int main(int argc, char *argv[]){
  if(argc != 4){
    printf("Il faut 3 arguments : fich1, fich2, dest");
    exit(0);
  }
  FILE * src1 = fopen(argv[1], "r");
  FILE * src2 = fopen(argv[2], "r");
  FILE * dest = fopen(argv[3], "w");
  // Sert à laisser la place pour le header qui est rajouté à la fin et fait toujours 52 octets
  for(int i=0; i<52; i++){
    fputc(' ',dest);
  }
  //Recuperer les Headers des 2 fichiers d'entrées
  Elf32_Ehdr * header1 = elf_read_entete(src1);
  Elf32_Ehdr * header2 = elf_read_entete(src2);

  //Recuperer les Sections Headers des 2 fichiers d'entrées
  Elf32_Shdr * sheader1[header1->e_shnum];
  Elf32_Shdr * sheader2[header2->e_shnum];
  lectureTableSectionHeader(src1,*header1,sheader1);
  lectureTableSectionHeader(src2,*header2,sheader2);

  //Recuperer les Section String table 2 fichiers d'entrées
  char* strTabSection1 = malloc(sheader1[header1->e_shstrndx]->sh_size);
  strTabSection1 = createStrTab(sheader1, src1, header1->e_shstrndx);
  char* strTabSection2 = malloc(sheader2[header2->e_shstrndx]->sh_size);
  strTabSection2 = createStrTab(sheader2, src2, header2->e_shstrndx);

  //Recuperer la table Symbole et String table des symboles 2 fichiers d'entrées
  //fichier 1
  int symTabNum=findSymTab(sheader1,header1->e_shnum);
  int numSec = 0;
  char* strTab1;
  numSec = findStrTabSym(sheader1,*header1);
  strTab1 = malloc(sheader1[numSec]->sh_size);
  strTab1 = createStrTab(sheader1, src1, numSec);
  Elf32_Sym * symTable1[sheader1[symTabNum]->sh_size/sizeof(Elf32_Sym)];
  readSymTab(src1, sheader1, symTabNum, symTable1, strTab1);

  int symTabNum2=findSymTab(sheader2,header2->e_shnum);
  int numSec2 = 0;
  char* strTab2;
  numSec2 = findStrTabSym(sheader2,*header2);
  strTab2 = malloc(sheader2[numSec2]->sh_size);
  strTab2 = createStrTab(sheader2, src2, numSec2);
  Elf32_Sym * symTable2[sheader2[symTabNum2]->sh_size/sizeof(Elf32_Sym)];
  readSymTab(src2, sheader2, symTabNum2, symTable2, strTab2);


  int symt1 = sheader1[symTabNum]->sh_size/sizeof(Elf32_Sym);
  int symt2 = sheader2[symTabNum2]->sh_size/sizeof(Elf32_Sym);
  char * strTabFin = malloc(sheader1[numSec]->sh_size + sheader2[numSec2]->sh_size);
  Elf32_Sym * symTableFin[sheader1[symTabNum]->sh_size/sizeof(Elf32_Sym) + sheader2[symTabNum2]->sh_size/sizeof(Elf32_Sym)];
  int nbSymboles=0;
  int tailleSTRTAB;
  int nouveauxIndices[symt2];
  int indicesFich1[symt1];
  int lastLocalSym = mergeSymAndStr(strTab1, strTab2, symTable1, symTable2,  strTabFin, symTableFin, symt1, symt2,&tailleSTRTAB,&nbSymboles, nouveauxIndices,indicesFich1);

  //Fusion des sections

  int compteurRel=nombre_reloc(*header2,sheader2);
  reloc *relTable[compteurRel];

  getRelTable(src2,*header2,sheader2,symTable2,strTab2,compteurRel,relTable );

  int compteurRel1=nombre_reloc(*header1,sheader1);
  reloc *relTable1[compteurRel1];

  getRelTable(src1,*header1,sheader1,symTable1,strTab1,compteurRel1,relTable1);


  Elf32_Shdr * sheaderFinTmp[header1->e_shnum + header2->e_shnum];

  int nombreDeSection=mergeSheader(sheader1, *header1, sheader2, *header2, sheaderFinTmp,nbSymboles,tailleSTRTAB, symTableFin, strTabFin, dest, src1, src2, strTabSection1, strTabSection2, nouveauxIndices ,relTable,compteurRel, indicesFich1 ,relTable1, compteurRel1, lastLocalSym);

  Elf32_Shdr * sheaderFin[nombreDeSection];
  for(int i=0; i<nombreDeSection;i++){
    sheaderFin[i]=sheaderFinTmp[i];
  }
  ecrireSheader(sheaderFin, dest, nombreDeSection);
  //Creation du ELFHeader
  Elf32_Ehdr headerFin = createElfHeader(sheaderFin, *header1, nombreDeSection);
  ecrireHeader(&headerFin, dest);

  /////////////FREE/////////////
  free(strTabSection1);
  free(strTabSection2);

  free(strTab1);
  free(strTab2);
  free(strTabFin);

  freeReloc (relTable,compteurRel);
  freeReloc (relTable1,compteurRel1);
  freeSHDR(sheader1);
  freeSHDR(sheader2);
  freeSHDR(sheaderFin);
  freeHeader(header1);
  freeHeader(header2);

  /////////CLOSE FILES////////
  fclose(src1);
  fclose(src2);
  fclose(dest);
}
