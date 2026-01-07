#include "Option.h"
#include <stdexcept>

// Constructeur de la classe Option. Initialise la maturité de l'option et vérifie qu'elle est valide (une maturité négative n'a pas de sens en finance).
Option::Option(double expiry) : _expiry(expiry) {
	if (expiry < 0.0)
	{
		throw std::invalid_argument("Expiry must be non-negative");
	}
}
// Accesseur en lecture de la maturité de l'option.
double Option::getExpiry() const {
	return _expiry;
}