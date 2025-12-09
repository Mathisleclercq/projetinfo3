#include <stdio.h>
#include <stdlib.h>

typedef struct AVL {
    char* identifiant;
    float valeur;
    int equilibre;
    struct AVL* gauche;
    struct AVL* droite;
} AVL;


AVL* creerNoeud(char* id, float val) {
    AVL* nouveau = (AVL*) malloc(sizeof(AVL));
    if (nouveau == NULL) {
        printf("Erreur allocation mémoire\n");
        exit(1);
    }
    nouveau->identifiant = id;
    nouveau->valeur = val;
    nouveau->equilibre = 0;
    nouveau->gauche = NULL;
    nouveau->droite = NULL;
    return n;
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


AVL* insertionAVL(AVL* a, char* id, float val, int* h) {
    if (a == NULL) {
        *h = 1;
        return creerNoeud(ident, val);
    }
    if (strcmp(ident, a->identifiant) < 0) {
        a->gauche = insertionAVL(a->gauche, id, val, h);
        *h = -*h;
    } else if (strcmp(ident, a->identifiant) > 0) {
        a->droite = insertionAVL(a->droite, id, val, h);
    } else {
        *h = 0;
       a->valeur = a->valeur + val;
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
void Ecrire_fichier(AVL* a,File* f){
    if(a!=NULL){    
        Ecrire_fichier(a->droite,f);
        fprintf(f,"%s;%f\n",a->identifiant,a->valeur);
        Ecrire_fichier(a->gauche,f);
    }
}
        

int main(){
    FILE *fichier = fopen("capaciter.txt", "r");
    if (fichier == NULL){
      return 1;
    }
AVL* a = NULL;
int h =0;
char id[50];
float val;
while(fscanf(fichier,"%s %d",id,&val)==2){
    insertionAVL(a,id,val,&h);
File* f = fopen("vol_max.dat", "w");
if(f == NULL){
    printf("erreur ouverture fichier");
    exit(1);
}
