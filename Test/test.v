string testMessage;
testMessage = "Bonjour\n";
print("Message : " testMessage);

int test;
test = 6 + 2;
print("Should be 8");
print(test);

int testDivide;
testDivide = 12 / 2 / 2;
print("Should be 3");
print(testDivide);

int testModulo;
testModulo = 6 % 4;
print("Should be 2");
print(testModulo);


float test2;
test2 = (3.5 + 1.5) * 5 / 10;
print("Should be 2.5");
print(test2);

float testFloatLiteral;
testFloatLiteral = 3.5 + 0.25;
print("Should be 3.75");
print(testFloatLiteral);

bool testBool;
testBool = test2 <= testFloatLiteral;
print("Should be 1");
print(testBool);

print("Should be 1");
print(test == 8);

print("Should be 0");
print(1 == 2);

{
    int testBlock;
    testBlock = 25;
}

if (50 == 3)
    print("If fail");
else
{
    print("Else success");
    int test2;
    test2 = 20;
    print("Should be 20 : " test2);
}

int ctr;
ctr = 0;
while(ctr < 5)
    ctr = ctr + 1;

print("Should be 5\n" ctr);

do
{
    ctr = ctr + 1;
}
while(false);

print("Should be 6\n" ctr);