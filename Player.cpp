#include "Player.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Constructor
Player::Player(std::string n, std::string t, int a, int g, int m,
    double bo, double sa, double de, double lb, double ch, double cs)
    : name(n), team(t), age(a), gp(g), minutes(m),
    boxOut(bo), screenAssist(sa), deflections(de),
    looseBalls(lb), charges(ch), contestedShots(cs) {
}

// Getters
std::string Player::getName() const { return name; }
std::string Player::getTeam() const {
    return team;}
double Player::getBoxOut() const { return boxOut; }
double Player::getScreenAssist() const { return screenAssist; }
double Player::getDeflections() const { return deflections; }
double Player::getLooseBalls() const { return looseBalls; }
double Player::getCharges() const { return charges; }
double Player::getContestedShots() const { return contestedShots; }

int Player::getAge() const {
    return age;
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


void Player::displayStats(const std::vector<Player>& players,  // ✅ Added `players` argument
    const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll) const {
    cout << "\nPlayer Profile\n";
    cout << "Name: " << name << "\nTeam: " << team << "\nAge: " << age << endl;

    double hustleIndex = calculateHustleIndex(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
    cout << "Hustle Index (Raw): " << hustleIndex << endl;

    double globalScore = getGlobalNormalizedScore(players, boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
    double ageAdjustedScore = getAgeAdjustedNormalizedScore(players, boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
    //double ageAdjustedScore = normalizeScores({ player.calculateHustleIndex(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll) })[0];
    cout << "**Global Normalized Hustle Index:** " << globalScore << endl;
    cout << "**Age Group Normalized Hustle Index:** " << ageAdjustedScore << endl;
}

// calculateHustleIndex: computes a raw hustle index based on several metrics.
double Player::calculateHustleIndex(const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll) const {

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

    // Calculate Z-scores, applying age-based adjustments using our template function.
    double zBoxOut = adjustZScoreByAge(computeZScore(boxOut, meanBoxOut, stdDevBoxOut), age, "boxOut");
    double zScreenAssist = adjustZScoreByAge(computeZScore(screenAssist, meanScreenAssist, stdDevScreenAssist), age, "screenAssist");
    double zDeflections = adjustZScoreByAge(computeZScore(deflections, meanDeflections, stdDevDeflections), age, "deflections");
    double zLooseBalls = adjustZScoreByAge(computeZScore(looseBalls, meanLooseBalls, stdDevLooseBalls), age, "looseBalls");
    double zCharges = adjustZScoreByAge(computeZScore(charges, meanCharges, stdDevCharges), age, "charges");
    double zContestedShots = adjustZScoreByAge(computeZScore(contestedShots, meanContestedShots, stdDevContestedShots), age, "contestedShots");

    // Aggregate all Z-scores to form the raw hustle index.
    double rawScore = zBoxOut + zScreenAssist + zDeflections + zLooseBalls + zCharges + zContestedShots;
    return rawScore;
}

double Player::getGlobalNormalizedScore(const std::vector<Player>& players,
    const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll) const {
    std::vector<double> allRawScores;

    // ✅ Ensure 'players' is used correctly inside the loop
    for (const Player& player : players) {
        allRawScores.push_back(player.calculateHustleIndex(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll));
    }

    std::vector<double> normalizedScores = normalizeScores(allRawScores);

    double rawScore = calculateHustleIndex(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
    auto it = std::find(allRawScores.begin(), allRawScores.end(), rawScore);
    size_t index = std::distance(allRawScores.begin(), it);

    return normalizedScores[index];
}

double Player::getAgeAdjustedNormalizedScore(const std::vector<Player>& players,
    const std::vector<double>& boxOutAll,
    const std::vector<double>& screenAssistAll,
    const std::vector<double>& deflectionsAll,
    const std::vector<double>& looseBallsAll,
    const std::vector<double>& chargesAll,
    const std::vector<double>& contestedShotsAll) const {
    std::vector<double> rawScores;
    for (const Player& player : players) {
        if ((age < 25 && player.getAge() < 25) ||
            (age >= 26 && age <= 32 && player.getAge() >= 26 && player.getAge() <= 32) ||
            (age > 33 && player.getAge() > 33)) {
            rawScores.push_back(player.calculateHustleIndex(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll));
        }
    }
    std::vector<double> normalizedScores = normalizeScoresByGroup(rawScores);

    double rawScore = calculateHustleIndex(boxOutAll, screenAssistAll, deflectionsAll, looseBallsAll, chargesAll, contestedShotsAll);
    auto it = std::find(rawScores.begin(), rawScores.end(), rawScore);
    size_t index = std::distance(rawScores.begin(), it);

    return normalizedScores[index];
}
