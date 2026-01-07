#pragma once
#include "Option.h"
#include "MT.h"
#include <vector>

/*Pricer Monte Carlo sous BlackScholes:
	- Ne stocke aucun chemin : uniquement une estimation courante.
	- Met à jour l'estimation de manière incrémentale à chaque appel à generate().*/
class BlackScholesMCPricer {
private:
	Option* _option; // option à pricer 
	double _S0;	// prix spot initial
	double _r;	 // taux sans risque 
	double _sigma;	// volatilité

	long long _nbPaths;	// nombre total de trajectoires générées
	double _estimate; //estimation courante du prix(moyenne des payoffs actualisés)
	double _M2;	// accumulateur pour variance (Welford), pour l'IC

public:
	BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility);

	// Accès en lecture au nombre de chemins générés
	long long getNbPaths() const { return _nbPaths; }

	// Génère nb_paths trajectoires supplémentaires et met à jour l'estimation.
	void generate(int nb_paths);

	// Retourne l'estimation courante 
	double operator()() const;

	// Retourne l'intervalle de confiance à 95% sous la forme [borne_inf, borne_sup]
	std::vector<double> confidenceInterval() const;
};
