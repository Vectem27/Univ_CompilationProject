int value;
int bornInf;
int bornSup;

bornInf = 10;
bornSup = 100;

if (bornInf < bornSup)
{
    value = bornInf + (rand() % (bornSup - bornInf));

    int valTest;
    do
    {
        print("Devine la valeur entre " bornInf " et " bornSup);
        read(valTest);

        if (valTest < value)
            print("Valeur trop petite !");
        else if (valTest > value)
            print("Valeur trop grande !");
    }while(valTest != value);

    print("Bravo, tu as trouve !!!");
    print("La valeur etait : " value);

}
else
    print("Erreur : La borne sup est inferieur ou egale a la borne inf");