#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<time.h>
#define MAX 1000 // taille maximale du bloc.
#define MAXT 100000

/********************************************************************************************************************************************/
/** declarations des structures et fonctions de la 1ere question 'construction fichier texte'*/
/*******/
//declaration des enregistrement contenant dans le fichier texte
 struct Tenreg
{
  long int cle;
  char sori; //I : insertion, S : suppression


} e;
//---------------------
//une fonction c_info qui permet de generer aleatoirement un charactere (I ou S)
char  c_info ()
    {
      char info;
      char caracteres[2] = "IS"; //caracteres est une variable qui contient
      srand ( time(0) );
      info = caracteres[rand() %2] ; //l'info' reçoit un caracteres I (insertion) ou S (suppression)
      return (info);
    }
//--------------------
//Construction d'un fichier texte avec 100000 enregistrements <cle,I/S>
void constr_ftexte(FILE* f , char nomFichier[30] )
{


   //variables
   long int i; //pour la boucle for (pour remplir le fichier)
   long int n;  // pour generer a chaque fois un nombre compris entre le nombre precedent et (le nombre precedent + un nombre qu'on va choisi)
   long int si=1;
  //DEBUT du programme
   printf("\nConstruction d'un fichier contenant 100000\n\n");
   // creation du nouveau fichier en mode texte
   f=fopen(nomFichier,"w");

   if ( f == NULL )
    {
        printf("erreur lors de l'ouverture du fichier %s en mode w\n", nomFichier);

    }
    else
    {
   // insertion des enregistrements generes aleatoirement
    n=1;
   for (i=1 ; i<=100000 ; i++)
   {

        e.cle=(rand()%(4))+n; //generer un nombre de n et n+4 , avec cette condition on peut assurer aue le fichier est ordonné (ie les clé sont ordonnées).
        n=e.cle+1; // (+1 pour la prochaine iteration ) pour assuere qu'on va repeter la cle car la cle doit etre unique (pour la prochaine iteration)
        if ((si % 3) == 0) {e.sori='S';}
        else {e.sori='I';}
        si++;
        //e.sori=c_info(); //generer l'info
       // l'écriture dans le fichier
       fprintf (f, " | %ld | %c \n", e.cle, e.sori); // un enregistrement par ligne

    }
    // fermeture du fichier
    fclose( f );
  }
}

//--------------------
void afficher_fichier(char nomf[30])
{

char buf[30];

    FILE *f = fopen(nomf, "r");
    rewind(f);
    if ( f == NULL )
    {
         printf("erreur lors de l'ouverture du fichier %s en mode r\n", nomf);

    }
    else
    {
       while(fgets(buf,sizeof(buf),f)!= NULL)
    {
        printf("%s",buf);
    }
    fclose(f);
    }

}
//----------------

/***************************************************************************/
/**|       Declaration de la structure de fichier TOF        |
 ****************************************************************************/
 /* Structure d'enregistrement */
typedef struct Tenreg_TOF
{
   int cle;
   int effac;
   char info[10];

} Tenreg_TOF;
/* Structure de bloc */
typedef struct Tbloc_TOF
{
    Tenreg_TOF tab[MAX+1];
    int nb; // nombre d'enregistrements remplits dans le bloc
} Tbloc_TOF;

/* Structure du buffer */
typedef struct Tbloc_TOF Buffer_TOF; // identique � la structure du bloc
/* Structure de l'ent�te du fichier */
typedef struct Entete_TOF
{

    int nbBloc; // Nombre de blcs dans le fichier

    int nbEnreg; //nombre d'enregistrements insérés dans le fichier TOF

    int nbSupp; //nombre d'enregistrements supprimés dans le fichier TOF

} Entete_TOF;
/*type de TOF*/
typedef struct TOF
{
    FILE *fichier;
    Entete_TOF entete;
} TOF;


/* machine abstraite TOF */

// fonction ouverture ou creation d'un fichier TOF.
TOF* ouvrir_TOF(char* nomFichier, char mode)
{
    TOF* f = malloc(sizeof(TOF));
    if((mode == 'A') || (mode == 'a'))
    {
        f->fichier = fopen(nomFichier, "rb+"); // ouvrir un fichier existant en mode binaire lecture/�criture
        if(f->fichier == NULL)
            return NULL;
        else
            fread(&(f->entete), sizeof(Entete_TOF), 1, f->fichier); // chargement de l'ent�te dans la structure
    }
    else if((mode == 'N') || (mode == 'n'))
    {
        f->fichier = fopen(nomFichier, "wb+"); // ouvrir un nouveau fichier en mode binaire lecture/�criture
        if(f->fichier == NULL)
            return NULL;
        else
        {

          (f->entete).nbSupp= 0; // initialisation de l'entête dans la structure

            (f->entete).nbEnreg= 0; // initialisation de l'entête dans la structure


            (f->entete).nbBloc = 0;

            fwrite(&(f->entete),sizeof(Entete_TOF),1,f->fichier);
        }
    } else return NULL;
    return f;
}


 /* Proc�dure fermeture d'un fichier TOF */

void fermer_TOF(TOF *f)
{
    rewind(f->fichier); // positionner le curseur au d�but du fichier.
    fwrite(&(f->entete), sizeof(Entete_TOF), 1, f->fichier); // sauvegarder l'ent�te au d�but du fichier
    fclose(f->fichier); // fermer le fichier
    free(f); // liberer l'espace m�moire occupe .
}


/*------------------------------------------------------------------------------*/
                /* Proc�dure de lecture d'un bloc methode TOF */

void lireDir_TOF(TOF *f, int Num_Bloc, Tbloc_TOF *buf)
{
   /* if(Num_Bloc <= (f->entete).nbBloc)
    {
        fseek(f->fichier, sizeof(Entete_TOF) + (Num_Bloc-1) * sizeof(Tbloc_TOF), SEEK_SET);
        fread(buf, 1, sizeof(Tbloc_TOF), f->fichier);
    }*/
    rewind(f->fichier);
    fseek(f->fichier, sizeof(Entete_TOF) + (Num_Bloc-1) * sizeof(Tbloc_TOF), SEEK_SET);
    fread(buf, sizeof(Tbloc_TOF),1, f->fichier);


}

/*------------------------------------------------------------------------------*/
                /* Proc�dure d'�criture d'un bloc m�thode TOF */

void ecrireDir_TOF(TOF *f, int Num_Bloc, Buffer_TOF *buf)
{
    /*if(Num_Bloc <= (f->entete).nbBloc)
    {
        fseek(f->fichier, sizeof(Entete_TOF) + (Num_Bloc-1) * sizeof(Tbloc_TOF), SEEK_SET);
        fwrite(buf, 1, sizeof(Tbloc_TOF), f->fichier);
    }*/
    rewind(f->fichier);
    fseek(f->fichier, sizeof(Entete_TOF) + (Num_Bloc-1) * sizeof(Tbloc_TOF), SEEK_SET);
    fwrite(buf, sizeof(Tbloc_TOF),1, f->fichier);
}

/*------------------------------------------------------------------------------*/
                /* Fonction de lecture de l'ent�te m�thode TOF */


int entete_TOF(TOF *f,int i)
{
    if(i == 1) return f->entete.nbBloc ;
    else if(i == 2) return f->entete.nbEnreg;
    else return f->entete.nbSupp;

}


/*------------------------------------------------------------------------------*/
              /* Proc�dure de modification de l'ent�te m�thode TOF */

/* lorsque i=1 ====> modification du nombre de blocs
   lorsque i=2 ====> modification du nombre d'enregistrement */
void aff_entete_TOF(TOF *f, int i, int val)
{

    if (i == 1)  {f->entete.nbBloc=val;}
    else if   (i == 2)      {f->entete.nbEnreg=val;}

    else {f->entete.nbSupp=val;}
}
/*------------------------------------------------------------------------------*/
                   /* allocation d'un nouveau bloc m�thode TOF */
int alloc_bloc_TOF(TOF *f)
{
    aff_entete_TOF(f,1,entete_TOF(f,1)+1);
    return entete_TOF(f,1);
}

