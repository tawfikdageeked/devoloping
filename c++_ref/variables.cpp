#include <iostream>

int main(){

    int x;
    x = 3;
    x = 4;

    std::cout << x;

}

/*
Variables

to create a variable you need to things

1- delaration

declaration is to ecplcitly tell the executer the name of the variable (x in our case) and the type of data it will holds (int or integer in our case)

2- assignment

assinnment is giving the the variable data to hold the data can be :

int --> stand  for integers

float --> numbers but to a certain limit (4 bytes i think i am not sure)

double --> numbers but to a limit bigger than floats (8 bytes i think not sure too)

std::string --> for strings like "Fuck you"

char --> for characters like 'A', 'B' etc.  (don't use double quotes)

bool --> true or false

there is more but i am lazy you fuck around to find out

you can add "const" in the decleration to make the variable non changalbe like "const int x"
*/