#pragma once
#include "Option.h"
#include <vector>
#include <stdexcept>
#include <numeric>

// Option asiatique : le payoff dépend de la moyenne arithmétique du chemin (S(t1), ..., S(tm)).
class AsianOption : public Option {
private:
    std::vector<double> _timeSteps;  // (t1, t2, ..., tm)

public:
    // Constructeur : prend les dates d'observation (t1,...,tm).L'expiry est fixée à tm (= dernier élément).
    explicit AsianOption(const std::vector<double>& timeSteps)
        :Option(timeSteps.empty() ? 0.0 : timeSteps.back()), _timeSteps(timeSteps)
    {
        if (timeSteps.empty()) {
            throw std::invalid_argument("Time steps vector cannot be empty.");
        }
    }
    // Getter en lecture seule des dates d'observation.
    const std::vector<double>& getTimeSteps() const {
        return _timeSteps;
	}
    bool isAsianOption() const override {
        return true;
    }
    // Payoff path-dependent : moyenne arithmétique du chemin, puis application du payoff(double) (défini dans AsianCallOption/AsianPutOption).
    double payoffPath(const std::vector<double>& path) const override {

        if (path.empty()) {
            throw std::invalid_argument("Path is empty.");
        }
        //n point du chemin par date d'observation.
        if (path.size() != _timeSteps.size()) {
            throw std::invalid_argument("Path size does not match the number of time steps.");
        }

        const double sum = std::accumulate(path.begin(), path.end(), 0.0);
        const double average = sum / static_cast<double>(path.size());

        return payoff(average);
    
    }  
};