/********************/
//une fonction ch_info qui permet de generer aleatoirement une chaine de charactere de longueure N (generer l'info)
char  *ch_info ( int N )
    {
      int k;
      char  * info = malloc(N+1);

      char caracteres[35] = "abcdefghijklmnopqrstuvwxyz123456789"; //caracteres est une variable qui contient les caractéres qu'on va les utiliser pour obtenir l'information'

      for (k=0;k<N; k++)
        {
           *(info+k) = caracteres[rand() %35] ; //a chaque fois l'info' reçoit un caracteres jusqu'on arrive a N (la langueur de chaine de caractéres)
        }
      info[k] =  '\0' ;//le charachtere NULL qui marque la fin de la chaine

      return (info);
    }
/*************************************************************************************************/

  void chargement_initial_TOF(FILE* ft,char nomft[30],TOF *f,  char nomFich[30], float u, int *cpt)//cpt c le nombre d'acces disques ECRITURES
{
    Tenreg_TOF e;
    Tbloc_TOF buf;
    char *infoo;
    int j,k;
    int cpt2=1;//compteur des insertions(au moins une insertion car le fichier texte contient 100000 enregistrement avec s ou i

    //debut chargement initial

    f=ouvrir_TOF(nomFich,'N');
    ft=fopen(nomft,"r");
     rewind(ft);
    long int i=1;

     j=1;
    *cpt=1  ;
     for( k=1; k<=100000; k++)
     {
       long int clee;

       char o;

    fscanf(ft," | %ld | %c \n",&clee,&o); //recuperer l'enregistrement dans le ficher texte

        if(o=='S')//si l'operation est supprimer dans lechamps 'sori' dans le fichier texte
        {
            cpt2 ++;
            e.cle=clee;//initialosation
            e.effac=0;
            printf("\n infooooooooo");
           infoo=ch_info(9);//generer aleatoirement l'i'info' comme chaine de caracteres
           printf("infoo = %s" , infoo);
            strcpy(e.info,infoo);
         if(j<=u*MAX) //si par ex u=0.5 on remplit les blocs jusqu'a b/2 enreg
           {
            buf.tab[j]=e; //je rempli mon bloc
             printf("\n|  cle: %d",e.cle);
            j++; //passer a l'autre position
           }
         else{ //le bloc est rempli je dois remplir un autre bloc
            // int i+1=alloc_bloc_TOF(f);
             buf.nb = j-1 ;
             *cpt = *cpt + 1;

             ecrireDir_TOF(f,i,&buf);

             buf.tab[1] = e;
             printf("\n|  cle : %d",e.cle);
             i++;
             j=2;
              } //fin else

        } //fin if operation='S'

            } //fin for
            fclose(ft);
            //a la fi;n le dernier buf chargé n'est pas enregistré sur disque
             //donc on doit ecriredir le buffer
        buf.nb=j-1;
        ecrireDir_TOF(f,i,&buf);

        aff_entete_TOF(f,1,*cpt); //mettre a jour l indice du nbre de bloc (nbBloc)
        aff_entete_TOF(f,2,cpt2); //mettre a jour lenbre d'enregistrements insérés dans le fichier
         aff_entete_TOF(f,3,0);//mettre a jour le nombre d'enregistrements supprimés dans le fichier;

        fermer_TOF(f);

        } //fin chargement initial
/*******************************************************************/
//affichage
void affichage (TOF *f,char nom_fich[30],int cpt)
{
    int i,j,k;
    Tbloc_TOF buf;
    f=ouvrir_TOF(nom_fich,'A');//ouvrir le fichier en mode ancien
    if(f->fichier!=NULL)//si le fichier existe
    {

printf("\t\t\t==============\n");
printf("\n  L'Affichage  \n");
printf("\t\t\t==============\n");
printf("\t\t+----------+----------+\n");
printf("\t\t|  La cle  | La Valeur|\n");
printf("\t\t+----------+----------+\n");

       //tant qu'on est pas arrivé à la fin de fichier
       i=1;
    for(k=1;k<=cpt;k++)
    {    printf("-------------------------------------------------------------------------------------------");
        lireDir_TOF(f,i,&buf);//lire le buf i


            for(j=1;j <= buf.nb ;j++)//parcourir tout le tableau
            {
                if(buf.tab[j].effac==0)

               {
                printf("\n  |   %d    |     %s    |\n",buf.tab[j].cle , buf.tab[j].info );
               }

            }

        i++;//aller au prochain bloc
          }
       }

    else{ printf("\n \t\t<<Le fichier est vide>>");}
    fermer_TOF(f);
}

/** module RECHERCHER Dans TOF****************************/
void Rech_DICHO_TOF(TOF *f, char nomFich[30], int c , int *trouv, int* i, int *j,int *acceslect)//acces lect nbre dacces disques LECTURES
 //acclct est le nombre de lectures
{

   f=ouvrir_TOF(nomFich,'A');

   //---declaration variables
   Tbloc_TOF Buf;
   int bs,bi,sup,inf;
   int stop;

   //----DEBUT
   if(entete_TOF(f,1)==0)//si le fichier est vide
   {
       *i=1;
       *j=1;
       *trouv=0;

   }
   else{
   bs=entete_TOF(f,1);//le dernier bloc
   bi =1;//le premier bloc .
   //boucle
   *trouv =0;
   stop = 0;
   *j=1;
   *i=1;
   while( bi<= bs && !(*trouv) && !stop)
   {
       *i = (bi + bs)/2; //prendre le bloc du milieu

       lireDir_TOF(f,*i,&Buf); //lecture du bloc i par buf
       *acceslect = *acceslect + 1;//incrementer nbre d'acces

       if((c >= Buf.tab[1].cle) && (c<= Buf.tab[Buf.nb].cle ))
       {
            //boucle recherche dicho interne dans le bloc i (dans buf)
            inf=1; sup=Buf.nb;
             while((inf <= sup) && !(*trouv) && (!stop)) //recherche interne
             {
               *j = (inf + sup)/2;
               if(c == Buf.tab[*j].cle)
               {
                   if (Buf.tab[*j].effac==0)  {*trouv=1;}
                   else {stop=1;}
               }
               else
               {
                    if (c < Buf.tab[*j].cle) sup= *j -1;
                    else inf = *j +1;
               }

             }//fin while

             if(inf > sup ){
                    *j = inf; //position de c dans le bloc
              }//fin de la recherche interne
             stop =1; //stop =VRAI
       }
       else
    {

        if (c < Buf.tab[1].cle)
           bs = *i - 1;
        else {
            bi = *i +1;
        }

    }

             }//Fin while

           if ( bi > bs){ //fin de la recherche externe
            *i = bi; // i le num du bloc ou doit se trouver c
            *j = 1;
           }
fermer_TOF(f);
   }//fin else
       } // FIN RECHERCHE DICHO TO
 /**insertion TOF*/

  void Inserer_TOF(TOF *f, char nomFich[30], Tenreg_TOF e,int *acclct, int *accecr , float*Fch)
