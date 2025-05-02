#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>  // Needed for math operations
using namespace std;

class Player {
private:
    string name, team;
    int age, gp, minutes;
    double boxOut, screenAssist, deflections, looseBalls, charges, contestedShots;

public:
    // Constructor
    Player(string n, string t, int a, int g, int m, double bo, double sa, double de,
        double lb, double ch, double cs);

    // Compute Hustle Index
    double calculateHustleIndex(const std::vector<double>& boxOutAll,
        const std::vector<double>& screenAssistAll,
        const std::vector<double>& deflectionsAll,
        const std::vector<double>& looseBallsAll,
        const std::vector<double>& chargesAll,
        const std::vector<double>& contestedShotsAll) const;

    string getName() const;
    double getBoxOut() const;
    double getScreenAssist() const;
    double getDeflections() const;
    double getLooseBalls() const;
    double getCharges() const;
    double getContestedShots() const;

    // Function to display player data
    void displayStats() const;

    // Static helper functions (ONLY prototypes)
    static double computeMean(const std::vector<double>& data);
    static double computeStdDev(const std::vector<double>& data, double mean);
    static double computeZScore(double value, double mean, double stdDev);
    static double scaleTo100(double value, double minValue, double maxValue);

    void displayStats(const std::vector<double>& boxOutAll,
        const std::vector<double>& screenAssistAll,
        const std::vector<double>& deflectionsAll,
        const std::vector<double>& looseBallsAll,
        const std::vector<double>& chargesAll,
        const std::vector<double>& contestedShotsAll) const;

};

#endif
