#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

class Player {
private:
    std::string name, team;
    int age, gp, minutes;
    double boxOut, screenAssist, deflections, looseBalls, charges, contestedShots;

public:
    // Constructor
    Player(std::string n, std::string t, int a, int g, int m,
        double bo, double sa, double de, double lb, double ch, double cs);

    // Compute Hustle Index (raw score) based on this player's stats relative to provided vectors
    double calculateHustleIndex(const std::vector<double>& boxOutAll,
        const std::vector<double>& screenAssistAll,
        const std::vector<double>& deflectionsAll,
        const std::vector<double>& looseBallsAll,
        const std::vector<double>& chargesAll,
        const std::vector<double>& contestedShotsAll) const;

    // Getters
    std::string getName() const;
    std::string getTeam() const;
    int getAge() const;
    double getBoxOut() const;
    double getScreenAssist() const;
    double getDeflections() const;
    double getLooseBalls() const;
    double getCharges() const;
    double getContestedShots() const;


    // Static helper functions
    static double computeMean(const std::vector<double>& data);
    static double computeStdDev(const std::vector<double>& data, double mean);
    static double computeZScore(double value, double mean, double stdDev);


    void displayStats(const std::vector<Player>& players,  
        const std::vector<double>& boxOutAll,
        const std::vector<double>& screenAssistAll,
        const std::vector<double>& deflectionsAll,
        const std::vector<double>& looseBallsAll,
        const std::vector<double>& chargesAll,
        const std::vector<double>& contestedShotsAll) const;

    double getGlobalNormalizedScore(const std::vector<Player>& players,
        const std::vector<double>& boxOutAll,
        const std::vector<double>& screenAssistAll,
        const std::vector<double>& deflectionsAll,
        const std::vector<double>& looseBallsAll,
        const std::vector<double>& chargesAll,
        const std::vector<double>& contestedShotsAll) const;

    double getAgeAdjustedNormalizedScore(const std::vector<Player>& players,  
        const std::vector<double>& boxOutAll,
        const std::vector<double>& screenAssistAll,
        const std::vector<double>& deflectionsAll,
        const std::vector<double>& looseBallsAll,
        const std::vector<double>& chargesAll,
        const std::vector<double>& contestedShotsAll) const;

};

#endif