//acclct est le nombre de lectures , accecr est le nombre d'ecritures et Fch est le facteur de chargement
  {
      /*VARIABLES*/
      Tbloc_TOF buf;
      int i,j,k,trouv,continu;
      int acceslect=0;
      Tenreg_TOF x;
      /**DEBUT INSERTION*/


      //*accecr=0;
      Rech_DICHO_TOF(f,nomFich,e.cle,&trouv,&i,&j,&acceslect);//il doit etre ins�r� dans
       *acclct = *acclct + acceslect;
      //le bloc i dans la position j

      if (trouv==1) printf("\n cle existe deja");
      else{
        f=ouvrir_TOF(nomFich,'A'); //ouvrir le fichier
       continu=1;
        while(continu==1 && i<=entete_TOF(f,1))
        {
            lireDir_TOF(f,i,&buf);
            *acclct = *acclct + 1;
             x = buf.tab[buf.nb]; //sauvegarder le dernier enregistrement
            //decalage interieur buffer
            k= buf.nb;
            while(k > j){
              buf.tab[j] = buf.tab[ k-1] ;
            k--;

                         }//fin while decalage

            buf.tab[j]=e; //insertion de e a la pos j dans buf
            if( buf.nb < MAX)//ya de l'espace dans le buffer
            {
                buf.nb ++;
                buf.tab[buf.nb] = x;
                ecrireDir_TOF(f,i,&buf);
                *accecr = *accecr + 1;
                continu=0;
            }
            else{ //buf plein insertion dans le bloc i+1a la pos 1
                ecrireDir_TOF(f,i,&buf);
                *accecr = *accecr + 1;

                i++;
                j=1;
                e = x;
            }
        }//fin while

        if( i > entete_TOF(f,1))//si je suis sortie de la boucle car j'ai inseré dans tous les blocs existants
        {
            buf.tab[1] = e;
            buf.nb = 1;

            ecrireDir_TOF(f,i,&buf);
            *accecr = *accecr + 1;

            aff_entete_TOF(f,1,i);//affecter bre de blocs dans le fichier
        }

         aff_entete_TOF(f,2, (entete_TOF(f,2)+1)) ;//incrementer compteur d'insertion d'enregistrements

      *Fch=(float) entete_TOF(f,2)/(MAX*entete_TOF(f,1));
      }//fin else non trouv
      fermer_TOF(f);
  } // fin insertion
  /**********************************************************************/

  /**suppression d'un enregistrement dans TOF*/
 void suppression(TOF *f, int c, char nomFich[30],TOF* f2, char nomFich2[30],int* acclct, int* accecr , float*Fch)
 //acclct est le nombre de lectures , accecr est le nombre d'ecritures et Fch est le facteur de chargement
 {
     Tbloc_TOF buf,buf2;
     int trouv,i,j;
     int acceslect=0;
     //debut
    //*accecr = 0;
     Rech_DICHO_TOF(f,nomFich,c,&trouv,&i,&j,&acceslect);
     *acclct = *acclct + acceslect; //le nombre de lectures recoit le nombre de lectures courant + le nombre de lecture de la recherche
     if(trouv==1)//si la clé existe dans le fichier
     {
        f=ouvrir_TOF(nomFich,'A');

        lireDir_TOF(f,i,&buf);
        *acclct = *acclct + 1; //on incremente le nombre de lecture

         buf.tab[j].effac = 1;//suppression logique effectuée

         ecrireDir_TOF(f,i,&buf);
         *accecr= *accecr +1;
         aff_entete_TOF(f,3,entete_TOF(f,3)+1);//decrementer compteurs d'enregistrements
         float seuil=(entete_TOF(f,3))/(entete_TOF(f,2));
         if( seuil > 0.5)//le seuil plus de 50% je fais une reorganisation au fichier TOF
         {
             f2=ouvrir_TOF(nomFich2,'N');
             i=1;
             int i2=1;
             int j2=1;//position dans le nv buffer
             int cpt=0;
             while(i<= entete_TOF(f,1))
             {
                 lireDir_TOF(f,i,&buf);
                 *acclct = *acclct + 1; //on incremente le nombre de lectures
                 j=1;
                 while(j<=buf.nb)
                 {
                     if(buf.tab[j].effac==1)
                     {
                         j++;
                     }
                     else{

                        if(j2<=MAX*0.5)
                        {
                            buf2.tab[j2]=buf.tab[j];
                            j++;
                            j2++;
                            cpt++;
                        }
                        else{
                                buf2.nb= j-1;

                                ecrireDir_TOF(f2,i2,&buf2);
                                *accecr = *accecr +1; //onincremente le  nombre d'ecritures
                                buf2.tab[1]=buf.tab[j];
                                j2=2;//incrementer la pos dans le nv buffer
                                j++;
                                i2++;
                                cpt++;

                        }
                     }
                 }//ftq
                 i++;//incrementer le num de blox dans le fichier TOF d'origine

             }//ftq
             buf2.nb= j2-1;//nombre d'enreg dans le buf2

             ecrireDir_TOF(f2,i2,&buf2);
             *accecr = *accecr +1;
             aff_entete_TOF(f2,1,i2);//initialisation nombre de blocs
             aff_entete_TOF(f,2,cpt);//initialisation nombre d'enreg supprimés
             aff_entete_TOF(f,3,0);//initialisation nombre d'enreg supprimés
            fermer_TOF(f2);
            rename(nomFich2,nomFich);
            *Fch= (float) entete_TOF(f,2)/(MAX*entete_TOF(f,1)); // on calcule le facteur de chargement qui est egale a nombre d'enregistrements inseres/(nombre du blocs * b)

         } // fin if

        fermer_TOF(f);
     } // fin if trouv



 } //fin suppression TOF









  /*******************************************************************/
  /** MODULES ET DECLARATION LOF****************/
/***/
/* Structure d'enregistrement */
typedef struct Tenreg_LOF
{
    int cle;
    char info[10];

} Tenreg_LOF;

/* Structure de bloc */
typedef struct Tbloc_LOF
{
    Tenreg_LOF tab[MAX+1];
    int nb; // nombre d'enregistrements remplits dans le bloc
    int suiv; //l'indice du bloc suivant
} Tbloc_LOF;

/* Structure du buffer */
typedef struct Tbloc_LOF Buffer_LOF; // identique à la structure du bloc
Buffer_LOF buf;

/* Structure de l'entête du fichier */
typedef struct Entete_LOF
{
    int no_prem_bloc; //l'indice du premier bloc de la liste
    int nbBloc; // Nombre de blocs dans le fichier
    int nbEnreg_inser; // Nombre d'enregistrements insérés dans le fichier LOF
    int nbEnreg_supp; //nombre d'enregistrements supprimés dans le fichier LOF
} Entete_LOF;

typedef struct LOF //structure du fichier
{
    FILE *fichier;
    Entete_LOF entete; //l'entete du fichier
} LOF;

/* machine abstraite LOF */

// fonction ouverture ou creation d'un fichier LOF.
// fonction ouverture ou creation d'un fichier LOF.
LOF *ouvrir_LOF(char *nomFichier, char mode)
{
    LOF *f = malloc(sizeof(LOF));
    if((mode == 'A') || (mode == 'a'))
    {
        f->fichier = fopen(nomFichier, "rb+"); // ouvrir un fichier existant en mode binaire lecture/écriture
        if(f->fichier == NULL)
            return NULL;
        else
            fread(&(f->entete), sizeof(Entete_LOF), 1, f->fichier); // chargement de l'entête dans la structure
    }
    else if((mode == 'N') || (mode == 'n'))
    {
        f->fichier = fopen(nomFichier, "wb+"); // ouvrir un nouveau fichier en mode binaire lecture/écriture
        if(f->fichier == NULL)
            return NULL;
        else
        {
            (f->entete).nbEnreg_inser= 0; // initialisation de l'entête dans la structure
            (f->entete).nbEnreg_supp = 0;
            (f->entete).nbBloc = 0;
            (f->entete).no_prem_bloc = 0;
            fwrite(&(f->entete),sizeof(Entete_LOF),1,f->fichier);
        }
    } else return NULL;
    return f;
}


 /* Procédure fermeture d'un fichier LOF */

void fermer_LOF(LOF *f)
{
    rewind(f->fichier); // positionner le curseur au début du fichier.
    fwrite(&(f->entete), sizeof(Entete_LOF), 1, f->fichier); // sauvegarder l'entête au début du fichier
    fclose(f->fichier); // fermer le fichier
    free(f); // libèrer l'espace mémoire occupé .
}


/*------------------------------------------------------------------------------*/
                /* Procédure de lecture d'un bloc methode LOF */

void lireDir_LOF(LOF *f, int Num_Bloc, Tbloc_LOF *buf)
{
    /*if(Num_Bloc <= f->entete.nbBloc)
    {
        fseek(f->fichier, sizeof(Entete_LOF) + (Num_Bloc-1) * sizeof(Tbloc_LOF), SEEK_SET);
        fread(buf, sizeof(Tbloc_LOF),1, f->fichier);

    }*/
    rewind(f->fichier);
    fseek(f->fichier, sizeof(Entete_LOF) + (Num_Bloc-1) * sizeof(Tbloc_LOF), SEEK_SET);
    fread(buf, sizeof(Tbloc_LOF),1, f->fichier);

}

