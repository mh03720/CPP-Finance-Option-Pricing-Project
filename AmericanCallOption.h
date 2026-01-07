#pragma once
#include "AmericanOption.h"
#include <algorithm>
#include <stdexcept>

class AmericanCallOption : public AmericanOption {
private:
	double _strike;

public:
	//Constructeur : initialise d'abord la partie "AmericanOption" (expiry),puis stocke le strike.
		
	AmericanCallOption(double expiry, double strike)
		: AmericanOption(expiry), _strike(strike)
	{
		//Vérification de la validité des paramètres
		if ( strike < 0.0) {
			throw std::invalid_argument("Strike must be non-negative.");
		}
	}
	
	// Payoff d'un call : max(S - K, 0)
	double payoff(double S) const override {
		return std::max(S - _strike, 0.0);
	}
};