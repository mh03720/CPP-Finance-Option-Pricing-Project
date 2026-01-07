#pragma once
#include "EuropeanDigitalOption.h"

// Option digitale européenne de type Put. Payoff : 1 si S <= K, 0 sinon.
class EuropeanDigitalPutOption : public EuropeanDigitalOption {
public:

	EuropeanDigitalPutOption(double expiry, double strike)
		: EuropeanDigitalOption(expiry, strike) {}

	// Indique que l'option est un Put digital.
	optionType GetOptionType() const override {
		return Put;
	}

	// Payoff digital put : h(S) = 1_{S <= K}
	double payoff(double S) const override {
		return (S <= getStrike()) ? 1.0 : 0.0;
	}
};

