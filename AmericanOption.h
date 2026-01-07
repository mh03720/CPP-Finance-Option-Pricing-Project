#pragma once
#include "Option.h"

// Classe abstraite intermédiaire représentant une option américaine.
// Elle ne définit aucun payoff : elle sert uniquement à identifier
// les options pouvant être exercées avant maturité.
class AmericanOption : public Option {
public:
	// Constructeur explicite.
   // L'expiry est transmis à la classe de base Option, qui se charge de vérifier que celui-ci est non négatif.
	explicit AmericanOption(double expiry)
		: Option(expiry) {
	}

	// Indique que l'option est de type américain.
	bool isAmericanOption() const override {
		return true;
	}
};