#pragma once
#include "BinaryTree.h"
#include "Option.h"
#include <cmath>
#include <stdexcept>

// Pricer binomial de Cox-Ross-Rubinstein (CRR).Permet de pricer des options européennes et américaines à l'aide d'un arbre binomial de profondeur N.
class CRRPricer {
private:
	Option* _option;	// Option à pricer
	int _depth;			 // Profondeur de l'arbre binomial
	double _S0;			// Prix initial du sous-jacent
	double _U, _D, _R;	//Paramètres du modèle (hausse,baisse,actualisation)
	double _q;			//Probabilité neutre du risque

	BinaryTree<double> _stockTree;	// Valeurs du sous-jacent
	BinaryTree<double> _priceTree;	 // Valeurs de l'option
	BinaryTree<bool> _exerciseTree;	// Décisions d'exercice (options américaines)

	bool _computed;	// Indique si l'arbre a déjà été construit


public:
	// Constructeur CRR avec paramètres explicites (U, D, R).

	CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate);
	// Constructeur CRR à partir des paramètres Black-Scholes (r, sigma).Les paramètres U, D, R sont calculés à partir de ces valeurs.
	CRRPricer(Option* option,
		int depth,
		double asset_price,
		double r,
		double volatility);

	// Construit l'arbre binomial et calcule les valeurs de l'option.
	void compute();

	// Retourne la valeur de l'option au noeud (n,i).
	double get(int n, int i) const;

	// Indique si l'exercice est optimal au noeud (n,i)
	bool getExercise(int n, int i) const {
		return _exerciseTree.getNode(n, i);
	}

	// Retourne le prix de l'option à l'origine.Si closed_form = true, utilise la formule fermée CRR (uniquement disponible pour les options européennes).
	double operator()(bool closed_form = false);
};
