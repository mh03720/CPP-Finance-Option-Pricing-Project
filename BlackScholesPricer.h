#pragma once
#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"
#include <cmath>
#include <stdexcept>

/*Pricer BlackScholes en formule fermée.
	Permet de pricer :
	 - options européennes vanilles (Call / Put)
	 - options digitales européennes (Call / Put)
 Le pricer contient soit une option vanilla, soit une option digitale, jamais les deux simultanément.*/
class BlackScholesPricer {

private:
	EuropeanVanillaOption* _vanilla=nullptr;	// option vanilla (si non nulle)
	EuropeanDigitalOption* _digital = nullptr;	// option digitale (si non nulle)

	double _S; // prix de l'actif sous-jacent
	double _r; // taux d'intérêt (continu)
	double _sigma; // volatilité 

public:
	BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);	// Constructeur pour options européennes vanilles
	BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility);	// Constructeur pour options digitales européennes

	// Retourne le prix BlackScholes de l'option
	double operator()() const;

	// Retourne le Delta BlackScholes de l'option
	double delta() const;
};