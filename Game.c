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
    int soin;
    int poison;
    int timerAgro;
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
        printf("Votre personage a gagne un niveau !\n");
        int check = 0;
        int choix;
        while (check == 0)
        {
            if (perso->id == 3)
            {
                printf("Que voulez vous ameliorer ?\n1 _ Vie\n3 _ Degats\n4 _ Action Special\n");
            }
            else
            {
                printf("Que voulez vous ameliorer ?\n1 _ Vie\n2 _ Mana\n3 _ Degats\n4 _ Action Special\n");
            }
            scanf("%d", &choix);

            //Verification pour que le joueur ne donne pas un autre chiffre que 1, 2, 3 ou 4

            //Vérifie si le mage et l'archer ont assez de mana pour leurs actions spéciales
            if (choix >= 0 && choix <= 4 && perso->id != 3)
            {
                check = 1;
            }
            else if (choix >= 0 && choix <= 4 && perso->id == 3 && choix != 2)
            {
                check = 1;
            }
        }

        switch (choix)
        {
        //life
        case 1:
            perso->maxLife *= 1.1;
            perso->life = perso->maxLife;
            printf("La vie maximum du perso est de %d.\n\n", perso->maxLife);
            break;
        //mana
        case 2:
            perso->pmMax += 5;
            perso->pm = perso->pmMax;
            printf("Le mana maximum du perso est de %d.\n\n", perso->pmMax);
            break;
        //degats
        case 3:
            perso->maxDegats *= 1.5;
            perso->degat *= 1.5; //multiplier et pas faire =maxDegtas pour ne pas annuler l'éventuel effet d'une potion de force
            if (perso->degat == perso->maxDegats)
            {
                printf("Les degats du perso sont de %d.\n\n", perso->maxDegats);
            }
            else
            {
                printf("Les degats du perso sont de %d avec la potion et de %d sans.\n", perso->degat, perso->maxDegats);
            }
            break;
        //Special
        case 4:
            if (perso->id == 1)
            {
                perso->soin *= 2;
                printf("Le mage soigne maintenant de %dPV.\n", perso->soin);
            }
            if (perso->id == 2)
            {
                perso->poison *= 2;
                printf("L'archer fait maintenant %d degats d'empoisonnement.\n", perso->poison);
            }
            if (perso->id == 3)
            {
                perso->timerAgro += 1;
                printf("Maintenant, le tank agro un mob pendant %d tour(s).\n", perso->timerAgro);
            }
            break;
        default:
            printf("erreur xp");
            break;
        }

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

void sortInv(object tableau[], int size)
{
    // i = taille du tab - 1
    for (int i = size - 1; i > 0; i--)
    {

        for (int j = 0; j < i; j++)
        {

            if (strcmp(tableau[j].name, tableau[j + 1].name) > 0)
            {

                object copie;
                copie = tableau[j + 1];
                tableau[j + 1] = tableau[j];
                tableau[j] = copie;
            }
        }
    }
}

