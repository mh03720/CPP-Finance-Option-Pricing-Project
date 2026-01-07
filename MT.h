#pragma once
#include <random>
#include <stdexcept>

// Générateur de nombres aléatoires basé sur le moteur Mersenne Twister.Cette classe est utilisée pour les simulations Monte Carlo.
class MT {
private:
	// Moteur pseudo-aléatoire Mersenne Twister partagé
	static std::mt19937 mt;
	// Constructeur privé pour empêcher l'instanciation
	MT() = default; 

public:
	MT(const MT&) = delete; // Suppression du constructeur de copie
	MT& operator=(const MT&) = delete;  // Suppression de l'opérateur d'affectation

	// Génère une variable aléatoire uniforme sur [0,1]
	static double rand_unif() {
		 std::uniform_real_distribution<double> dist(0.0, 1.0);
		return dist(mt);
	}

	// Génère une variable aléatoire suivant une loi normale standard N(0,1)
	static double rand_norm() {
		 std::normal_distribution<double> dist(0.0, 1.0);
		return dist(mt);
	}
};
