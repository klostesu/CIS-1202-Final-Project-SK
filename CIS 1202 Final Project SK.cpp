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

        getline(ss, name);
        ss >> team;
        ss.ignore(); // Skip comma
        ss >> age;
        ss.ignore(); // Skip comma
        ss >> gp;
        ss.ignore(); // Skip comma
        ss >> minutes;
        ss.ignore(); // Skip comma
        ss >> boxOut;
        ss.ignore(); // Skip comma
        ss >> screenAssist;
        ss.ignore(); // Skip comma
        ss >> deflections;
        ss.ignore(); // Skip comma
        ss >> looseBalls;
        ss.ignore(); // Skip comma
        ss >> charges;
        ss.ignore(); // Skip comma
        ss >> contestedShots;
        ss.ignore(); // Skip comma

        players.emplace_back(name, team, age,gp, minutes, 
            boxOut, screenAssist, deflections, looseBalls, charges, contestedShots);
    }

    return players;
}

int main() {
    string filename = "nba_hustle_stats.csv";
    vector<Player> players = readCSV(filename);

    // Now process the players using functions from Player.cpp
    for (Player& player : players) {
        player.displayStats();  // Call method from Player.cpp
    }

    return 0;
}
