// utils.h
// for declarations that are not directly related to players
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <algorithm>
#include <string>
using namespace std;



// Normalize a vector of raw scores to the 1–100 scale.
inline std::vector<double> normalizeScores(const std::vector<double>& rawScores) {
    std::vector<double> normalized;
    if (rawScores.empty())
        return normalized;

    double minScore = *std::min_element(rawScores.begin(), rawScores.end());
    double maxScore = *std::max_element(rawScores.begin(), rawScores.end());

    for (double score : rawScores) {
        double norm = (maxScore == minScore)
            ? 100
            : ((score - minScore) / (maxScore - minScore)) * 100;
        normalized.push_back(norm);
    }
    return normalized;
}

// Function template to adjust a metric's Z-score based on player's age and metric name.
template <typename T>
inline T adjustZScoreByAge(T zScore, int age, const std::string& metric) {
    // For players under 25: multiply charges and contestedShots by 2.
    if (age < 25 && (metric == "looseBalls" || metric == "contestedShots")) {
        return zScore * 2;
    }
    // For players 26 to 32: multiply deflections and charges by 2.
    else if (age >= 26 && age <= 32 && (metric == "deflections" || metric == "charges")) {
        return zScore * 2;
    }
    // For players over 33: multiply boxOut and screenAssist by 2.
    else if (age > 33 && (metric == "boxOut" || metric == "screenAssist")) {
        return zScore * 2;
    }
    return zScore;
}

#endif // UTILS_H