/*------------------------------------------------------------------------------*/
                /* Procédure d'écriture d'un bloc méthode LOF */

void ecrireDir_LOF(LOF *f, int Num_Bloc, Tbloc_LOF *buf)
{
    /*if(Num_Bloc <= ((f->entete).nbBloc))
    {
        fseek(f->fichier, sizeof(Entete_LOF) + (Num_Bloc-1) * sizeof(Tbloc_LOF), SEEK_SET);
        fwrite(buf, sizeof(Tbloc_LOF),1, f->fichier);
    }*/
    rewind(f->fichier);
    fseek(f->fichier, sizeof(Entete_LOF) + (Num_Bloc-1) * sizeof(Tbloc_LOF), SEEK_SET);
        fwrite(buf, sizeof(Tbloc_LOF),1, f->fichier);

}

/*------------------------------------------------------------------------------*/
                /* Fonction de lecture de l'entête méthode LOF */


int entete_LOF(LOF *f,int i)
{
    if(i == 1) return f->entete.no_prem_bloc ;
    else if(i == 2) return f->entete.nbBloc;
    else {
        if (i==3) return f->entete.nbEnreg_inser;
        else return f->entete.nbEnreg_supp;
    }
}

/*------------------------------------------------------------------------------*/
   /*------------------------------------------------------------------------------*/
              /* Procédure de modification de l'entête méthode LOF */


void aff_entete_LOF(LOF *f, int i, int val)
{
    if(i == 1) {f->entete.no_prem_bloc=val;}
    else{
            if(i == 2) {f->entete.nbBloc=val;}
            else{
                if(i==3) {f->entete.nbEnreg_inser=val; }
                else {f->entete.nbEnreg_supp=val;}
                }
        }

}


/*------------------------------------------------------------------------------*/
                   /* allocation d'un nouveau bloc méthode LOF */
long int alloc_bloc_LOF(LOF *f)

{


    aff_entete_LOF(f,2,entete_LOF(f,2)+1);
    return entete_LOF(f,2);
}


/*********************************************************************************************/
/**MODULE CHARGEMENT INITIAL LOF*/
void chargement_initial_LOF(FILE *ft , char nomft[30],LOF *f ,char nomFich[30],float u, int *cpt)
{
    Tenreg_LOF e;
    Tbloc_LOF buf;

    int j,k;
    //int i; //pour parcourir le fichier texte
    int cpt2=0;//compteurs d'insertionq
    char *infoo;
    //debut chargement initial
    f=ouvrir_LOF(nomFich,'N');
    ft=fopen(nomft,"r");//ouvrir fichier texte
    rewind(ft);
    // i=1; //indice bloc
    long int i=alloc_bloc_LOF(f);

     aff_entete_LOF(f,1,i);//affecter num du premier bloc

     j=1;
    *cpt=1  ;
     for( k=1; k<=100000; k++)
     {
         long int clee;
         char sori;
         //parcourir le fichier texte
         fscanf(ft, " | %ld | %c \n",&clee,&sori); // un enregistrement par ligne

        if (sori=='S')//si l'operation est supprimer dans lechamps 'sori' dans le fichier texte
        {
            cpt2++;
            e.cle=clee;//affecter la cle
            infoo=ch_info(9);//generer aleatoirement l'i'info' comme chaine de caractere
         strcpy(e.info,infoo);

         if(j<=u*MAX) //si par ex u=0.5 on remplit les blocs jusqu'a b/2 enreg
           {

            buf.tab[j]=e; //je rempli mon bloc

            j++; //passer a l'autre position
         }
         else{ //le bloc est rempli je dois remplir un autre bloc
            int v=alloc_bloc_LOF(f);
             buf.nb = j-1 ;//nbre d'enreg dans le bloc i
             buf.suiv = v;//le chainage du bloci  avec son suivant v
             *cpt = *cpt + 1;
             //printf("\n CLE :%d\nINFO :%s",buf.tab[buf.nb].cle,&buf.tab[buf.nb].info);
             ecrireDir_LOF(f,i,&buf);
             buf.tab[1] = e;
             j=2;
             i=v;
             } //fin else
        }//fin if

        } //fin for
        fclose(ft);
        //a la fin le dernier buf chargé n'est pas enregistré sur disque
        //donc on doit ecriredir le buffer
        buf.nb=j-1;

        buf.suiv = -1; //suiv=-1 veut dire NULL


        ecrireDir_LOF(f,i,&buf);

        aff_entete_LOF(f,2,*cpt); //mettre a jour le nbre de blocs (nbBloc)
        aff_entete_LOF(f,3,cpt2); //mettre a jour lenbre d'enregistrements insérés dans le fichier
         aff_entete_LOF(f,4,0);//nbre d'enreg supp==0 initialement
        fermer_LOF(f);


        } //fin chargement initial

/******************************************************************************/


void affichage_LOF (LOF *f,char nom_fich[30],int n)
{int i,j;
Tbloc_LOF buf;
    f=ouvrir_LOF(nom_fich,'A');//ouvrir le fichier en mode ancien
    if(f->fichier!=NULL)//si le fichier existe
    {
        i=entete_LOF(f,1);//recupérer le numéro du premier bloc

    //n=(entete_LOF(f,3) - entete_LOF(f,4));//recupérer le nombre d'enregistrements


    if (n!=0)//si le fichier n'est pas vide
    {
        printf("\t\t\t==============\n");
printf("\n  L'Affichage  \n");
printf("\t\t\t==============\n");
printf("\t\t+----------+----------+\n");
printf("\t\t|  La cle  | La Valeur|\n");
printf("\t\t+----------+----------+\n");

       //tant qu'on est pas arrivé à la fin de fichier
    do {
        printf("------------------------------------------------------");
        lireDir_LOF(f,i,&buf);//lire le buf i

             //int k = buf.nb;

            for(j=1;j <= buf.nb ;j++)//parcourir tout le tableau
            {


                printf("\n  |   %d    |    %s     |\n",buf.tab[j].cle,buf.tab[j].info);
                printf("\n   +----------+----------+\n");






            }

        i=buf.suiv;//aller au prochain bloc
          }while(i != -1);
       }
    }
    else{ printf("\n \t\t<<Le fichier est vide>>");}
    fermer_LOF(f);
}



/**   Module qui recherche un enregistrement dans un fichie LOF*/
// i numero du bloc dans le quel se situe e , j sa position dans le bloc
//trouv =1 , rechcle trouvé dans f , sinon e n'existe pas
//la recherche dans LOF est séquentielle
void Recherche_LOF(char nomFich[30],int rechcle, int *trouv, int *i, int *j , int*prec , int *acclect)
{
//acclect est le nombre d'acces disques en lecture
 LOF *f;
 f=ouvrir_LOF(nomFich,'A');
 //declaaration
 Tbloc_LOF buf;
 int stop=0; //booleen pour l'arret de la recherche
 int bloc,pos;
 *trouv=0;//initialisation de trouv à 0;
 *prec=-1;
 //debut recherche LOF
 bloc=entete_LOF(f,1);//se positionner sur le premier bloc
 if(entete_LOF(f,1)!=0) //si le fichier n'est pas vide
 {

 //je commence le parcourt dans le fichier
 //tant qu'on a pas trouvé e et qu'on est pas à la fin du fichier stop=1
 do {

 lireDir_LOF(f,bloc,&buf); //lire le bloc courant indice "bloc"
 *acclect=*acclect+1; //incrementer les acces disques en lecture (le nombre de lectures)
 //je parcourt le buffer séquentiellement
 pos=1;//se positionner sur la premier position dans le bloc
 while((pos <= buf.nb) && (buf.tab[pos].cle <= rechcle) && (*trouv!=1) )//parcourir le buf

 {


 if (rechcle == buf.tab[pos].cle) //comparaison
 {
 *trouv=1; //trouvé , sortir de la grande boucle while
 }
 else pos++; //passer a la position prochaine puisque je ne l ai pas trouvé


 }//fin for
 if(*trouv!=1){

 if (pos>buf.nb) //la cle n existe pas(non trouv) et je suis sortie du buff
 {
 *prec=bloc;
 bloc = buf.suiv;

 }//passer au bloc prochain
 else //j'ai dépassé l ordre de la clé , c'est a dire buf.tab[pos].cle > rechecle
 {
 stop=1; //j'arrete la boucle
 }
 }
 } while((bloc != -1) && (*trouv!=1) && (stop!=1));


 *i=bloc;
 *j=pos;
 }//fin if
else {
 *i=1;
 *j=1;
}


fermer_LOF(f);

} // fin recherche




