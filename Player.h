#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
using namespace std;

class Player {
private:
    string name, team;
    int age, gp, minutes;
    double boxOut, screenAssist,deflections, looseBalls, charges, contestedShots;

public:
    // Constructor
    Player(string n, string t, int a, int g, int m, double bo, double sa, double de, 
        double lb, double ch, double cs);

    // Member function to compute hustle index
    double calculateHustleIndex();

    // Function to display player data
    void displayStats();
};

#endif
