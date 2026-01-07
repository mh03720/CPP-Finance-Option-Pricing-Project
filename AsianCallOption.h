#pragma once
#include "AsianOption.h"
#include <algorithm> 
#include <stdexcept>

// Option asiatique de type Call. Le payoff est appliqué à la moyenne arithmétique calculée dans AsianOption::payoffPath.
class AsianCallOption : public AsianOption {
private:
    double _strike;

public:
    // Constructeur : prend les dates d'observation (t1, ..., tm) et le strike de l'option.
    AsianCallOption(const std::vector<double>& timeSteps, double strike)
        : AsianOption(timeSteps), _strike(strike) {
        if (strike < 0.0) {
            throw std::invalid_argument("Strike must be non-negative.");
        }
    }

    // Payoff final : h(x) = max(x - K, 0), où x est la moyenne arithmétique calculée dans AsianOption.
    double payoff(double x) const override {
        return std::max(x - _strike, 0.0);
    }
};
