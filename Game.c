#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Monstre
{
    int life;
    int pm;
    int pmMax;
    int degats;
    int oResistance;
    int resistance;
    int poison;
    int agro;
    int choix;
};

typedef struct Monstre monstre;

struct Perso
{
    int id;
    int life;
    int maxLife;
    int pm;
    int pmMax;
    int degat;
    int maxDegats;
    int degTimer;
    int xp;
    int niveau;
    int resistance;
    int choix;
};

typedef struct Perso perso;

struct Lieux
{
    char name[25];
    char description[100];
    int dif;
    monstre tMonstre[10];
};
typedef struct Lieux lieux;

struct Object
{
    int id;
    char name[21];
    int price;
};
typedef struct Object object;

void creationMonstre(lieux *lieu, int nbMonstre)
{
    for (int i = 0; i < nbMonstre; i++)
    {

        lieu->tMonstre[i].life = 100 * lieu->dif;

        lieu->tMonstre[i].pm = 5 * lieu->dif;

        lieu->tMonstre[i].pmMax = lieu->tMonstre[i].pm;

        lieu->tMonstre[i].degats = ((rand() % 11) + 10) * lieu->dif;

        if (lieu->tMonstre[i].degats > (15 * lieu->dif))
        {
            lieu->tMonstre[i].oResistance = 1;
        }
        else
        {
            lieu->tMonstre[i].oResistance = 2;
        }

        lieu->tMonstre[i].resistance = lieu->tMonstre[i].oResistance;
        lieu->tMonstre[i].poison = 0;
        lieu->tMonstre[i].agro = 0;
    }
}

void xpPerso(perso *perso)
{
    if (perso->xp == perso->niveau)
    {
        perso->maxLife += 10;
        perso->life = perso->maxLife;
        perso->niveau++;
        perso->xp = 0;

        //printf("xp : %d, niveau : %d, maxlife: %d, life : %d\n",perso->xp, perso->niveau, perso->maxLife, perso->life);
    }
}

int checkMonstreChoisi(monstre tMonstre[], int choix, int nbMonstre)
{

    //int tSize = (int)( sizeof(tMonstre) / sizeof(tMonstre[0]));
    if (choix < nbMonstre && choix >= 0 && tMonstre[choix].life > 0)
    {
        return 1;
    }
    return 0;
}

void sortInv(object tInventaire[])
{
    for (int i = 2; i > 0; i--)
    {

        for (int j = 0; j < i; j++)
        {

            if (strcmp(tInventaire[j].name, tInventaire[j + 1].name) > 0)
            {

                object copie;
                copie = tInventaire[j + 1];
                tInventaire[j + 1] = tInventaire[j];
                tInventaire[j] = copie;
            }
        }
    }
}

void view(object tableau[], int size, int *money)
{
    printf("Vous avez : %dAg\n", *money);
    for (int i = 0; i < size; i++)
    {
        if (tableau[i].price == 0)
        {
            printf("%d _ Vide\n", i);
        }
        else
        {
            printf("%d _ %s : %d\n", i, tableau[i].name, tableau[i].price);
        }
    }
    printf("\n");
}

