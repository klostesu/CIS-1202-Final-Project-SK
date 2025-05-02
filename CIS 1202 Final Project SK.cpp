// CIS 1202 Final Project SK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>
#include "Player.h"
#include "utils.h"

using namespace std;

// Function prototypes for menu options:
void displayAllPlayersByAgeGroup(const vector<Player>& players,
    const vector<double>& boxOutAll,
    const vector<double>& screenAssistAll,
    const vector<double>& deflectionsAll,
    const vector<double>& looseBallsAll,
    const vector<double>& chargesAll,
    const vector<double>& contestedShotsAll);

void displayTop10LeaderboardByAgeGroup(const vector<Player>& players,
    const vector<double>& boxOutAll,
    const vector<double>& screenAssistAll,
    const vector<double>& deflectionsAll,
    const vector<double>& looseBallsAll,
    const vector<double>& chargesAll,
    const vector<double>& contestedShotsAll);

void searchPlayerByName(const vector<Player>& players,
    const vector<double>& boxOutAll,
    const vector<double>& screenAssistAll,
    const vector<double>& deflectionsAll,
    const vector<double>& looseBallsAll,
    const vector<double>& chargesAll,
    const vector<double>& contestedShotsAll);

// CSV reading function prototype:
static vector<Player> readCSV(const string& filename);

//-------------------------------------------
// main() function
//-------------------------------------------
int main() {
    cout << "Welcome to the NBA Hustle Stats Menu!" << endl;

    string filename = "nba_hustle_stats.csv";
    vector<Player> players = readCSV(filename);
    if (players.empty()) {
        cout << "No player data available. Exiting." << endl;
        return 1;
    }

    // Build stat vectors for hustle index calculations.
    vector<double> boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll;
    for (const Player& player : players) {
        boxOutAll.push_back(player.getBoxOut());
        screenAssistAll.push_back(player.getScreenAssist());
        deflectionsAll.push_back(player.getDeflections());
        looseBallsAll.push_back(player.getLooseBalls());
        chargesAll.push_back(player.getCharges());
        contestedShotsAll.push_back(player.getContestedShots());
    }

    int selection = 0;
    do {
        cout << "\n=== Main Menu ===\n";
        cout << "1. Display All Players by Age Group (Descending by Raw Hustle Index)\n";
        cout << "2. Leaderboards (Top 10 by Normalized Hustle Index for Each Age Group)\n";
        cout << "3. Search for a Player by Name\n";
        cout << "4. Exit\n";
        cout << "Enter your selection: ";
        cin >> selection;
        if (cin.fail() || selection < 1 || selection > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }
        switch (selection) {
        case 1:
            displayAllPlayersByAgeGroup(players, boxOutAll, screenAssistAll,
                deflectionsAll, looseBallsAll, chargesAll,
                contestedShotsAll);
            break;
        case 2:
            displayTop10LeaderboardByAgeGroup(players, boxOutAll, screenAssistAll,
                deflectionsAll, looseBallsAll, chargesAll,
                contestedShotsAll);
            break;
        case 3:
            searchPlayerByName(players, boxOutAll, screenAssistAll,
                deflectionsAll, looseBallsAll, chargesAll,
                contestedShotsAll);
            break;
        case 4:
            cout << "Exiting program. Goodbye!" << endl;
            break;
        }
    } while (selection != 4);

    return 0;
}

//-------------------------------------------
// Function definitions below main()
//-------------------------------------------

