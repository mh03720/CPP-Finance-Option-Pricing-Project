#include "BlackScholesMCPricer.h"
#include <cmath>
#include "AsianOption.h"
#include <stdexcept>


// Constructeur du pricer Monte Carlo Black-Scholes. Initialise les paramètres du modèle et l'estimateur incrémental.
BlackScholesMCPricer::BlackScholesMCPricer(Option* option,
    double initial_price,
    double interest_rate,
    double volatility)
    : _option(option),
    _S0(initial_price),
    _r(interest_rate),
    _sigma(volatility),
    _nbPaths(0),
    _estimate(0.0),
    _M2(0.0)
{
    // Vérification de la validité des paramètres
    if (!_option) {
        throw std::invalid_argument("Option pointer is null.");
    }
    if (_S0 <= 0.0) {
        throw std::invalid_argument("Initial price must be positive.");
    }
    if (_sigma < 0.0) {
        throw std::invalid_argument("Volatility must be non-negative.");
    }
}

// Génère nb_paths trajectoires supplémentaires sous Black-Scholes et met à jour l'estimation du prix par moyenne incrémentale.
void BlackScholesMCPricer::generate(int nb_paths) {
    if (nb_paths <= 0) {
        throw std::invalid_argument("Number of paths must be positive.");
    }

    // Maturité de l'option
    const double T = _option->getExpiry();
    if (T < 0.0) {
        throw std::invalid_argument("Expiry must be non-negative.");
    }

    // Facteur d'actualisation
    const double disc = std::exp(-_r * T);

    // Identification du type d'option
    const bool isAsian = _option->isAsianOption();

    // Pré-calculs pour le cas européen (m = 1)
    const double driftT = (_r - 0.5 * _sigma * _sigma) * T;
    const double diffT = _sigma * std::sqrt(T);

    // Pointeurs utilisés uniquement dans le cas asiatique
    const AsianOption* asian = nullptr;
    const std::vector<double>* ts = nullptr;

    if (isAsian) {

        // Récupération des dates d'observation
        asian = dynamic_cast<const AsianOption*>(_option);

        if (!asian) {
            throw std::runtime_error("Option says it is Asian, but cannot cast to AsianOption.");
        }
        ts = &asian->getTimeSteps();
        if (ts->empty()) {
            throw std::runtime_error("Asian timeSteps vector is empty.");
        }
    }

    // Boucle principale de Monte Carlo
    for (int p = 0; p < nb_paths; ++p) {
        double payoff = 0.0;

        //Cas européen : une seule simulation à maturité
        if (!isAsian) {
            
            const double Z = MT::rand_norm();
            const double ST = _S0 * std::exp(driftT + diffT * Z);
            payoff = _option->payoff(ST);
        }
        //Cas asiatique : simulation d'un chemin discret
        else {
            std::vector<double> path;
            path.reserve(ts->size());

            double S = _S0;
            double t_prev = 0.0;

            // Simulation incrémentale du processus de Black-Scholes
            for (double t : *ts) {
                const double dt = t - t_prev;
                if (dt <= 0.0) {
                    throw std::invalid_argument("Asian timeSteps must be non-decreasing.");
                }

                const double drift_dt = (_r - 0.5 * _sigma * _sigma) * dt;
                const double diff_dt = _sigma * std::sqrt(dt);

                const double Z = MT::rand_norm();
                S *= std::exp(drift_dt + diff_dt * Z);

                path.push_back(S); // Stocke S(t_k)
                t_prev = t;
            }

            // Calcul du payoff à partir du chemin simulé
            payoff = asian->payoffPath(path);
        }

        // Actualisation du payoff
        const double discounted = disc * payoff;

        // Mise à jour incrémentale (algorithme de Welford)
        ++_nbPaths;
        const double delta = discounted - _estimate;
        _estimate += delta / static_cast<double>(_nbPaths);
        _M2 += delta * (discounted - _estimate);
    }
}

// Retourne l'estimation courante du prix.Une exception est levée si aucun chemin n'a été généré.
double BlackScholesMCPricer::operator()() const {
    if (_nbPaths == 0) {
        throw std::runtime_error("No paths generated. Call generate() before pricing.");
    }
    return _estimate;
}

// Calcule l'intervalle de confiance à 95 % autour de l'estimation.
std::vector<double> BlackScholesMCPricer::confidenceInterval() const {
    if (_nbPaths < 2) {
        throw std::runtime_error("At least two paths are required to compute confidence interval.");
    }
    const double variance = _M2 / static_cast<double>(_nbPaths - 1);
    const double stddev = std::sqrt(variance);
    const double margin = 1.96 * stddev / std::sqrt(static_cast<double>(_nbPaths));
    return { _estimate - margin, _estimate + margin };
}