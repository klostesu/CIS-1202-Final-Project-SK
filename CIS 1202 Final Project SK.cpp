// CIS 1202 Final Project SK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Player.h"
#include <algorithm>  // For std::transform
using namespace std;

static vector<Player> readCSV(const string& filename) {
    vector<Player> players;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return players;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;  // Skip blank rows

        stringstream ss(line);
        string name, team;
        int age, gp, minutes;
        double boxOut, screenAssist, deflections, looseBalls, charges, contestedShots;

        if (!(getline(ss, name, ',') && getline(ss, team, ','))) {
            cerr << "Warning: Skipped row with missing name or team ? " << line << endl;
            continue;
        }

        // Try reading all numeric fields in one go
        if (!(ss >> age)) { cerr << "Warning: Skipped row (age fail) ? " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> gp)) { cerr << "Warning: Skipped row (gp fail) ? " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> minutes)) { cerr << "Warning: Skipped row (minutes fail) ? " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> boxOut)) { cerr << "Warning: Skipped row (boxOut fail) ? " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> screenAssist)) { cerr << "Warning: Skipped row (screenAssist fail) ? " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> deflections)) { cerr << "Warning: Skipped row (deflections fail) ? " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> looseBalls)) { cerr << "Warning: Skipped row (looseBalls fail) ? " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> charges)) { cerr << "Warning: Skipped row (charges fail) ? " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> contestedShots)) { cerr << "Warning: Skipped row (contestedShots fail) ? " << line << endl; continue; }

        players.emplace_back(name, team, age, gp, minutes,
            boxOut, screenAssist, deflections, looseBalls,
            charges, contestedShots);
    }

    return players;
}


int main() {
    cout << "Hello World" << endl;

    string filename = "nba_hustle_stats.csv";
    vector<Player> players = readCSV(filename);

    // Declare vectors **before** filling them
    vector<double> boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll;

    // Populate vectors with player stats
    for (const Player& player : players) {
        boxOutAll.push_back(player.getBoxOut());
        screenAssistAll.push_back(player.getScreenAssist());
        deflectionsAll.push_back(player.getDeflections());
        looseBallsAll.push_back(player.getLooseBalls());
        chargesAll.push_back(player.getCharges());
        contestedShotsAll.push_back(player.getContestedShots());
    }

    // Debug prints to check vector sizes
    cout << "Total Players Loaded: " << players.size() << endl;
    cout << "BoxOut Data Size: " << boxOutAll.size() << endl;
    cout << "Screen Assist Data Size: " << screenAssistAll.size() << endl;
    cout << "Deflections Data Size: " << deflectionsAll.size() << endl;
    cout << "Loose Balls Data Size: " << looseBallsAll.size() << endl;
    cout << "Charges Data Size: " << chargesAll.size() << endl;
    cout << "Contested shots Data Size: " << contestedShotsAll.size() << endl;

    // Calculate and display hustle index for each player
    for (const Player& player : players) {
        player.displayStats(boxOutAll, screenAssistAll,
            deflectionsAll, looseBallsAll,
            chargesAll, contestedShotsAll);
    }

    return 0;  // **Properly closes `main()`**
}
      
