#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure représentant un noeud du réseau
typedef struct Noeud {
    char *id;
    float fuite;    
    struct Enfant *enfants;     
} Noeud;

// Liste chaînée des enfants d’un noeud
typedef struct Enfant {
    struct Noeud *noeud;
    float fuite;
    struct Enfant *suivant;
} Enfant;

// Structure de l’arbre AVL
typedef struct AVL {
    Noeud *noeud;
    int equilibre;
    struct AVL *gauche;
    struct AVL *droite;
} AVL;

AVL *racineAVL = NULL;

// Fonctions utilitaires min/max
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

// Rotations pour équilibrer l’AVL
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

// Rééquilibrage après insertion
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

// Création d’un nouvel élément AVL
AVL* creerAVL(Noeud *n) {
    AVL* a = malloc(sizeof(AVL));
    if (!a) {
        printf("Erreur allocation mémoire\n");
        exit(1);
    }
    a->noeud = n;
    a->equilibre = 0;
    a->gauche = NULL;
    a->droite = NULL;
    return a;
}

// Insertion dans l’arbre AVL
AVL* insertionAVL(AVL* a, Noeud *n, int *h) {
    if (a == NULL) {
        *h = 1;
        return creerAVL(n);
    }

    if (strcmp(n->id, a->noeud->id) < 0) {
        a->gauche = insertionAVL(a->gauche, n, h);
        *h = -*h;
    }
    else if (strcmp(n->id, a->noeud->id) > 0) {
        a->droite = insertionAVL(a->droite, n, h);
    }
    else {
        *h = 0;
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

// Compte le nombre d’enfants d’un noeud
int compterEnfants(Noeud *n){
    int count = 0;
    for(Enfant *e = n->enfants; e; e = e->suivant)
        count++;
    return count;
}

// Recherche d’un noeud dans l’AVL
Noeud *chercherAVL(AVL *a, const char *id){
    if (a == NULL)
        return NULL;

    int c = strcmp(id, a->noeud->id);
    if (c == 0)
        return a->noeud;
    if (c < 0)
        return chercherAVL(a->gauche, id);
    return chercherAVL(a->droite, id);
}

// Récupère ou crée un noeud
Noeud *obtenirNoeud(const char *id){
    Noeud *n = chercherAVL(racineAVL, id);
    if (n != NULL)
        return n;

    n = malloc(sizeof(Noeud));
    n->id = strdup(id);
    n->fuite = 0;
    n->enfants = NULL;

    int h = 0;
    racineAVL = insertionAVL(racineAVL, n, &h);
    return n;
}

// Ajoute un lien parent → enfant avec fuite
void ajouterLien(const char *parent, const char *enfant, float fuite){
    Noeud *p = obtenirNoeud(parent);
    Noeud *e = obtenirNoeud(enfant);

    Enfant *c = malloc(sizeof(Enfant));
    c->noeud = e;
    c->fuite = fuite;
    c->suivant = p->enfants;
    p->enfants = c;
}

// Calcul récursif des pertes de volume
float calculFuite(Noeud *n, float volume) {
    if(n == NULL || volume <= 0)
        return 0;

    float pertesTotal = 0;
    int nbEnfants = compterEnfants(n);
    
    if(nbEnfants == 0)
        return 0;

    float volumeParEnfant = volume / nbEnfants;

    for(Enfant *c = n->enfants; c; c = c->suivant) {
        float perteTroncon = volumeParEnfant * (c->fuite / 100.0);
        float volumeRestant = volumeParEnfant - perteTroncon;
        
        pertesTotal += perteTroncon;
        pertesTotal += calculFuite(c->noeud, volumeRestant);
    }

    return pertesTotal;
}

// Libération mémoire
void libererEnfants(Enfant *e){
    Enfant *tmp;
    while(e){
        tmp = e;
        e = e->suivant;
        free(tmp);
    }
}

void libererNoeud(Noeud *n) {
    if (n == NULL)
        return;

    libererEnfants(n->enfants);
    free(n->id);
    free(n);
}

void libererAVL(AVL *a) {
    if (a == NULL) return;

    libererAVL(a->gauche);
    libererAVL(a->droite);

    libererNoeud(a->noeud);
    free(a);
}

int main(){
FILE* f = fopen("test.txt", "r");
	if(f == NULL){
	printf("erreur ouverture fichier");
	exit(1);
	}
	
   char W[200];
    float Z;
    char A[200], B[200];
    float fuite;
    fscanf(f, "%[^;];%f\n", W,&Z);

    while (fscanf(f, "%[^;];%[^;];%f\n", A, B, &fuite) == 3){
        ajouterLien(A, B, fuite);
       
    }
   
    
    Noeud *usine =  chercherAVL(racineAVL, W);
   
    float volumeInitial = Z;
    float pertes = calculFuite(usine, volumeInitial);
    FILE* f97 = fopen("vol_fuite.txt", "a");
    if (f97 == NULL) {
        printf("erreur ouverture fichier");
        exit(1);
    }
    fprintf(f97, "%s;%.2f\n", W, pertes);
libererAVL(racineAVL);
racineAVL = NULL;    
fclose(f);
fclose(f97);
return 0;

}
 