/************************************************************************************************/
void insertion1 (LOF *f,char nomFich[30] , Tenreg_LOF e , int *lect ,int *ect, float *seuil , float* Fch)
// S est le nombre maximale pour allouer un nouveau bloc , p est le pourcentage de chargement du bloc , dec est le nombre de decalages , ect est le nombre de d'ecritures et lect est le nombre de lectures supprimes.
{
     //DECLARATIONS
     int cle;
     int trouv,prec;
     int i , j , k;
     int arret;
     Tbloc_LOF buf;
     Tenreg_LOF temp;
     int m , n;
     int b=MAX; // b est le nombre max d'enregistrements dans un bloc
     //DEBUT INSERTION
      cle= e.cle;
      int accelect=0;
      Recherche_LOF(nomFich, cle , &trouv , &i , &j, &prec , &accelect);
      *lect=*lect+accelect;
      if (trouv==1)
      {
          printf("cle existe deja");
      }
      else // si non trouv
      {
          f = ouvrir_LOF(nomFich , 'A');
          arret = 0;
          lireDir_LOF(f,i,&buf);
          *lect=*lect+1;
          while ((i != -1) && (arret == 0))
          {

              if (buf.nb==b)
              {
                  temp=buf.tab[b]; // le bloc est saturé ,sauvegarder le dernier enregistrement du bloc dans temp
                  if ((buf.suiv ==-1) || (*seuil >= 0.5)) //cas du dernier bloc ou on a depasse le seuil
                  {
                      m=alloc_bloc_LOF(f); //allouer un nouveau bloc
                      aff_entete_LOF(f,2,entete_LOF(f,2)+1);//incrementer le nombre de blocs
                      k=b;
                      while (k>j) //decalage
                      {
                          buf.tab[k]=buf.tab[k-1];
                          k--;
                      }
                      buf.tab[j]=e; //insertioon
                      if (buf.suiv==-1) //cas du dernier bloc
                      {

                          buf.suiv=m; //le chainage
                          n=-1;
                      }
                      else // cas du depassement du seuil
                      {
                          n=buf.suiv; //garder le bloc suivant ( ancien avant decalage=
                         buf.suiv=m; //chainage  avec le bloc alloué

                      }
                      ecrireDir_LOF(f,i,&buf);
                      *ect=*ect+1;
                      buf.tab[1]=temp;
                      buf.nb=1;
                      buf.suiv=n;
                      ecrireDir_LOF(f,m,&buf);
                      *ect=*ect+1;
                      arret=1;


                  }
                  else
                  {
                      k=b;
                      while (k>j) //decalage
                      {
                          buf.tab[k]=buf.tab[k-1];
                          k--;
                      }
                      buf.tab[j]=e;
                      ecrireDir_LOF(f , i , &buf);
                      *ect=*ect+1;
                      i=buf.suiv;
                      lireDir_LOF(f,i,&buf);
                      *lect=*lect+1;
                      j=1;
                      e=temp;
                  }


              }
              else // 3eme cas(buf.nb < b)
              {
                  arret=1;
                  k=buf.nb+1;
                      while (k>j) //decalage
                      {
                          buf.tab[k]=buf.tab[k-1];
                          k--;
                      }

                      buf.tab[j]=e;
                      buf.nb=buf.nb+1;
                      ecrireDir_LOF(f,i,&buf);
                      *ect=*ect+1;
              }

          }
          if (i==-1)  //il existe deux cas
          {
              if (prec==-1) //ie fichier vide
              {
                  long int h=alloc_bloc_LOF(f);
                  lireDir_LOF(f,h,&buf);
                  *lect=*lect+1;
                  buf.tab[1]=e;
                  buf.nb=1;
                  buf.suiv=-1;
                  aff_entete_LOF(f,1,h);
                  aff_entete_LOF(f,2,1);
                  ecrireDir_LOF(f,h,&buf);
                  *ect=*ect+1;
              }
              else //prec!=-1 ie c'est le dernier bloc
              {
                  lireDir_LOF(f,prec,&buf);
                  *lect=*lect+1;
                  if(buf.nb<b) //le bloc precedent n'est pas plein , on peut inserer
                  {

                      buf.tab[buf.nb+1]=e;
                      buf.nb=buf.nb+1;
                      ecrireDir_LOF(f,prec,&buf);
                      *ect=*ect+1;


                  } else //buf.nb==b , il faut allouer un bloc pou inserer
                  {
                      int h=alloc_bloc_LOF(f);
                      buf.suiv=h;
                      ecrireDir_LOF(f,prec,&buf);
                      *ect=*ect+1;
                      lireDir_LOF(f,h,&buf);
                      *lect=*lect+1;
                      buf.tab[1]=e;
                      buf.nb=1;
                      buf.suiv=-1;
                      aff_entete_LOF(f,2,entete_LOF(f,2)+1);
                      ecrireDir_LOF(f,h,&buf);
                      *ect=*ect+1;
                  }
              }
          }

          aff_entete_LOF(f,3,entete_LOF(f,3)+1); //incrementer le nombre d'enregistrements inseres
          //calculer le nouveau seuil
                *seuil =(float) (entete_LOF(f,3)-entete_LOF(f,4)) / (MAX * entete_LOF(f,2));

            //calculer le facteur de chargement
                *Fch=(float)(entete_LOF(f,3))/(MAX* entete_LOF(f,2));

          //
          fermer_LOF(f);

      }

}
 /************************************************************************************************/
 void suppression1(LOF *f,char nomFich[30], int clesup , int *acclect , int *accect , float *fch )
 {
     //accelect est le nombre de lectures , accect est le nombre d'ecritures et Fch est le facteur de chargement
     int trouv,i,j,prec,proch;
     Tbloc_LOF buf,buf2;
     //supprimer physiquement et libérer le bloc vide
     int acceslect=0;
     Recherche_LOF(nomFich,clesup,&trouv,&i,&j,&prec,&acceslect);
     *acclect=*acclect+acceslect;
     if(trouv==1)
    {
     f=ouvrir_LOF(nomFich,'A');
     lireDir_LOF(f,i,&buf);
     *acclect=*acclect+1;
     if(buf.nb== 1) //si le bloc contient un seul enregsitrement qui est à supprimer
     {

       if(i==entete_LOF(f,1)) //le bloc a supprimer est le premier bloc dans le fichier
       {

           if(buf.suiv==-1){
                printf("\n\nLe fichier est devenu vide après la suppression effectuée!\n\n");
                aff_entete_LOF(f,1,0);
                aff_entete_LOF(f,2,0); //le nombre de blocs est 0 : fichier vide
           }
           else
           {
               aff_entete_LOF(f,1,buf.suiv); //modifier l'indice du premier bloc alors liberer le 1er bloc
               int a = entete_LOF(f,2) -1 ;
               aff_entete_LOF(f,2,a) ; //decrementer le nombre du blocs

           }
       }
       else if(i==entete_LOF(f,2)) //le bloc a supprimer est le dernier bloc du fichier
       {

         int tmp = entete_LOF(f,2) - 1;
         aff_entete_LOF(f,2,tmp); //modifier le nombre de blocs dans le fichier
         lireDir_LOF(f,prec,&buf2);
         *acclect=*acclect+1;
         buf.suiv=-1;  //modifier le suivant du precedent
         ecrireDir_LOF(f,prec,&buf2);
         *accect=*accect+1;

       }
        else { //le bloc a supprimer est au milieu
                proch=buf.suiv;
                lireDir_LOF(f,prec,&buf2); //mpdifier le chainage
                *acclect=*acclect+1;
                buf2.suiv=proch;
                ecrireDir_LOF(f,prec,&buf2);
                *accect=*accect+1;
        }
        }
        else{ //le bloc contient beacoup d'enregistrements
           int k=j; //se pointer dans l'enregistrement a supprimer
           while(k <= buf.nb)
           {
               buf.tab[k]=buf.tab[k+1]; //decalage vers le haut
                k++;
           }
            buf.nb = buf.nb - 1; //decrementer le nombre d'enregistrements dans le buffer+ traitement du cas du dernier enregsitrement

        }
        ecrireDir_LOF(f,i,&buf);
        *accect=*accect+1;
        *fch=(float)(entete_LOF(f,3))/(MAX* entete_LOF(f,2));
        aff_entete_LOF(f,4,entete_LOF(f,4)+1);
        fermer_LOF(f);
     }
     else printf(" ");//supression impsssible la clé existe deja

 }// fin suppression1
 /////////////////////////////////////////////////////////////////////
 /**********************************************************************************************/
 /************************* MODULE INSERTION 2 ***********************************/