// ne reconnait pas perso donc je laisse struct Perso mais reconnait monstre
void actionPerso(perso *perso, monstre tMonstre[], struct Perso tPerso[], int nbMonstre, int *comptMonstreMort, object tInventaire[], int *money)
{
    int monstreChoisi;
    int check = 0;
    int choix = 0;
    int select = 0;

    //affichage des monstres en vie
    for (int i = 0; i < nbMonstre; i++)
    {
        if (tMonstre[i].life > 0)
        {
            printf("Caracteristiques du monstre %d : %d pv, %d degats, %d resitance\n", i, tMonstre[i].life, tMonstre[i].degats, tMonstre[i].resistance);
        }
    }

    //choix de l'action
    while (check == 0)
    {
        printf("\nTape 1 pour attaquer, 2 pour l'action speciale, 3 pour te proteger ou 4 pour utiliser un objet.\n");
        scanf("%d", &perso->choix);

        //Verifie que le joueur ne donne pas un autre chiffre que 1, 2, 3 ou 4
        //Vérifie si le mage et l'archer ont assez de mana pour leurs actions spéciales
        if (perso->choix == 2 && perso->pm >= 5 && perso->id != 3)
        {
            check = 1;
        }
        else if (perso->choix == 2 && perso->pm < 5 && perso->id != 3)
        {
            printf("Vous n'avez plus assez de pm.\n");
        }
        //Vérifie qu'il y ai au moins un objet a utiliser
        if (perso->choix == 4)
        {
            int j = 0;
            for (int i = 0; i < 10; i++)
            {
                if (tInventaire[i].price == 0)
                {
                    j++;
                }
            }
            if (j == 10)
            {
                printf("Vous n'avez aucun objet dans l'inventaire.\n");
            }
            else
            {
                check = 1;
            }
        }
        //Vérifie les autres actions
        if (perso->choix == 3 || perso->choix == 1 || (perso->choix == 2 && perso->id == 3))
        {
            check = 1;
        }
    }
    //attaque
    if (perso->choix == 1)
    {
        while (choix == 0)
        {
            //printf("1");
            printf("Choisissez un monstre a attaquer.\n");
            scanf("%d", &monstreChoisi);

            choix = checkMonstreChoisi(tMonstre, monstreChoisi, nbMonstre);
        }

        tMonstre[monstreChoisi].life -= perso->degat / tMonstre[monstreChoisi].resistance;
        printf("Le monstre %d a subit %d degats.\n\n", monstreChoisi, perso->degat / tMonstre[monstreChoisi].resistance);

        if (tMonstre[monstreChoisi].life <= 0)
        {
            printf("Le monstre %d est mort.\n\n", monstreChoisi);
            *comptMonstreMort += 1;
            perso->xp++;
            xpPerso(perso);
        }
    }

    //resistance
    if (perso->choix == 3)
    {
        perso->resistance *= 2;
        printf("Vous subissez 2x moins de degats pendant se tour.\n\n");
    }

    //actions spéciales
    if (perso->choix == 2)
    {

        //mage
        //ajouter une protection
        if (perso->id == 1)
        {
            int i;
            printf("Choix du perso a soigner 1 mage, 2 archer ou 3 tank.\n");
            scanf("%d", &i);
            i--;
            tPerso[i].life += 20;
            perso->pm -= 5;
            if (tPerso[i].life > 100)
            {
                tPerso[i].life = 100;
            }
            printf("Vous avez %dPM, le perso %d a %dPV\n\n", perso->pm, i + 1, tPerso[i].life);
        }
        //archer
        if (perso->id == 2)
        {
            while (choix == 0)
            {
                printf("Choisissez un monstre a empoisonner.\n");
                scanf("%d", &monstreChoisi);

                choix = checkMonstreChoisi(tMonstre, monstreChoisi, nbMonstre);
            }
            tMonstre[monstreChoisi].life -= 10;
            tMonstre[monstreChoisi].poison = 2;
            perso->pm -= 5;
            if (tMonstre[monstreChoisi].life <= 0)
            {
                printf("Le monstre %d est mort.\n\n", monstreChoisi);
                *comptMonstreMort += 1;
                perso->xp++;
                xpPerso(perso);
            }
            else
            {
                printf("Vous avez %dPM, le monstre %d a %dPV\n\n", perso->pm, monstreChoisi, tMonstre[monstreChoisi].life);
            }
        }

        //tank
        if (perso->id == 3)
        {
            while (choix == 0)
            {
                printf("Choisissez un monstre a agro.\n");
                scanf("%d", &monstreChoisi);

                choix = checkMonstreChoisi(tMonstre, monstreChoisi, nbMonstre);
            }
            tMonstre[monstreChoisi].agro = 1;
            printf("Le tank a agro le monstre %d.\n\n", monstreChoisi);
        }
    }

    //inventaire
    if (perso->choix == 4)
    {
        view(tInventaire, 10, money);
        int ok = 0;
        while (ok == 0)
        {
            printf("Choisissez l'object a utiliser.\n");
            scanf("%d", &select);
            if (select <= 10 && select >= 0)
            {
                ok = 1;
            }
        }

        switch (tInventaire[select].id)
        {
        //mana +10
        case 1:
            perso->pm += 10;
            if (perso->pm > perso->pmMax)
            {
                perso->pm = perso->pmMax;
            }
            printf("Vous avez %dPM.\n", perso->pm);
            break;
        //life +20
        case 2:
            perso->life += 20;
            if (perso->life > perso->maxLife)
            {
                perso->life = perso->maxLife;
            }
            printf("Vous avez %dPV.\n", perso->life);
            break;
        //degats +10
        case 3:
            perso->degat += 10;
            perso->degTimer += 2;
            printf("Vous faites %d point de degat.\n", perso->degat);
            break;
        //mana +20
        case 4:
            perso->pm += 20;
            if (perso->pm > perso->pmMax)
            {
                perso->pm = perso->pmMax;
            }
            printf("Vous avez %dPM.\n", perso->pm);
            break;
        //life +50
        case 5:
            perso->life += 50;
            if (perso->life > perso->maxLife)
            {
                perso->life = perso->maxLife;
            }
            printf("Vous avez %dPV.\n", perso->life);
            break;
        //degats +20
        case 6:
            perso->degat += 20;
            perso->degTimer += 2;
            printf("Vous faites %d point de degat.\n", perso->degat);
            break;
        //mana +30
        case 7:
            perso->pm += 30;
            if (perso->pm > perso->pmMax)
            {
                perso->pm = perso->pmMax;
            }
            printf("Vous avez %dPM.\n", perso->pm);
            break;
        //life +100
        case 8:
            perso->life += 100;
            if (perso->life > perso->maxLife)
            {
                perso->life = perso->maxLife;
            }
            printf("Vous avez %dPV.\n", perso->life);
            break;
        //degats +50
        case 9:
            perso->degat += 50;
            perso->degTimer += 2;
            printf("Vous faites %d point de degat.\n", perso->degat);
            break;
        default:
            printf("erreur");
            break;
        }
        //retire de l'inventaire l'objet utilisé
        object vide = {0, "\0", 0};
        tInventaire[select] = vide;


    }
}

