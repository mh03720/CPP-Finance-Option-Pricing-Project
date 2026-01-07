#include "EuropeanVanillaOption.h"
#include <stdexcept>

// Constructeur de l'option européenne vanille.Initialise la maturité via la classe Option et stocke le strike de l'option.
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike)
	: Option(expiry), _strike(strike) 
{
	if (strike < 0.0) {
		throw std::invalid_argument("Expiry and Strike must be non-negative");
	}
}

// Accesseur en lecture du strike
double EuropeanVanillaOption::getStrike() const {

	return _strike;
}