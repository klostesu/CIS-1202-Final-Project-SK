#include "Player.h"
#include <iostream>
#include <vector>
#include <cmath>  
using namespace std;

// Initialize all member variables in the constructor
Player::Player(string n, string t, int a, int g, int m,
    double bo, double sa, double de, double lb, double ch, double cs)
    : name(n), team(t), age(a), gp(g), minutes(m),
    boxOut(bo), screenAssist(sa), deflections(de),
    looseBalls(lb), charges(ch), contestedShots(cs) {
}


// Compute mean for a given dataset
double computeMean(const std::vector<double>& data) {
    double sum = 0.0;
    for (double val : data) {
        sum += val;
    }
    return sum / data.size();
}

// Compute standard deviation using mean
double computeStdDev(const std::vector<double>& data, double mean) {
    double sumSq = 0.0;
    for (double val : data) {
        sumSq += std::pow(val - mean, 2);
    }
    return std::sqrt(sumSq / data.size());
}

// Compute Z-score
double computeZScore(double value, double mean, double stdDev) {
    return (value - mean) / stdDev;
}

double Player::calculateHustleIndex(const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll)
{
    // Compute mean & standard deviation for each hustle stat
    double meanBoxOut = computeMean(boxOutAll);
    double stdDevBoxOut = computeStdDev(boxOutAll, meanBoxOut);

    double meanScreenAssist = computeMean(screenAssistAll);
    double stdDevScreenAssist = computeStdDev(screenAssistAll, meanScreenAssist);

    double meanDeflections = computeMean(deflectionsAll);
    double stdDevDeflections = computeStdDev(deflectionsAll, meanDeflections);

    double meanLooseBalls = computeMean(looseBallsAll);
    double stdDevLooseBalls = computeStdDev(looseBallsAll, meanLooseBalls);

    double meanCharges = computeMean(chargesAll);
    double stdDevCharges = computeStdDev(chargesAll, meanCharges);

    double meanContestedShots = computeMean(contestedShotsAll);
    double stdDevContestedShots = computeStdDev(contestedShotsAll, meanContestedShots);

    // Compute Z-scores
    double zBoxOut = computeZScore(boxOut, meanBoxOut, stdDevBoxOut);
    double zScreenAssist = computeZScore(screenAssist, meanScreenAssist, stdDevScreenAssist);
    double zDeflections = computeZScore(deflections, meanDeflections, stdDevDeflections);
    double zLooseBalls = computeZScore(looseBalls, meanLooseBalls, stdDevLooseBalls);
    double zCharges = computeZScore(charges, meanCharges, stdDevCharges);
    double zContestedShots = computeZScore(contestedShots, meanContestedShots, stdDevContestedShots);

    // Aggregate all Z-scores
    double rawScore = zBoxOut + zScreenAssist + zDeflections + zLooseBalls + zCharges + zContestedShots;

    // Scale to a 1-100 range
    double standardizedScore = ((rawScore + 6) / 12) * 100;  // Adjust range appropriately

    return standardizedScore;
}

void Player::displayStats() {
    cout << "Player: " << name << " | Team: " << team << " | Age: " << age << "\n";
}

void displayStats()
{
}
