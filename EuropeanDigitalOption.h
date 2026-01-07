#pragma once
#include "Option.h"
#include <stdexcept>

// Option digitale européenne abstraite. Le payoff vaut 1 si la condition est satisfaite, 0 sinon. Cette classe sert de base aux options digitales Call et Put.
class EuropeanDigitalOption : public Option {
private:
	double _strike;
public:
	// Type de l'option digitale
	enum optionType { Call, Put };

	// Constructeur : initialise la maturité (via Option) et le strike de l'option digitale.
	EuropeanDigitalOption(double expiry, double strike)
		: Option(expiry), _strike(strike)
	{
		if ( strike < 0.0) {
			throw std::invalid_argument("Strike must be non-negative.");
		}
	}
	// Destructeur virtuel pour permettre la destruction polymorphe
		virtual ~EuropeanDigitalOption() = default;

		// Accès en lecture au strike
		double getStrike() const {
			return _strike;
		}
		// Retourne le type de l'option (Call ou Put)
		virtual optionType GetOptionType() const = 0;

		// Payoff digital à maturité (implémenté dans les classes dérivées)
		virtual double payoff(double S) const override = 0;
};