void actionMonstre(monstre tMonstre[], perso tPerso[], int nbMonstre)
{
    for (int i = 0; i < nbMonstre; i++)
    {
        if (tMonstre[i].life > 0)
        {
            if (tMonstre[i].agro == 1)
            {
                tPerso[2].life -= tMonstre[i].degats / tPerso[2].resistance;
                tMonstre[i].agro = 0;
                printf("Le monstre %d a attaquer le tank car il l'avait agro. Le tank a %d pv.\n", i, tPerso[2].life);
            }
            else
            {
                if (tMonstre[i].pm >= 5 && tMonstre[i].life < 90)
                {
                    //1 pour le soin, 2 pour l'attaque et 3 pour se protéger
                    tMonstre[i].choix = (rand() % 3) + 1; //entre 1 et 3
                }
                else
                {
                    //2 pour l'attaque et 3 pour se protéger
                    tMonstre[i].choix = (rand() % 2) + 2; // entre 2 et 3
                }

                if (tMonstre[i].choix == 1)
                {
                    tMonstre[i].life += 20;
                    if (tMonstre[i].life > 100)
                    {
                        tMonstre[i].life = 100;
                    }

                    tMonstre[i].pm -= 5;
                    printf("Le monstre %d se soigne, il a maintenant %d de points de vie\n", i, tMonstre[i].life);
                }

                if (tMonstre[i].choix == 2)
                {
                    int persoChoisi = (rand() % 3);
                    tPerso[persoChoisi].life -= tMonstre[i].degats / tPerso[persoChoisi].resistance;
                    printf("Le monstre %d attaque le perso %d, il a maintenant %d de points de vie.\n", i, persoChoisi + 1, tPerso[persoChoisi].life);
                }

                if (tMonstre[i].choix == 3)
                {
                    tMonstre[i].resistance *= 2;
                    printf("Le monstre %d resiste ! Les degats qu'il recoit sont divises par %d.\n", i, tMonstre[i].resistance);
                }
            }
        }
    }
}