/*************************   MODULE INSERTION 2 ***********************************/
void insertion2(LOF *f, char nomfich[30], Tenreg_LOF insere , int *accclect , int *accect , float *fch)
  {

//accclect est le nombre de lectures , accct est le nombre d'ecritures , fch est le facteur de chargement
     Tbloc_LOF buf,buf2;
     int i,j,prec,trouv,v,k;
     int acceslecture=0;
     Recherche_LOF(nomfich,insere.cle,&trouv,&i,&j,&prec,&acceslecture);//i,j ou on doit inserer insercle
     *accclect=*accclect+acceslecture; // le nombre de lectures recoit le nombre de lectures courant + le nombre de lecture de la recherche
     if(trouv==0)//si la clé a inserer n'existe  pas déja dans le fichier
     {
         f=ouvrir_LOF(nomfich,'A');

         if (i==-1) //il faut inserer a la fin de fichier
          {
              if (prec==-1) //ie fichier vide
              {
                   long int h=alloc_bloc_LOF(f);//alouer un bloc
                  lireDir_LOF(f,h,&buf);//lire le bloc
                  *accclect=*accclect+1;//incrementer le nombre de lectures
                  buf.tab[1]=insere;//inserer l'enregistrement dans le nouveau bloc
                  buf.nb=1;
                  buf.suiv=-1;
                  aff_entete_LOF(f,1,h);//modifier l'indice du premier bloc
                  aff_entete_LOF(f,2,1); //modifier le nombre du blocs
                  ecrireDir_LOF(f,h,&buf);
                  *accect=*accect+1;
              }
              else //prec!=-1 ie c'est le dernier bloc
              {
                  lireDir_LOF(f,prec,&buf);
                  *accclect=*accclect+1;
                  if(buf.nb<MAX) //le bloc precedent n'est pas plein , on peut inserer
                  {

                      buf.tab[buf.nb+1]=insere;
                      buf.nb=buf.nb+1;
                      ecrireDir_LOF(f,prec,&buf);
                      *accect=*accect+1;


                  } else //buf.nb==b , il faut allouer un bloc pou inserer
                  {
                      int h=alloc_bloc_LOF(f);//alouer un bloc lof
                      buf.suiv=h;//chainage
                      ecrireDir_LOF(f,prec,&buf);
                      *accect=*accect+1;
                      lireDir_LOF(f,h,&buf);
                      *accclect=*accclect+1;
                      buf.tab[1]=insere;//inserer
                      buf.nb=1;
                      buf.suiv=-1;
                      aff_entete_LOF(f,2,entete_LOF(f,2)+1);
                      ecrireDir_LOF(f,h,&buf);
                      *accect=*accect+1;
                  }
              }
          }
          else //i!=-1
          {
          lireDir_LOF(f,i,&buf);//lire le bloc ou la recherche est pointée
           *accclect=*accclect+1;
        // lireDir_LOF(f,buf.suiv,&buf2);//lire le bloc suivant
        //initialiser la position dans le nouveau bloc

             if(buf.nb==MAX)//si le bloc ou on doit inserer est plein
             {

                 v=alloc_bloc_LOF(f);

                 int tm=MAX*2/3+1;
                 lireDir_LOF(f,v,&buf2);
                 *accclect=*accclect+1;
                 int n=1;
                  for(k=tm; k <= MAX; k++)//remplir le nouveau bloc à partir
                  {

                     buf2.tab[n]=buf.tab[k]; //charger le nouveau bloc alloué
                     buf.nb= buf.nb -1;

                     n++;//incrementer la position dans le nouveau bloc


                  }//fin for(remplissage du bloc alloué a partir des enregsitrement du premier bloc)
                  buf2.nb = n-1 ;//affecter nbre d'enregistrements du bloc alloué

                    /*je teste ou est ce que on doit inserer 'insere' apres la redistribution*/
                    if(j <= MAX*2/3) //on doit inserer dans le bloc parent (buf)
                 {
                    k = MAX*2/3 ;
                    while(k >= j )
                    {
                        buf.tab[k+1]=buf.tab[k]; //decalage vers le bas pour inserer 'insere'
                        k--;

                    }
                    buf.tab[j]=insere;//effectuer l'insertion dans buf

                    buf.nb = buf.nb +1;//incrementer nb
                 }
                else //on doit inserer dans le nouveau bloc
                {
                    k=buf2.nb;

                    int nvpos =  j - (MAX*2/3); //calcul de la nouvelle position dans e nouveau bloc
                     while(k >= nvpos)
                    {
                        buf2.tab[k+1]=buf2.tab[k]; //decalage vers le bas pour inserer 'insere'
                        k--;
                    }
                    buf2.tab[nvpos]=insere ;//effectuer l'inserton dans buf
                    buf2.nb = buf2.nb +1 ;

                }
              int tmp=buf.suiv;
              buf.suiv=v;//chainer le nouveau bloc comme suivant du bloc principal
               buf2.suiv=tmp; //changer le chainage


               ecrireDir_LOF(f,i,&buf);//ecrire le buf parent apres traitement
               *accect=*accect+1;

               lireDir_LOF(f,tmp,&buf); //lire le bloc adjacent d'avant et verifier s'il est plein aussi pour reditribuer
               *accclect=*accclect+1;

               if(buf.nb==MAX)//le bloc adjacent d'avant est plein
                 {

                   for(k=1; k<=MAX/3 ; k++)//recuperer le premier tiers du bloc adjacent et l'inséerer dans nv blov.
                   {
                      buf2.tab[n+1]=buf.tab[k];
                    // buf.tab[k]=buf.tab[k+1];
                      n++;
                   }
                   buf2.nb= n;

                   /*decalage vers le haut dans le bloc adjacent*/
                   k=1;//debut du bloc adjacent
                   int k2=MAX/3+1  ;
                   while(k2<=MAX)//decalage vers le haut pour remplir les vides dans le bloc adjacent
                   {
                       buf.tab[k]=buf.tab[k2];//remplir les vides en haut ( les enregistrements pris pour le nv bloc)
                       k2++;
                       k++;

                   }
                  buf.nb = MAX*2/3 ; //nbre d'elements dans le bloc adjacent
                  ecrireDir_LOF(f,buf2.suiv,&buf);
                  *accect=*accect+1;

                  ecrireDir_LOF(f,v,&buf2);
                  *accect=*accect+1;
                }//si le adjacent n'est

                else {ecrireDir_LOF(f,v,&buf2);
                *accect=*accect+1;}//fin else
             }//fin if buf.nb==MAX

             else // le bloc parent n'est pas plein je peux effectuer l'insertion
                {//insertion normale

                    k= buf.nb;
                    while( k >= j) //decalage pour insertion
                    {
                        buf.tab[k+1]=buf.tab[k]; //decalage vers le bas
                        k--;
                    }
                    buf.tab[j]=insere;//inserer a la position j
                    buf.nb = buf.nb + 1;
                      ecrireDir_LOF(f,i,&buf);//ecrire le buf parent apres traitement
                      *accect=*accect+1;
                }

          }
            aff_entete_LOF(f,3,entete_LOF(f,3)+1); //incrementer le nombre d'enregistrements inseres
            *fch=(float)(entete_LOF(f,3))/(MAX* entete_LOF(f,2));
            fermer_LOF(f);

     }
     else{
        printf("\nInsertion2 impossible! la cle existe deja");
     }

 }
 /*********************************************************************************************/
 /** Suppression 2 */
 /*****************************************suppression2**********************************************/
