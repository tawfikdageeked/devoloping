#include <iostream>


int main(){

    /*
    a switch is like an if statement but it compares one thing to multible things
    */

    int month;
    

    std::cout << "Enter a month (1 -> 12): ";

    std::cin >> month;

    switch(month){

        case 1:   // case is like ==
            std::cout << "It is Jenuary";
        break;

        case 2:
            std::cout << "It is February";
        break;
        // and so on

        default: // what gets executed if there is no matches
            std::cout << "Fuck You";
        
    }

}