int combat(perso tPerso[], lieux *lieu, int nbMonstre, object tInventaire[], int *money)
{

    printf("Vous etes arrive %s, ou il y a %s.\n", lieu->name, lieu->description);
    int comptMonstreMort = 0;

    while ((tPerso[0].life > 0 || tPerso[1].life > 0 || tPerso[2].life > 0) && comptMonstreMort < nbMonstre)
    {
        printf("\nNouveau tour\n");
        //avant que le joueur ne joue
        //regain d'un point de mana
        if (tPerso[0].pm < tPerso[0].pmMax)
        {
            tPerso[0].pm++;
        }
        if (tPerso[1].pm < tPerso[1].pmMax)
        {
            tPerso[1].pm++;
        }
        //effet de la potion de force
        for (int i = 0; i < 3; i++)
        {
            if (tPerso[i].degTimer > 0)
            {
                tPerso[i].degTimer--;
            }
            else
            {
                tPerso[i].degat = tPerso[i].maxDegats;
            }
        }

        //pour les monstres
        for (int i = 0; i < nbMonstre; i++)
        {

            //regain de mana
            if (lieu->tMonstre[i].pm < lieu->tMonstre[i].pmMax)
            {
                lieu->tMonstre[i].pm++;
            }
            //action du poison si necessaire
            if (lieu->tMonstre[i].poison > 0)
            {
                lieu->tMonstre[i].poison--;
                lieu->tMonstre[i].life -= 10;
                printf("\nLe monstre %d a perdu 10 PV avec le poison.\n", i);
            }
        }

        //tour de jeu pour chaque perso
        printf("\nVous jouez le mage :\nPV : %d\nPM : %d\nDegats :%d\nResistance :%d\nAction speciale : soin\n\n", tPerso[0].life, tPerso[0].pm, tPerso[0].degat, tPerso[0].resistance);
        actionPerso(&tPerso[0], lieu->tMonstre, tPerso, nbMonstre, &comptMonstreMort, tInventaire, money);
        printf("\nVous jouez l'archer :\nPV : %d\nPM : %d\nDegats :%d\nResistance :%d\nAction speciale : fleche empoisonnes\n\n", tPerso[1].life, tPerso[1].pm, tPerso[1].degat, tPerso[1].resistance);
        actionPerso(&tPerso[1], lieu->tMonstre, tPerso, nbMonstre, &comptMonstreMort, tInventaire, money);
        printf("\nVous jouez le tank :\nPV : %d\nPM : %d\nDegats :%d\nResistance :%d\nAction speciale : agro\n\n", tPerso[2].life, tPerso[2].pm, tPerso[2].degat, tPerso[2].resistance);
        actionPerso(&tPerso[2], lieu->tMonstre, tPerso, nbMonstre, &comptMonstreMort, tInventaire, money);

        //avant le tour des monstres
        for (int i = 0; i < nbMonstre; i++)
        {
            lieu->tMonstre[i].resistance = lieu->tMonstre[i].oResistance;
        }
        //tour des monstres
        actionMonstre(lieu->tMonstre, tPerso, nbMonstre);

        //Une fois le tour terminer
        tPerso[0].resistance = 1;
        tPerso[1].resistance = 1;
        tPerso[2].resistance = 2;

        //compte le nombre de monstres morts
        comptMonstreMort = 0;
        for (int i = 0; i < nbMonstre; i++)
        {
            if (lieu->tMonstre[i].life <= 0)
            {
                comptMonstreMort++;
            }
        }
        if (comptMonstreMort == nbMonstre)
        {
            printf("\nVous avez tue tous les monstres.\n");
        }
        else
        {
            printf("\nVous avez tue %d monstres au total.\n", comptMonstreMort);
        }
    }

    if (comptMonstreMort == nbMonstre)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main()
{

    int persoChoisi;
    int nbMonstre = 10;

    srand(time(NULL));

    //Creation Perso
    //id; life; maxLife; pm; pmMax; degat; maxDegats; degTimer; xp; niveau; resistance

    perso mage = {1, 100, 100, 20, 20, 2, 2, 0, 0, 1, 1};
    perso archer = {2, 100, 100, 5, 5, 20, 20, 0, 0, 1, 1};
    perso tank = {3, 100, 100, 0, 0, 15, 15, 0, 0, 1, 2};

    perso tPerso[3] = {mage, archer, tank};

    lieux lieu0 = {"au Pot de Chambre", "une foret tropical", 1};
    creationMonstre(&lieu0, nbMonstre);
    lieux lieu1 = {"au Mordor", "des montagnes", 2};
    creationMonstre(&lieu1, nbMonstre);
    lieux lieu2 = {"a Guenievre Blanche Fesse", "une plage de sable blanc", 3};
    creationMonstre(&lieu2, nbMonstre);
    lieux lieu3 = {"a L age de Pierre", "d'enormes grottes. Vous entendez quelqu'un crier present.", 4};
    creationMonstre(&lieu3, nbMonstre);
    lieux lieu4 = {"au Mont Cameroun", "un volcan en activitee", 5};
    creationMonstre(&lieu4, nbMonstre);
    lieux lieu5 = {"au Ranch ta Chambre", "un ranch", 6};
    creationMonstre(&lieu5, nbMonstre);

    lieux tLieux[6] = {lieu0, lieu1, lieu2, lieu3, lieu4, lieu5};

    object potMana = {1, "Potion de mana +10", 10};
    object potLife = {2, "Potion de soin +20", 10};
    object potStrength = {3, "Potion de force +10", 10};
    object potMana2 = {4, "Potion de mana +20", 20};
    object potLife2 = {5, "Potion de soin +50", 25};
    object potStrength2 = {6, "Potion de force +20", 20};
    object potMana3 = {7, "Potion de mana +30", 30};
    object potLife3 = {8, "Potion de soin +100", 50};
    object potStrength3 = {9, "Potion de force +50", 40};

    object vide = {0, "\0", 0};

    object tInventaire[10] = {potMana, potLife, potStrength, potMana2, potLife2, potStrength2, potMana3, potLife3, potStrength3, vide};
    //object tInventaire[10] = {vide, vide, vide, vide, vide, vide, vide, vide, vide, vide};

    int lieuAct = 0;
    int victory = 0;

    int money = 1000;

    //sortInv(tInventaire);
    //view(tInventaire, 10, &money);

    while (lieuAct <= 5)
    {
        victory = combat(tPerso, &tLieux[lieuAct], nbMonstre, tInventaire, &money);
        if (victory == 1)
        {
            lieuAct++;
            victory = 0;
            if (lieuAct == 6)
            {
                printf("Vous avez nettoyer le nomde de tout les monstres, bravo !\n");
                return 0;
            }
        }
        else
        {
            printf("Vous avez perdu. Vous allez recommencer le combat.\n");
        }
    }

    return 0;
}