// Reads player data from a CSV.
static vector<Player> readCSV(const string& filename) {
    vector<Player> players;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return players;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;
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

// Option 1: Display all players by age group (descending raw score)
void displayAllPlayersByAgeGroup(const vector<Player>& players,
    const vector<double>& boxOutAll,
    const vector<double>& screenAssistAll,
    const vector<double>& deflectionsAll,
    const vector<double>& looseBallsAll,
    const vector<double>& chargesAll,
    const vector<double>& contestedShotsAll) {
    vector<pair<double, const Player*>> groupUnder25;
    vector<pair<double, const Player*>> group26_32;
    vector<pair<double, const Player*>> groupOver33;

    for (const Player& player : players) {
        double rawScore = player.calculateHustleIndex(boxOutAll, screenAssistAll,
            deflectionsAll, looseBallsAll,
            chargesAll, contestedShotsAll);
        if (player.getAge() < 25) {
            groupUnder25.push_back(make_pair(rawScore, &player));
        }
        else if (player.getAge() >= 26 && player.getAge() <= 32) {
            group26_32.push_back(make_pair(rawScore, &player));
        }
        else {
            groupOver33.push_back(make_pair(rawScore, &player));
        }
    }

    auto sortDesc = [](const pair<double, const Player*>& a, const pair<double, const Player*>& b) {
        return a.first > b.first;
        };
    sort(groupUnder25.begin(), groupUnder25.end(), sortDesc);
    sort(group26_32.begin(), group26_32.end(), sortDesc);
    sort(groupOver33.begin(), groupOver33.end(), sortDesc);

    cout << "\n--- All Players Grouped by Age (Descending by Raw Hustle Index) ---\n";
    cout << "\nUnder 25:\n";
    for (const auto& entry : groupUnder25)
        cout << entry.second->getName() << "  | Raw Hustle: " << entry.first << endl;
    cout << "\n26-32:\n";
    for (const auto& entry : group26_32)
        cout << entry.second->getName() << "  | Raw Hustle: " << entry.first << endl;
    cout << "\nOver 33:\n";
    for (const auto& entry : groupOver33)
        cout << entry.second->getName() << "  | Raw Hustle: " << entry.first << endl;
}

// Option 2: Display top 10 players by normalized hustle index for each age group.
void displayTop10LeaderboardByAgeGroup(const vector<Player>& players,
    const vector<double>& boxOutAll,
    const vector<double>& screenAssistAll,
    const vector<double>& deflectionsAll,
    const vector<double>& looseBallsAll,
    const vector<double>& chargesAll,
    const vector<double>& contestedShotsAll) {
    vector<const Player*> groupUnder25, group26_32, groupOver33;
    vector<double> rawUnder25, raw26_32, rawOver33;

    for (const Player& player : players) {
        double rawScore = player.calculateHustleIndex(boxOutAll, screenAssistAll,
            deflectionsAll, looseBallsAll,
            chargesAll, contestedShotsAll);
        if (player.getAge() < 25) {
            groupUnder25.push_back(&player);
            rawUnder25.push_back(rawScore);
        }
        else if (player.getAge() >= 26 && player.getAge() <= 32) {
            group26_32.push_back(&player);
            raw26_32.push_back(rawScore);
        }
        else {
            groupOver33.push_back(&player);
            rawOver33.push_back(rawScore);
        }
    }

    vector<double> normalizedUnder25 = normalizeScores(rawUnder25);
    vector<double> normalized26_32 = normalizeScores(raw26_32);
    vector<double> normalizedOver33 = normalizeScores(rawOver33);

    vector<pair<double, const Player*>> leaderboardUnder25, leaderboard26_32, leaderboardOver33;
    for (size_t i = 0; i < groupUnder25.size(); i++)
        leaderboardUnder25.push_back(make_pair(normalizedUnder25[i], groupUnder25[i]));
    for (size_t i = 0; i < group26_32.size(); i++)
        leaderboard26_32.push_back(make_pair(normalized26_32[i], group26_32[i]));
    for (size_t i = 0; i < groupOver33.size(); i++)
        leaderboardOver33.push_back(make_pair(normalizedOver33[i], groupOver33[i]));

    auto sortDesc = [](const pair<double, const Player*>& a, const pair<double, const Player*>& b) {
        return a.first > b.first;
        };
    sort(leaderboardUnder25.begin(), leaderboardUnder25.end(), sortDesc);
    sort(leaderboard26_32.begin(), leaderboard26_32.end(), sortDesc);
    sort(leaderboardOver33.begin(), leaderboardOver33.end(), sortDesc);

    cout << "\n--- Leaderboard (Top 10 by Normalized Hustle Index for Each Age Group) ---\n";

    cout << "\nUnder 25:\n";
    for (size_t i = 0; i < leaderboardUnder25.size() && i < 10; i++) {
        cout << i + 1 << ". " << leaderboardUnder25[i].second->getName()
            << " | Normalized Hustle: " << leaderboardUnder25[i].first << endl;
    }
    cout << "\n26-32:\n";
    for (size_t i = 0; i < leaderboard26_32.size() && i < 10; i++) {
        cout << i + 1 << ". " << leaderboard26_32[i].second->getName()
            << " | Normalized Hustle: " << leaderboard26_32[i].first << endl;
    }
    cout << "\nOver 33:\n";
    for (size_t i = 0; i < leaderboardOver33.size() && i < 10; i++) {
        cout << i + 1 << ". " << leaderboardOver33[i].second->getName()
            << " | Normalized Hustle: " << leaderboardOver33[i].first << endl;
    }
}

// Option 3: Search for a player by name and display stats.
void searchPlayerByName(const vector<Player>& players,
    const vector<double>& boxOutAll,
    const vector<double>& screenAssistAll,
    const vector<double>& deflectionsAll,
    const vector<double>& looseBallsAll,
    const vector<double>& chargesAll,
    const vector<double>& contestedShotsAll) {
    cout << "Enter the player's name to search: ";
    string searchName;
    cin.ignore();
    getline(cin, searchName);

    bool found = false;
    for (const Player& player : players) {
        if (player.getName() == searchName) {
            cout << "\nFound player:\n";
            player.displayStats(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Player \"" << searchName << "\" not found." << endl;
    }
}