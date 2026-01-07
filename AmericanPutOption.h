#pragma once
#include "AmericanOption.h"
#include <algorithm>
#include <stdexcept>

// Option de vente américaine (American Put).
// Le détenteur peut exercer l'option à tout moment avant l'échéance.
class AmericanPutOption : public AmericanOption {
	private:
		double _strike;

	public:
		// Constructeur : initialise l'expiry via AmericanOption, puis stocke le strike.
		AmericanPutOption(double expiry, double strike)
			: AmericanOption(expiry), _strike(strike)
		{
			// Le contrôle de expiry est effectué dans Option. Ici, on vérifie uniquement la validité du strike.
			if (strike < 0.0) {
				throw std::invalid_argument("Strike must be non-negative.");
			}
		}
		
		// Payoff d'un put : max(K - S, 0).
		double payoff(double S) const override {
			return std::max(_strike - S, 0.0);
		}
};
