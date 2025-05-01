// CIS 1202 Final Project SK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Player.h"
using namespace std;

static vector<Player> readCSV(const::string& filename) {
    vector<Player> players;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string name, team;
        int age, gp, minutes;
        double boxOut, screenAssist, deflections, looseBalls, charges, contestedShots;

        getline(ss, name, ',');  // Properly extracts name up to the comma
        getline(ss, team, ',');  // Extracts the 3-letter team code correctly
        ss >> age; ss.ignore();
        ss >> gp; ss.ignore();
        ss >> minutes; ss.ignore();
        ss >> boxOut; ss.ignore();
        ss >> screenAssist; ss.ignore();
        ss >> deflections; ss.ignore();
        ss >> looseBalls; ss.ignore();
        ss >> charges; ss.ignore();
        ss >> contestedShots; ss.ignore();

        players.emplace_back(name, team, age,gp, minutes, 
            boxOut, screenAssist, deflections, looseBalls, charges, contestedShots);
    }

    return players;
}

int main()

{
    cout << "Hello World" << endl;

    string filename = "nba_hustle_stats.csv";
    vector<Player> players = readCSV(filename);

    // Now process the players using functions from Player.cpp
    for (Player& player : players) {
        player.displayStats();  // Call method from Player.cpp
    }

    return 0;
}