void view(object tableau[], int size)
{
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

// ne reconnait pas perso donc je laisse struct Perso tPerso[]
void actionPerso(perso *perso, monstre tMonstre[], struct Perso tPerso[], int nbMonstre, object tInventaire[], int *money)
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

        //Verification pour que le joueur ne donne pas un autre chiffre que 1, 2, 3 ou 4

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
            printf("Le monstre %d est mort. Vous gagnez 5Ag.\n\n", monstreChoisi);
            perso->xp++;
            *money += 5;
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
        if (perso->id == 1)
        {
            int i;
            printf("Choix du perso a soigner(%dPV) 1 mage, 2 archer ou 3 tank.\n", perso->soin);
            scanf("%d", &i);
            i--;
            tPerso[i].life += perso->soin;
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
            tMonstre[monstreChoisi].life -= perso->poison;
            tMonstre[monstreChoisi].poison = 2;
            perso->pm -= 5;
            if (tMonstre[monstreChoisi].life <= 0)
            {
                printf("Le monstre %d est mort. Vous gagnez 5Ag.\n\n", monstreChoisi);
                perso->xp++;
                *money += 5;
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
            tMonstre[monstreChoisi].agro = perso->timerAgro;
            printf("Le tank a agro le monstre %d pour %d tour(s).\n\n", monstreChoisi, perso->timerAgro);
        }
    }

    //inventaire
    if (perso->choix == 4)
    {
        sortInv(tInventaire, 10);
        view(tInventaire, 10);
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
            printf("erreur inventaire");
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
            if (tMonstre[i].agro > 0)
            {
                tPerso[2].life -= tMonstre[i].degats / tPerso[2].resistance;
                tMonstre[i].agro--;
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

int monstreMort(lieux *lieu, int nbMonstre)
{
    int comptMonstreMort = 0;
    for (int i = 0; i < nbMonstre; i++)
    {
        if (lieu->tMonstre[i].life <= 0)
        {
            comptMonstreMort++;
        }
    }
    return comptMonstreMort;
}

int combat(perso tPerso[], lieux *lieu, int nbMonstre, object tInventaire[], int *money)
{

    printf("\nVous etes arrive %s, ou il y a %s.\n", lieu->name, lieu->description);
    int comptMonstreMort = 0;

    while (tPerso[0].life > 0 && tPerso[1].life > 0 && tPerso[2].life > 0 && comptMonstreMort < nbMonstre)
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
                lieu->tMonstre[i].life -= tPerso[1].poison;
                printf("\nLe monstre %d a perdu %d PV avec le poison.\n", i, tPerso[1].poison);
                if (lieu->tMonstre[i].life <= 0)
                {
                    printf("Il est mort. Vous gagnez 5Ag.\n\n");
                    tPerso[1].xp++;
                    *money += 5;
                    xpPerso(&tPerso[1]);
                    comptMonstreMort = monstreMort(lieu, 10);
                }
            }
        }

        //tour de jeu pour chaque perso
        if (comptMonstreMort != nbMonstre)
        {
            printf("\nVous jouez le mage :\nPV : %d\nPM : %d\nDegats : %d\nResistance : %d\nAction speciale : soin\n\n", tPerso[0].life, tPerso[0].pm, tPerso[0].degat, tPerso[0].resistance);
            actionPerso(&tPerso[0], lieu->tMonstre, tPerso, nbMonstre, tInventaire, money);
        }
        comptMonstreMort = monstreMort(lieu, 10);
        if (comptMonstreMort != nbMonstre)
        {
            printf("\nVous jouez l'archer :\nPV : %d\nPM : %d\nDegats : %d\nResistance : %d\nAction speciale : fleche empoisonnes\n\n", tPerso[1].life, tPerso[1].pm, tPerso[1].degat, tPerso[1].resistance);
            actionPerso(&tPerso[1], lieu->tMonstre, tPerso, nbMonstre, tInventaire, money);
        }
        comptMonstreMort = monstreMort(lieu, 10);
        if (comptMonstreMort != nbMonstre)
        {
            printf("\nVous jouez le tank :\nPV : %d\nPM : %d\nDegats : %d\nResistance : %d\nAction speciale : agro\n\n", tPerso[2].life, tPerso[2].pm, tPerso[2].degat, tPerso[2].resistance);
            actionPerso(&tPerso[2], lieu->tMonstre, tPerso, nbMonstre, tInventaire, money);
        }

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
        comptMonstreMort = monstreMort(lieu, 10);

        if (comptMonstreMort == nbMonstre)
        {
            printf("\nVous avez tue tous les monstres.\n");
        }
        else
        {
            printf("\nVous avez tue %d monstres sur 10.\n", comptMonstreMort);
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

void marchand(object tSeller[], object tStock[], object tInventaire[], int *money)
{
    sortInv(tInventaire, 10);
    view(tInventaire, 10);

    printf("\nVous avez %dAg.\n", *money);

    //rempli l'inventaire du marchand
    for (int i = 0; i < 5; i++)
    {
        int j = (rand() % 9);
        tSeller[i] = tStock[j];
    }

    sortInv(tSeller, 5);
    view(tSeller, 5);

    char choix[4];
    int verif = 0;

    //trouve un emplacement vide dans l'inventaire
    int i = 0;
    while (tInventaire[i].price != 0 && i <= 9)
    {
        i++;
    }
    if (i > 9)
    {
        printf("Votre inventaire est plein. Vous ne pouvez rien acheter.\n");
        verif = 1;
    }

    printf("Voulez vous acheter ?(oui ou non)\n");
    
    while (verif == 0)
    {
        scanf("%s", choix);
        if (strcmp(choix, "oui") == 0)
        {
            int ok = 0;
            int select;
            while (ok == 0)
            {
                printf("Choisissez l'object a acheter.\n");
                scanf("%d", &select);
                if (select < 5 && select >= 0)
                {
                    ok = 1;
                }
                fflush(stdin);
                
            }

            *money -= tSeller[select].price;
            if (*money < 0)
            {
                *money += tSeller[select].price;
                printf("Vous n'avez pas asser d'argent. Vous avez %dAg.\n", *money);
                int check = 0;
                while (check == 0)
                {
                    printf("Voulez vous acheter autre chose ?(oui ou non)\n");
                    scanf("%s", choix);

                    if (strcmp(choix, "oui") == 0)
                    {
                        check = 1;
                    }
                    else if (strcmp(choix, "non") == 0)
                    {
                        check = 1;
                        verif = 1;
                    }
                    else
                    {
                        printf("Reponse par oui ou non.\n");
                    }
                }
            }
            else if (*money >= 0)
            {

                tInventaire[i] = tSeller[select];
                tSeller[select] = tStock[9];

                printf("Vous avez achetez %s.\n", tInventaire[i].name);
                printf("Il vous reste %dAg\n", *money);

                sortInv(tSeller, 5);
                view(tSeller, 5);

                int check = 0;
                while (check == 0)
                {
                    printf("Voulez vous acheter autre chose ?(oui ou non)\n");
                    scanf("%s", choix);

                    if (strcmp(choix, "oui") == 0)
                    {
                        check = 1;
                    }
                    else if (strcmp(choix, "non") == 0)
                    {
                        check = 1;
                        verif = 1;
                    }
                    else
                    {
                        printf("Reponse par oui ou non.\n");
                    }
                }
            }
        }
        else if (strcmp(choix, "non") == 0)
        {
            verif = 1;
        }
        else
        {
            printf("Reponse par oui ou non.\n");
        }
    }
}

int main()
{

    int persoChoisi;
    int nbMonstre = 10;

    srand(time(NULL));

    //Creation Perso
    //id; life; maxLife; pm; pmMax; degat; maxDegats; degTimer; xp; niveau; resistance; soin; poison; timerAgro;

    perso mage = {1, 100, 100, 20, 20, 2, 2, 0, 0, 1, 1, 20, 0, 0};
    perso archer = {2, 100, 100, 5, 5, 20, 20, 0, 0, 1, 1, 0, 10, 0};
    perso tank = {3, 100, 100, 0, 0, 15, 15, 0, 0, 1, 2, 0, 0, 1};

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

    object vide = {0, "z", 0};

    object tStock[10] = {potMana, potLife, potStrength, potMana2, potLife2, potStrength2, potMana3, potLife3, potStrength3, vide};
    object tInventaire[10] = {vide, vide, vide, vide, vide, vide, vide, vide, vide, vide};
    object tSeller[5] = {vide, vide, vide, vide, vide};

    int lieuAct = 0;
    int victory = 0;

    int money = 30;

    while (lieuAct <= 5)
    {
        marchand(tSeller, tStock, tInventaire, &money);
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
            printf("Vous avez perdu. Vous allez passer chez le marchand et recommencer le combat.\n");
        }
    }

    return 0;
}
