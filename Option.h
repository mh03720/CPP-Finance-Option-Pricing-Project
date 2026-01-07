#pragma once
#include <vector>
#include <stdexcept>

// Classe abstraite représentant une option financière.Elle définit l'interface commune à toutes les options du projet (européennes, américaines, asiatiques, etc.).
class Option {
private :
	double _expiry; // Maturité de l'option

public:

    // Constructeur : initialise la maturité de l'option.La validité de expiry (>= 0) est vérifiée dans l'implémentation.
	explicit Option(double expiry);// constructor

    // Accesseur en lecture de la maturité
	double getExpiry() const;

    // Payoff dépendant uniquement du prix final du sous-jacent.Méthode pure virtuelle : chaque option concrète doit l'implémenter.
	virtual double payoff(double spot) const = 0;

    // Payoff dépendant d'un chemin de prix.Par défaut, on considère un payoff européen classique : h(S_T), où S_T est la dernière valeur du chemin.
   // Les options path-dependent (ex : asiatiques) redéfinissent cette méthode.
    virtual double payoffPath(const std::vector<double>& path) const {
        if (path.empty())
            throw std::invalid_argument("Empty path provided to payoffPath().");
        return payoff(path.back()); // h(S_tm)
    }

    // Indique si l'option est de type asiatique 
    virtual bool isAsianOption() const { return false; }

    //Indique si l'option est de type américaine
	virtual bool isAmericanOption() const { return false; } 

    // Destructeur virtuel
	virtual ~Option() = default;// virtual destructor


};
