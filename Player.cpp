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

void Player::displayStats() const {
    cout << "Player Name: " << name << endl;
    cout << "Team: " << team << endl;
    cout << "Age: " << age << endl;
}

// Compute mean for a given dataset
double Player::computeMean(const std::vector<double>& data) {
    double sum = 0.0;
    for (double val : data) {
        sum += val;
    }
    return sum / data.size();
}

// Compute standard deviation using mean
double Player::computeStdDev(const std::vector<double>& data, double mean) {
    double sumSq = 0.0;
    for (double val : data) {
        sumSq += std::pow(val - mean, 2);
    }
    return std::sqrt(sumSq / data.size());
}

// Compute Z-score
double Player::computeZScore(double value, double mean, double stdDev) {
    return (value - mean) / stdDev;
}

// Scale index from 1 to 100
double Player::scaleTo100(double value, double minValue, double maxValue) {
    return ((value - minValue) / (maxValue - minValue)) * 100;
}

double Player::calculateHustleIndex(const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll) const
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
    // Compute Z-scores
    double rawScore = zBoxOut + zScreenAssist + zDeflections + zLooseBalls + zCharges + zContestedShots;

    return rawScore;
}

string Player::getName() const {
    return name;
}

double Player::getBoxOut() const { return boxOut; }
double Player::getScreenAssist() const { return screenAssist; }
double Player::getDeflections() const { return deflections; }
double Player::getLooseBalls() const { return looseBalls; }
double Player::getCharges() const { return charges; }
double Player::getContestedShots() const { return contestedShots; }

void Player::displayStats(const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll) const {
    cout << "Player Name: " << name << endl;
    cout << "Team: " << team << endl;
    cout << "Age: " << age << endl;

    // Compute Hustle Index **without normalization** first
    double hustleIndexRaw = calculateHustleIndex(boxOutAll, screenAssistAll,
        deflectionsAll, looseBallsAll,
        chargesAll, contestedShotsAll);

    // Print raw index
    cout << "Raw Hustle Index (before scaling): " << hustleIndexRaw << endl;
}

