#pragma once
#include "EuropeanVanillaOption.h"	
#include <algorithm>

// Option européenne vanille de type Put.
class PutOption : public EuropeanVanillaOption {

public:

	// Constructeur : initialise la maturité et le strike via la classe mère.
	PutOption(double expiry, double strike)
		: EuropeanVanillaOption(expiry, strike) {}

	// Payoff du Put européen : h(S) = max(K - S, 0).
	double payoff(double spot) const override {
		return std::max(getStrike() - spot, 0.0);
	}
	// Indique que l'option est un Put.
	optionType GetOptionType() const override {
		return optionType::Put;
	}

};
