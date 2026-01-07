#pragma once
#include "EuropeanVanillaOption.h"
#include <algorithm>

// Option européenne vanille de type Call.
class CallOption : public EuropeanVanillaOption {
public:

	// Constructeur : maturité et strike transmis à la classe mère.
	CallOption(double expiry, double strike)
		: EuropeanVanillaOption(expiry, strike) {}

	// Payoff du Call européen : h(S) = max(S - K, 0).
	double payoff(double spot) const override {
		return std::max(spot - getStrike(), 0.0);
	}

	// Indique que l'option est un Call.
	optionType GetOptionType() const override {
		return optionType::Call;
	}
};