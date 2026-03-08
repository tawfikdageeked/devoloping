//some were included for dubugging and aren't used in the program
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <iterator>

//some global shit
typedef std::vector<std::array<double, 2>> poi; // poi stands for points
double no_slope = std::nan("i can't code infinity :("); // some empty value
double epsilon = 1e-10; // comparison value cuz C++ is really dumb

//input function(return a string with a certain format)
std::string input()
{
    std::string points_string;
    bool is_valid = false;

    std::cout << '\n';
    std::cout << "IMPORTANT ----> enter points in this form : [1.0,2.0],[3.0,4.0], ...etc\ndon't use spaces or anything and make sure to type a coma after the last point\n";
    std::cout << '\n';
    std::cout << "This algorithm is made by noob C++ user trying to learn please help my ass and enter your data in the right format\n";
    std::cout << '\n';

    while (!is_valid) 
    {
        std::cout << "Enter Your Data: ";
        std::cin >> points_string;
        is_valid = true;

        for(char c : points_string)
        {
            if(c != '-' && !std::isdigit(c) && c != '.' && c != '[' && c != ']' && c != ',')
            {
                std::cout << "Wrong Format :( Please try again.\n";
                is_valid = false;
                break;
            }
        }
    }

    std::cout << '\n';
    return points_string;
}


// functions that convert the string from input to 2D points
/// FromStoP stands for from string to points and sofpoints stands for string of points
poi FromStoP(std::string sofpoints)
{

    poi points;
    std::vector<double> number_of_points;
    std::string number = "";
    std::array<double, 2> point;

    for(char c : sofpoints)
    { 

        if(c == '-' || std::isdigit(c) || c == '.')
        {
            number += c;
        }
        
        else if (c == ',')
        {
            number_of_points.push_back(std::stod(number));
            number = "";
        }

        else{continue;}

    }
    
    for(int i = 0; i < number_of_points.size(); i++)
    {
        if(i % 2 == 0)
        {
            point[0] = number_of_points[i];
        }

        else if (i % 2 != 0)
        {
            point[1] = number_of_points[i];
            points.push_back(point);
        }
    }

    return points;
}

// MnC if it is only one array that have one slope and one intercept value but mnc if it is a vecotor of arrays of many slopes and intercept values

// checks if this line exists already in the line vector
bool duplicant_checker(std::array<double, 2> MnC, poi mnc)
{
    for(int i = 0; i < mnc.size(); i++)
    {
        if(!std::isnan(MnC[0]) && !std::isnan(mnc[i][0]) && std::abs(MnC[1] - mnc[i][1]) < epsilon && std::abs(MnC[0] - mnc[i][0]) < epsilon)
        {
            return false;
        }

        else if(std::isnan(mnc[i][0]) && std::isnan(MnC[0]) && std::abs(MnC[1] - mnc[i][1]) < epsilon)
        {
            return false;
        }
        
        else{continue;}
    }
    
    return true;
}

/// eqofline stand for equation of line and it goes through all the points literally creating every possible line form the points provided
poi eqofline_generator(poi points)
{
    double dx;
    double dy;
    poi mnc;
    std::array<double, 2> MnC;

    for(int i = 0; i < points.size(); i++)
    {
        for(int l = i + 1; l < points.size(); l++)
        {
            dx = points[l][0] - points[i][0];
            dy = points[l][1] - points[i][1];

            if(dx != 0 && dy !=0)
            {
                double m = dy / dx;
                MnC[0] = m;
                double c = (points[l][1]) - (m * points[l][0]);
                MnC[1] = c;
            }

            else if (dy == 0)
            {
                double m = 0.0;
                MnC[0] = m;
                double c = (points[l][1]);
                MnC[1] = c;
            }

            else if (dx == 0)
            {
                double m = no_slope;
                double c = points[l][0];
                MnC[0] = m;
                MnC[1] = c;
            }

            if(duplicant_checker(MnC, mnc))
            {
                mnc.push_back(MnC);
            }
        }
    }
    
    return mnc;
}

// it stops at every line and checks literally every point on that line and count the matches
std::vector<int> validator(poi points, poi MnC)
{
    int counter = 0;
    std::vector<int> counters(MnC.size(), 0);

    for(int i = 0; i < MnC.size(); i++)
    {
        for(int l = 0; l < points.size(); l++)
        {
            if(!std::isnan(MnC[i][0]))
            {
                if(std::abs(points[l][1] - ((MnC[i][0] * points[l][0]) + MnC[i][1])) < epsilon)
                {
                    counter++;
                }
            }

            else
            {
                if(std::abs(points[l][0] - MnC[i][1]) < epsilon)
                {
                    counter++;
                }                
            }
        }

        counters[i] += counter;
        counter = 0;
    }

    return counters;
}



int main()
{
    std::string x = input();
    
    poi point = FromStoP(x);

    poi mnc = eqofline_generator(point);

    std::vector<int> counters = validator(point, mnc);

    auto it = std::max_element(counters.begin(), counters.end());
    int max_value = *it; 
    int max_index = std::distance(counters.begin(), it);

    std::cout << " ----------------------------- YOUR RESULT ----------------------------- \n";
    std::cout << '\n';
    
    if(!std::isnan(mnc[max_index][0]))
    {
        std::cout << "Your Winner Line Equations Is y = " << mnc[max_index][0] << "x + (" << mnc[max_index][1] << ")\n";
        std::cout << "And The Number Of Points That Fit On It = " << max_value << '\n';
    }
    else if (std::isnan(mnc[max_index][0]))
    {
        std::cout << "Your Winner Line Equations Is x = " << mnc[max_index][1] << '\n';
        std::cout << "And The Number Of Points That Fit On It = " << max_value << '\n';
    }
    
}