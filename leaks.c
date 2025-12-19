#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Noeud {
    char *id;
    float fuite;    
    struct Enfant *enfants;     
} Noeud;

typedef struct Enfant {
    struct Noeud *noeud;
    float fuite;
    struct Enfant *suivant;
} Enfant;

typedef struct AVL {
    Noeud *noeud;
    int equilibre;
    struct AVL *gauche;
    struct AVL *droite;
} AVL;

AVL *racineAVL = NULL;


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


void ajouterLien(const char *parent, const char *enfant, float fuite){
    Noeud *p = obtenirNoeud(parent);
    Noeud *e = obtenirNoeud(enfant);

    Enfant *c = malloc(sizeof(Enfant));
    c->noeud = e;
    c->fuite = fuite;
    c->suivant = p->enfants;
    p->enfants = c;
}


float calculerFuites(Noeud *n, float volume) {
       if (n == NULL || volume <= 0)
        return 0;

    float pertes = 0;
    Enfant *e = n->enfants;

    while (e != NULL) {
        float volumeFuite = volume * e->fuite;
        pertes += volumeFuite;

        float volumeRestant = volume - volumeFuite;
        pertes += calculerFuites(e->noeud, volumeRestant);

        e = e->suivant;
    }

    return pertes;
}


int main(){
FILE* f = fopen("test.txt", "r");
	if(f == NULL){
	printf("erreur ouverture fichier");
	exit(1);
	}
	
    char A[200], B[200];
    float fuite;

    while (fscanf(f, "%[^;];%[^;];%f\n", A, B, &fuite) == 3){
        ajouterLien(A, B, fuite);
       printf("%s",A);
    }
    fclose(f);
    
    Noeud *usine = chercherAVL(racineAVL, A);
    if (!usine){
        printf("-1\n");
        return 1;
    }
    float volumeInitial = 239178.062500;
    float pertes = calculerFuites(usine, volumeInitial);
    FILE* f97 = fopen("vol_fuite.txt", "w");
    if (f97 == NULL) {
        printf("erreur ouverture fichier");
        exit(1);
    }
    fprintf(f97,"%s;%f\n",usine->id, pertes);
    return 0;
}
    float volumeInitial = 239178.062500;
    float pertes = calculerFuites(usine, volumeInitial);

    printf("%f\n", pertes);
    return 0;
}
