#pragma once
#include "AsianOption.h"
#include <algorithm>
#include <stdexcept>

// Option asiatique de type Put. Le payoff est appliqué à la moyenne arithmétique calculée dans AsianOption::payoffPath.
class AsianPutOption : public AsianOption {
private:
    double _strike;

public:
    // Constructeur : prend les dates d'observation (t1,...,tm) et le strike.
    AsianPutOption(const std::vector<double>& timeSteps, double strike)
        : AsianOption(timeSteps), _strike(strike) {
        if (strike < 0.0)
            throw std::invalid_argument("Strike must be non-negative.");
    }

   
    // Payoff : h(x) = max(K - x, 0), où x est la moyenne arithmétique du chemin.
    double payoff(double x) const override {
        return std::max(_strike - x, 0.0);
    }
};
