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
void displayAllPlayersByGlobalNormalization(const vector<Player>& players,
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
            displayAllPlayersByGlobalNormalization(players, boxOutAll, screenAssistAll,
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
                deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
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

void displayAllPlayersByGlobalNormalization(const std::vector<Player>& players,
    const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll) {
    std::vector<std::pair<Player, double>> rankedPlayers;  // Store player + normalized score

    for (const Player& player : players) {
        double hustleIndex = player.calculateHustleIndex(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
        rankedPlayers.emplace_back(player, hustleIndex);
    }

    // Normalize the scores
    std::vector<double> rawScores;
    for (const auto& pair : rankedPlayers) {
        rawScores.push_back(pair.second);
    }
    std::vector<double> normalizedScores = normalizeScores(rawScores);

    // Assign normalized scores back to players
    for (size_t i = 0; i < rankedPlayers.size(); ++i) {
        rankedPlayers[i].second = normalizedScores[i];
    }

    // Sort players by normalized score (Descending order)
    std::sort(rankedPlayers.begin(), rankedPlayers.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;  // Highest scores first
        });

    // Display sorted results
    cout << "\n--- Global Normalized Hustle Index Rankings (Descending) ---\n";
    for (const auto& pair : rankedPlayers) {
        cout << pair.first.getName() << " | Hustle Index (Global): " << pair.second << endl;
    }
}

void displayTop10LeaderboardByAgeGroup(const std::vector<Player>& players,
    const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll) {
    // Separate players into age groups
    std::vector<Player> under25, age26to32, over33;

    for (const Player& player : players) {
        if (player.getAge() < 25) under25.push_back(player);
        else if (player.getAge() >= 26 && player.getAge() <= 32) age26to32.push_back(player);
        else over33.push_back(player);
    }

    // Function to compute normalized hustle scores within each age group
    auto rankPlayersByAgeGroup = [](const std::vector<Player>& group,
        const std::vector<double>& boxOutAll,
        const std::vector<double>& screenAssistAll,
        const std::vector<double>& deflectionsAll,
        const std::vector<double>& looseBallsAll,
        const std::vector<double>& chargesAll,
        const std::vector<double>& contestedShotsAll) {
            std::vector<std::pair<Player, double>> rankedPlayers;
            std::vector<double> rawScores;

            // ✅ Compute raw hustle index for each player in the group
            for (const Player& player : group) {
                double hustleIndex = player.calculateHustleIndex(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
                rankedPlayers.emplace_back(player, hustleIndex);
                rawScores.push_back(hustleIndex);
            }

            // ✅ Normalize scores within the group
            std::vector<double> normalizedScores = normalizeScoresByGroup(rawScores);
            for (double score : normalizedScores) {
                cout << "DEBUG: Normalized Score -> " << score << endl;
            }

            // ✅ Assign normalized scores back to players
            for (size_t i = 0; i < rankedPlayers.size(); ++i) {
                rankedPlayers[i].second = normalizedScores[i];
                cout << "DEBUG: Raw Score -> " << rawScores[i] << " | Normalized Score -> " << normalizedScores[i] << endl;
            }
            

            // ✅ Sort by normalized hustle index (Descending)
            std::sort(rankedPlayers.begin(), rankedPlayers.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;
                });

            // ✅ Display leaderboard (Top 10 players in the group)
            cout << "\n--- Leaderboard for Age Group ---\n";
            for (size_t i = 0; i < std::min(rankedPlayers.size(), static_cast<size_t>(10)); ++i) {
                cout << rankedPlayers[i].first.getName() << " | Hustle Index (Age Adjusted): " << rankedPlayers[i].second << endl;
            }
        };

    cout << "\n=== TOP 10 HUSTLE PLAYERS BY AGE GROUP ===";
    rankPlayersByAgeGroup(under25, boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
    rankPlayersByAgeGroup(age26to32, boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
    rankPlayersByAgeGroup(over33, boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
}

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
            cout << "Name: " << player.getName() << "\nTeam: " << player.getTeam() << "\nAge: " << player.getAge() << endl;

            for (const Player& player : players) {
                if (player.getName() == searchName) {
                    cout << "\nFound player:\n";
                    cout << "Name: " << player.getName() << "\nTeam: " << player.getTeam() << "\nAge: " << player.getAge() << endl;

                    // ✅ Call displayStats() here
                    player.displayStats(players, boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);

                    found = true;
                    break;
                }
            }
            double globalScore = player.getGlobalNormalizedScore(players, boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
            double ageAdjustedScore = player.getAgeAdjustedNormalizedScore(players, boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);

            cout << "**Global Normalized Hustle Index:** " << globalScore << endl;
            cout << "**Age Group Normalized Hustle Index:** " << ageAdjustedScore << endl;

            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Player \"" << searchName << "\" not found." << endl;
    }
}