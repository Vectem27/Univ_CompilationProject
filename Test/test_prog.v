int value;
int bornInf;
int bornSup;

bornInf = 10;
bornSup = 100;

if (bornInf < bornSup)
{
    value = bornInf + (rand() % (bornSup - bornInf));
    print("Valeur a trouver : "value);
}
else
    print("Erreur : Borne sup inferieur ou égale à la borne inf");