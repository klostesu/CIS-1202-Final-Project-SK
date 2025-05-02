// CIS 1202 Final Project SK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Player.h"
#include "utils.h"      

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
            cerr << "Warning: Skipped row with missing name or team -> " << line << endl;
            continue;
        }

        if (!(ss >> age)) { cerr << "Warning: Skipped row (age fail) -> " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> gp)) { cerr << "Warning: Skipped row (gp fail) -> " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> minutes)) { cerr << "Warning: Skipped row (minutes fail) -> " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> boxOut)) { cerr << "Warning: Skipped row (boxOut fail) -> " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> screenAssist)) { cerr << "Warning: Skipped row (screenAssist fail) -> " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> deflections)) { cerr << "Warning: Skipped row (deflections fail) -> " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> looseBalls)) { cerr << "Warning: Skipped row (looseBalls fail) -> " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> charges)) { cerr << "Warning: Skipped row (charges fail) -> " << line << endl; continue; }
        ss.ignore();
        if (!(ss >> contestedShots)) { cerr << "Warning: Skipped row (contestedShots fail) -> " << line << endl; continue; }

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

    // Build vectors for each stat needed to compute each player's raw hustle index.
    vector<double> boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll;
    for (const Player& player : players) {
        boxOutAll.push_back(player.getBoxOut());
        screenAssistAll.push_back(player.getScreenAssist());
        deflectionsAll.push_back(player.getDeflections());
        looseBallsAll.push_back(player.getLooseBalls());
        chargesAll.push_back(player.getCharges());
        contestedShotsAll.push_back(player.getContestedShots());
    }

    // Compute raw hustle scores for all players.
    vector<double> rawScores;
    for (const Player& player : players) {
        double rawScore = player.calculateHustleIndex(
            boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll
        );
        rawScores.push_back(rawScore);
    }

    // Normalize the raw scores using our utils function.
    vector<double> normalizedScores = normalizeScores(rawScores);

    // Print out the raw score and the normalized score for each player.
    for (size_t i = 0; i < players.size(); i++) {
        cout << "Player: " << players[i].getName()
            << " | Raw Hustle Score: " << rawScores[i]
            << " | Normalized Hustle Score: " << normalizedScores[i] << endl;
    }

    return 0;
}
      