void suppression2(LOF *f,char nomFich[30], int clesup , int *acclect , int *accect , float *fch )
 {
     //accelect est le nombre de lectures , accect est le nombre d'ecritures et Fch est le facteur de chargement
     int trouv,i,j,prec,proch;
     Tbloc_LOF buf,buf2;
     //supprimer physiquement et libérer le bloc vide
     int acceslect=0;
     Recherche_LOF(nomFich,clesup,&trouv,&i,&j,&prec,&acceslect);
     *acclect=*acclect+acceslect;
     if(trouv==1)
    {
     f=ouvrir_LOF(nomFich,'A');
     lireDir_LOF(f,i,&buf);
     *acclect=*acclect+1;
     if(buf.nb== 1) //si le bloc contient un seul enregsitrement qui est à supprimer , pas de redistrubution car on va liberer le bloc
     {

       if(i==entete_LOF(f,1)) //le bloc a supprimer est le premier bloc dans le fichier
       {

           if(buf.suiv==-1){
                printf("\n\nLe fichier est devenu vide après la suppression effectuée!\n\n");
                aff_entete_LOF(f,1,0);
                aff_entete_LOF(f,2,0); //le nombre de blocs est 0 : fichier vide
           }
           else
           {
               aff_entete_LOF(f,1,buf.suiv); //modifier l'indice du premier bloc alors liberer le 1er bloc
               int a = entete_LOF(f,2) -1 ;
               aff_entete_LOF(f,2,a) ; //decrementer le nombre du blocs

           }
       }
       else if(i==entete_LOF(f,2)) //le bloc a supprimer est le dernier bloc du fichier
       {

         int tmp = entete_LOF(f,2) - 1;
         aff_entete_LOF(f,2,tmp); //modifier le nombre de blocs dans le fichier
         lireDir_LOF(f,prec,&buf2);
         *acclect=*acclect+1;
         buf.suiv=-1;  //modifier le suivant du precedent
         ecrireDir_LOF(f,prec,&buf2);
         *accect=*accect+1;

       }
        else { //le bloc a supprimer est au milieu
                proch=buf.suiv;
                lireDir_LOF(f,prec,&buf2); //modifier le chainage
                *acclect=*acclect+1;
                buf2.suiv=proch;
                ecrireDir_LOF(f,prec,&buf2);
                *accect=*accect+1;
        }
        }
        else{ //le bloc contient beacoup d'enregistrements (plus que 1 enregistrement) et dans ce cas on peut faire la redistrubution
           int k=j; //se pointer dans l'enregistrement a supprimer
           while(k <= buf.nb) //decalage
           {
                buf.tab[k]=buf.tab[k+1]; //decalage vers le haut
                k++;
           }
            buf.nb = buf.nb - 1; //decrementer le nombre d'enregistrements dans le buffer + traitement du cas du dernier enregsitrement
            if (buf.nb<(2/3)* MAX) //il y a olus d'un tiers vide alors on peut faire la redistribution entre ce bloc et le bloc precedent si ce dernier est plein
            {     if (prec!= -1) //le bloc courant n'est pas le premier bloc
                {
                  lireDir_LOF(f,prec,&buf2);
                  *acclect=*acclect+1;
                  if (buf2.nb==MAX) // le bloc precedent est plein donc on fait la redistribution
                  {
                      //decalage vers le bas du bloc courant ( i )
                      int r = 1 ;
                      k = buf.nb +1;
                      while (r<=(1/3)*MAX)
                      {
                          buf.tab[k]=buf.tab[r];
                          k++;
                          r++;
                      }
                      buf.nb=k-1; //modifier le nombre d'enregistrement du bloc courant (i).
                      //decalage inter blocs (decaler un tiers du bloc precedent (prec) vers le bloc courant (i))
                       k = (2/3)*MAX +1;
                       r=1;
                       while (k<=buf2.nb)
                       {
                           buf.tab[r]=buf2.tab[k];
                           r++;
                           k++;
                       }
                       buf2.nb=(2/3)*MAX; //modifier le nombre d'enregistrements dans le bloc prec
                       ecrireDir_LOF(f,prec,&buf2);
                       *accect=*accect+1;
                       if (buf.nb<(2/3)*MAX) // il reste toujours de l'espace dans le bloc courant donc on peut faire le redistribution du bloc suivant si ce dernier est plein
                       {
                           if (buf.suiv!=-1)
                           {
                           lireDir_LOF(f,buf.suiv,&buf2); //lire le bloc suivant
                           acclect=*acclect+1;
                           if (buf2.nb==MAX) //le bloc "suivant" est plein , donc on fait la redistribution
                           {
                               r=1;
                               k=buf.nb+1;
                               //decalage du 1 er tiers du bloc suivant vers le bloc courant (i).
                               while (r<=(1/3)*MAX)
                               {
                                   buf.tab[k]=buf2.tab[r];
                                   k++;
                                   r++;
                               }
                               buf.nb=k-1; //modifier buf.nb (du bloc courant i)
                               //decalage vers le haut dans le bloc "suivant"
                               r=1;
                               k=(1/3)*MAX+1;
                               while (k<=buf2.nb)
                               {
                                   buf2.tab[r]=buf2.tab[k];
                                   k++;
                                   r++;
                               }
                               buf2.nb=r--; //modifier le nombre d'enregistrement du bloc "suiv"
                               ecrireDir_LOF(f,buf.suiv,&buf2);
                               *accect=*accect+1;


                           }
                        }

                       }


                  }
               }
            }
        }
        ecrireDir_LOF(f,i,&buf);
        *accect=*accect+1;
        //calculer le facteur de chargement
        *fch=(float) (entete_LOF(f,3))/(MAX* entete_LOF(f,2));
        aff_entete_LOF(f,4,entete_LOF(f,4)+1); //incrementer le nombre d'enregistrements supprimes
        fermer_LOF(f);
     }
     else printf(" ");//suppression impossible

 }// fin suppression2


/****************************************************************/
 /*************/
 /*************** PROTOTYPES FONCTIONS ***********************/
 void constr_ftexte(FILE* f , char nomFichier[30] );
 LOF *ouvrir_LOF(char *nomFichier, char mode);
 void lireDir_LOF(LOF *f, int Num_Bloc, Tbloc_LOF *buf);
 void ecrireDir_LOF(LOF *f, int Num_Bloc, Tbloc_LOF *buf);
 int entete_LOF(LOF *f,int i);
 void aff_entete_LOF(LOF *f, int i, int val);
 void chargement_initial_LOF(FILE *ft , char nomft[30],LOF *f ,char nomFich[30],float u, int *cpt);
void affichage_LOF (LOF *f,char nom_fich[30],int n);
 void Recherche_LOF(char nomFich[30],int rechcle, int *trouv, int *i, int *j, int* prec, int *accelect);
 void insertion1 (LOF *f,char nomFich[30] , Tenreg_LOF e , int *lect ,int *ect, float *seuil , float* Fch);
 void suppression1(LOF *f,char nomFich[30], int clesup,int *lct, int* ecr, float *fch);
//void chargement_initial_LOF(FILE *ft , char nomft[30],LOF *f ,char nomFich[30],float u, int *cpt)

/****************************************************************/
/** LE PROGRAMME PRINCIPALE*/

