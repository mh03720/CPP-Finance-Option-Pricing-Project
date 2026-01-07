#include "BlackScholesPricer.h"
#include <cmath>
#include <stdexcept>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace {
    // Fonction de répartition de la loi normale standard N(0,1)
    inline double N(double x) { // Standard normal CDF
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    }

    // Densité de la loi normale standard n(x)
    inline double n_pdf(double x) { 
        static const double INV_SQRT_2PI = 1.0 / std::sqrt(2.0 * M_PI);
        return INV_SQRT_2PI * std::exp(-0.5 * x * x);
    }
}

// Constructeur pour option européenne vanilla (Call/Put).
BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
    : _vanilla(option), _digital(nullptr),
    _S(asset_price), _r(interest_rate), _sigma(volatility)
{
    // Validations de base
    if (!option) throw std::invalid_argument("Null vanilla option pointer.");
    if (_S <= 0.0) throw std::invalid_argument("Asset price must be positive.");
    if (_sigma <= 0.0) throw std::invalid_argument("Volatility must be non-negative.");
}

// Constructeur pour option européenne digitale (Digital Call/Put).
BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
    : _vanilla(nullptr), _digital(option),
    _S(asset_price), _r(interest_rate), _sigma(volatility)
{
    if (!option) throw std::invalid_argument("Null digital option pointer.");
    if (_S <= 0.0) throw std::invalid_argument("Asset price must be positive.");
    if (_sigma < 0.0) throw std::invalid_argument("Volatility must be non-negative.");
}

// Prix Black-Scholes (formule fermée).
double BlackScholesPricer::operator()() const {
    //Cas option vanilla
    if (_vanilla) {
        const double T = _vanilla->getExpiry();
        const double K = _vanilla->_strike; // friend access

        // Préconditions (sinon d1/d2 non définis)
        if (T <= 0.0) throw std::invalid_argument("Expiry must be positive for BS pricing.");
        if (K <= 0.0) throw std::invalid_argument("Strike must be positive for BS pricing.");
        if (_sigma <= 0.0) throw std::invalid_argument("Volatility must be positive for BS pricing.");

        const double sT = _sigma * std::sqrt(T);
        const double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / sT;
        const double d2 = d1 - sT;

        // Formule Call/Put standard
        if (_vanilla->GetOptionType() == EuropeanVanillaOption::Call) {
            return _S * N(d1) - K * std::exp(-_r * T) * N(d2);
        }
        else {
            return K * std::exp(-_r * T) * N(-d2) - _S * N(-d1);
        }
    }

    // Cas option digital
    if (_digital) {
        const double T = _digital->getExpiry();
        const double K = _digital->getStrike();

        if (T <= 0.0) throw std::invalid_argument("Expiry must be positive for BS pricing.");
        if (K <= 0.0) throw std::invalid_argument("Strike must be positive for BS pricing.");
        if (_sigma <= 0.0) throw std::invalid_argument("Volatility must be positive for BS pricing.");

        const double sT = _sigma * std::sqrt(T);
        const double d2 = (std::log(_S / K) + (_r - 0.5 * _sigma * _sigma) * T) / sT;

        // Prix d'une digitale
        if (_digital->GetOptionType() == EuropeanDigitalOption::Call) {
            return std::exp(-_r * T) * N(d2);
        }
        else { 
            return std::exp(-_r * T) * N(-d2);
        }

        throw std::invalid_argument("Unsupported digital option type.");
    }

    // Si aucun pointeur d'option n'est fourni (situation anormale)
    throw std::runtime_error("No option provided to BlackScholesPricer.");
}

// Delta Black-Scholes.
double BlackScholesPricer::delta() const {
    // as option vanilla
    if (_vanilla) {
        const double T = _vanilla->getExpiry();
        const double K = _vanilla->_strike;

        if (T <= 0.0) throw std::invalid_argument("Expiry must be positive for BS delta.");
        if (K <= 0.0) throw std::invalid_argument("Strike must be positive for BS delta.");
        if (_sigma <= 0.0) throw std::invalid_argument("Volatility must be positive for BS delta.");

        const double sT = _sigma * std::sqrt(T);
        const double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / sT;

        if (_vanilla->GetOptionType() == EuropeanVanillaOption::Call) {
            return N(d1);
        }
        else {
            return N(d1) - 1.0;
        }
    }

    // Cas option digital
    if (_digital) {
        const double T = _digital->getExpiry();
        const double K = _digital->getStrike();

        if (T <= 0.0) throw std::invalid_argument("Expiry must be positive for BS delta.");
        if (K <= 0.0) throw std::invalid_argument("Strike must be positive for BS delta.");
        if (_sigma <= 0.0) throw std::invalid_argument("Volatility must be positive for BS delta.");

        const double sT = _sigma * std::sqrt(T);
        const double d2 = (std::log(_S / K) + (_r - 0.5 * _sigma * _sigma) * T) / sT;

        const double common = std::exp(-_r * T) * n_pdf(d2) / (_S * sT);

        if (_digital->GetOptionType() == EuropeanDigitalOption::Call) {
            return common;
        }
        else { 
            return -common;
        }

        throw std::invalid_argument("Unsupported digital option type.");
    }

    throw std::runtime_error("No option provided to BlackScholesPricer.");
}
