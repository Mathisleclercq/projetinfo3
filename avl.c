#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct AVL {
    char* identifiant;
    float valeur;
    float valeurreal;
    int equilibre;
    struct AVL* gauche;
    struct AVL* droite;
} AVL;


AVL* creerNoeud(char* id, float val,float valreal) {
    AVL* nouveau = (AVL*) malloc(sizeof(AVL));
    if (nouveau == NULL) {
        printf("Erreur allocation mémoire\n");
        exit(1);
    }
    nouveau->identifiant =  strdup(id);
    nouveau->valeur = val;
    nouveau->valeurreal=valreal;
    nouveau->equilibre = 0;
    nouveau->gauche = NULL;
    nouveau->droite = NULL;
    return nouveau;
}
int min(int a, int b) {
    if (a < b)
        return a;
    return b;
}

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int min3(int a, int b, int c) {
    return min(min(a, b), c);
}

int max3(int a, int b, int c) {
    return max(max(a, b), c);
}

AVL* rotationGauche(AVL* a) {
    AVL* pivot = a->droite;
    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;
    a->droite = pivot->gauche;
    pivot->gauche = a;
    a->equilibre = eq_a - max(eq_p, 0) - 1;
    pivot->equilibre = min3(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);
    return pivot;
}
AVL* rotationDroite(AVL* a) {
    AVL* pivot = a->gauche;
    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;
    a->gauche = pivot->droite;
    pivot->droite = a;
    a->equilibre = eq_a - min(eq_p, 0) + 1;
    pivot->equilibre = max3(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);
    return pivot;
}

AVL* doubleRotationGauche(AVL* a) {
    a->droite = rotationDroite(a->droite);
    return rotationGauche(a);
}

AVL* doubleRotationDroite(AVL* a) {
    a->gauche = rotationGauche(a->gauche);
    return rotationDroite(a);
}

AVL* equilibrerAVL(AVL* a) {
    if (a->equilibre >= 2) {
        if (a->droite->equilibre >= 0)
            return rotationGauche(a);
        else
            return doubleRotationGauche(a);
    }
    else if (a->equilibre <= -2) {
        if (a->gauche->equilibre <= 0)
            return rotationDroite(a);
        else
            return doubleRotationDroite(a);
    }
    return a;
}


AVL* insertionAVL(AVL* a, char* id, float val,float valreal, int* h) {
    if (a == NULL) {
        *h = 1;
        return creerNoeud(id, val, valreal);
    }
    if (strcmp(id, a->identifiant) < 0) {
        a->gauche = insertionAVL(a->gauche, id, val,valreal, h);
        *h = -*h;
    } else if (strcmp(id, a->identifiant) > 0) {
        a->droite = insertionAVL(a->droite, id, val,valreal, h);
    } else {
        *h = 0;
       a->valeur = a->valeur + val;
       a->valeurreal = a->valeurreal + valreal;
        return a; 
    }
    if (*h != 0) {
        a->equilibre += *h;
        a = equilibrerAVL(a);
        if (a->equilibre == 0)
            *h = 0;
        else
            *h = 1;
    }
    return a;
}

void Ecrire_fichier(AVL* a,FILE* f){
    if(a!=NULL){    
        Ecrire_fichier(a->droite,f);
        fprintf(f,"%s;%f\n",a->identifiant,a->valeur);
        Ecrire_fichier(a->gauche,f);
    }
}
void Ecrire_fichierR(AVL* a,FILE* f){
    if(a!=NULL){    
        Ecrire_fichierR(a->droite,f);
        fprintf(f,"%s;%f\n",a->identifiant,a->valeurreal);
        Ecrire_fichierR(a->gauche,f);
    }
}
int main(){
    FILE *capacite = fopen("sources_volumes.dat", "r");
    if (capacite == NULL){
    	printf("erreur ouverture");
      exit(1);
    }
    FILE *nom = fopen("plants_id.dat", "r");
    if (nom == NULL){
    	printf("erreur ouverture");
      exit(1);
    }
  FILE *Ffuite = fopen("sources_leaks.dat", "r");
    if (Ffuite == NULL){
    	printf("erreur ouverture");
      exit(1);
      }
  FILE* f = fopen("vol_captation.txt", "w");
	if(f == NULL){
	printf("erreur ouverture fichier");
	exit(1);
	}
FILE* f2 = fopen("vol_traitement.tmp", "w");
	if(f == NULL){
	printf("erreur ouverture fichier");
	exit(1);
	}
AVL* a = NULL;
int h =0;
char id[50];
float val;
float fuite;
float valreal;
fgets(id, 49, nom);
fprintf(f,"%s",id);
fgets(id, 49,capacite);
fprintf(f2,"%s",id);
fgets(id, 49, Ffuite);
while ( fscanf(capacite, "%f", &val) == 1 && fgets(id, 49, nom)!= NULL && fscanf(Ffuite, "%f", &fuite)==1) {
	id[strcspn(id, "\r\n")] = '\0';
	valreal = val * (1 - fuite/100);
	printf("%f\n",valreal);
	a = insertionAVL(a, id, val,valreal, &h);
}

Ecrire_fichierR(a,f2);
Ecrire_fichier(a,f);
fclose(f);
 fclose(nom);
 fclose(capacite);
return 0;
}