int main()
{
 //1ERE QUESTION
 int cpt;
 long int k=1;
 int acclct=0;
 int accecr=0;
 float fch=0;
 float p;

 long int clee;

 char o;// 's' ou 'i'

 FILE *fcsv;//declaration fichier csv
 FILE *ft;//fichier texte

 char nom[30];

 strcpy(nom,"ftexte.txt");
 constr_ftexte(ft,nom);
 afficher_fichier(nom);
 printf("\n\n    Vous voulez la combinaison:\n");
 printf("\n       1- insertion_suppression_TOF");
 printf("\n       2- insertion1_suppression1_LOF");
 printf("\n       3-insertion2_suppression1_LOF");
  printf("\n       4-insertion2_suppression2_LOF");
  printf("\n       5-insertion1_suppression2_LOF");
  printf("\n       6- QUITTER . ");
  int choix=0;

 /************************************************************************/
 /** partie tof**/
 //2e Question
 while(choix!=6)
 {
     printf("\nVeuillez inserer votre choix : ");
scanf("%d",&choix);

 if (choix==1){

accecr=0;
 accecr=0;//initialisations nombre d acces disques
 fch=0;

 TOF *f;
 TOF *f2;
 char nomfichier[30];
 char nomfichier2[30];


 //printf("\n\nEcrivez le nom du fichier TOF\n");
 //scanf("%s",nomfichier);
 strcpy(nomfichier,"fTOF");
 strcpy(nomfichier2,"nouveau");
 printf("\nDonner le poucentage de remplissage de blocs TOF\n");
 scanf("%f",&p);
 chargement_initial_TOF(ft,nom,f,nomfichier,p,&cpt);

 affichage(f,nomfichier,cpt);

 ft=fopen(nom,"r");//ouvrir fichier texte
 rewind(ft);
 char fichcsv[30];
 strcpy(fichcsv,"ftof.csv");

 Tenreg_TOF e;

 fcsv=fopen(fichcsv,"wb");

 while(k<= 100000)//parcourir tout le fichier texte
 {

 fscanf(ft," | %ld | %c \n", &clee,&o);
 e.cle=clee;
 strcpy(e.info,"inserer");

     if(o =='S')//une suppression a effectuer
     {
         suppression(f,e.cle,nomfichier,f2,nomfichier2,&acclct,&accecr,&fch);
         float pfch=fch*100;//pourcentage facteur de chargement
         fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);

     }
     else{//ine insertion a effectuer

       Inserer_TOF(f,nomfichier,e,&acclct,&accecr,&fch);
       float pfch=fch*100;//pourcentage facteur de chargement
       fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);
     }
k++;
 }

fclose(ft);
fclose(fcsv);
 }
/********************************************************************/
/**partie lof */
   else if (choix==2){
        /**INSERTION 1 SUPPREESSION 1 **/

 accecr=0;
 accecr=0;//initialisations nombre d acces disques
 fch=0;


 char nomFich[30];
 LOF *f2;
 int cptt;
 strcpy(nomFich,"inser1sup1");
 p=0;
 printf("\nDonner le poucentage de remplissage de blocs lOF\n");
 scanf("%f",&p);
 chargement_initial_LOF(ft,nom,f2,nomFich,p,&cptt);
// affichage_LOF(f2,nomFich,cpt);
   printf("apres chargement");
  ft=fopen(nom,"r");//ouvrir fichier texte
 rewind(ft);
 char fichcsv[30];
 strcpy(fichcsv,"ins1sup1.csv");
 Tenreg_LOF e;


 fcsv=fopen(fichcsv,"wb");



 while(k <= 100000)//parcourir tout le fichier texte
 {

 fscanf(ft," | %ld | %c \n", &clee,&o);
 e.cle=clee;
 strcpy(e.info,"inserer");

     if(o =='S')//une suppression a effectuer
     {

         suppression1(f2,nomFich,e.cle,&acclct,&accecr,&fch);
         float pfch=100*fch; //pourcentage facteur de chargement
         fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);

     }
     else{//ine insertion a effectuer
float seuil=0;


//       insertion1(ft,nom,e,&acclct,&accecr,&seuil,&cpt,&fch);
insertion1(f2,nomFich,e,&acclct,&accecr,&seuil,&fch);
        float pfch=100*fch;//pourcentage facteur de chargement
       fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);
     }
 k++;
 }
fclose(ft);
fclose(fcsv);



}//fin choix 2

else if(choix==3)
{

    accecr=0;
 accecr=0;//initialisations nombre d acces disques
 fch=0;

    char nomfich3[30];
 LOF *f3;
 int cptt3;
 strcpy(nomfich3,"inser2sup1");
 p=0;
 cpt=0;
 printf("\nDonner le poucentage de remplissage de blocs LOF\n");
 scanf("%f",&p);
 chargement_initial_LOF(ft,nom,f3,nomfich3,p,&cpt);
 printf("fin chargement");

  ft=fopen(nom,"r");//ouvrir fichier texte
 rewind(ft);
 char fichcsv[30];
 strcpy(fichcsv,"insr2sup1.csv");
 Tenreg_LOF e;


 fcsv=fopen(fichcsv,"wb");



 while(k <= 100000)//parcourir tout le fichier texte
 {

 fscanf(ft," | %ld | %c \n", &clee,&o);
 e.cle=clee;
 strcpy(e.info,"inserer");

     if(o =='S')//une suppression a effectuer
     {

         suppression1(f3,nomfich3,e.cle,&acclct,&accecr,&fch);
         float pfch=100*fch; //pourcentage facteur de chargement
         fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);

     }
     else{//ine insertion a effectuer

       insertion2(f3,nomfich3,e,&acclct,&accecr,&fch);
        float pfch=100*fch;//pourcentage facteur de chargement
       fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);
     }
 k++;
 }
fclose(ft);
fclose(fcsv);




}
else if(choix==4)//insertion2 supp2
{


 accecr=0;
 accecr=0;//initialisations nombre d acces disques
 fch=0;

    char nomfich4[30];
 LOF *f4;

 strcpy(nomfich4,"inser2sup2");
 p=0;
 printf("\nDonner le poucentage de remplissage de blocs LOF\n");
 scanf("%f",&p);
 chargement_initial_LOF(ft,nom,f4,nomfich4,p,&cpt);
 printf("fin chargement");

  ft=fopen(nom,"r");//ouvrir fichier texte
 rewind(ft);
 char fichcsv4[30];
 strcpy(fichcsv4,"insr2sup2.csv");
 Tenreg_LOF e;


 fcsv=fopen(fichcsv4,"wb");



 while(k <= 100000)//parcourir tout le fichier texte
 {

 fscanf(ft," | %ld | %c \n", &clee,&o);
 e.cle=clee;
 strcpy(e.info,"inserer");

     if(o =='S')//une suppression a effectuer
     {

         suppression2(f4,nomfich4,e.cle,&acclct,&accecr,&fch);
         float pfch=100*fch; //pourcentage facteur de chargement
         fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);

     }
     else{//ine insertion a effectuer

       insertion2(f4,nomfich4,e,&acclct,&accecr,&fch);
        float pfch=100*fch;//pourcentage facteur de chargement
       fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);
     }
 k++;
 }
fclose(ft);
fclose(fcsv);


}
else{//insertion1 suppression2

 accecr=0;
 accecr=0;//initialisations nombre d acces disques
 fch=0;

    char nomfich5[30];
 LOF *f5;
 cpt=0;
 strcpy(nomfich5,"inser1sup2");
 p=0;
 printf("\nDonner le poucentage de remplissage de blocs LOF\n");
 scanf("%f",&p);
 chargement_initial_LOF(ft,nom,f5,nomfich5,p,&cpt);
 printf("fin chargement");

  ft=fopen(nom,"r");//ouvrir fichier texte
 rewind(ft);
 char fichcsv5[30];
 strcpy(fichcsv5,"Insertion_1_Suppression_2.csv");
 Tenreg_LOF e;


 fcsv=fopen(fichcsv5,"wb");



 while(k <= 100000)//parcourir tout le fichier texte
 {

 fscanf(ft," | %ld | %c \n", &clee,&o);
 e.cle=clee;
 strcpy(e.info,"inserer");

     if(o =='S')//une suppression a effectuer
     {

         suppression2(f5,nomfich5,e.cle,&acclct,&accecr,&fch);
         float pfch=100*fch; //pourcentage facteur de chargement
         fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);

     }
     else{//ine insertion a effectuer
float seuil=0;
       insertion1(f5,nomfich5,e,&acclct,&accecr,&seuil,&fch);
        float pfch=100*fch;//pourcentage facteur de chargement (*100)
       fprintf(fcsv," %d %d %f\n",acclct,accecr,pfch);
     }
 k++;
 }
fclose(ft);
fclose(fcsv);


}

 }//fin while

 return 0;
}
