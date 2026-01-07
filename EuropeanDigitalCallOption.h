#pragma once
#include "EuropeanDigitalOption.h"

// Option digitale européenne de type Call. Payoff : 1 si S >= K, 0 sinon
class EuropeanDigitalCallOption : public EuropeanDigitalOption {
public:
	EuropeanDigitalCallOption(double expiry, double strike)
		: EuropeanDigitalOption(expiry,strike) {}

	// Indique que l'option est un Call digital.
	optionType GetOptionType() const override {
		return Call;
	}

	// Payoff digital call : h(S) = 1_{S >= K}
	double payoff(double S) const override {
		return (S >= getStrike()) ? 1.0 : 0.0;
	}
};