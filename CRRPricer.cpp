#include "CRRPricer.h"

/*Constructeur CRR explicite
    Paramètres :
      - option        : option a pricer (européenne ou américaine)
      - depth         : profondeur de l'arbre binomial (N)
      - asset_price   : prix initial du sous-jacent S0
      - up            : rendement à la hausse (U)
      - down          : rendement à la baisse (D)
      - interest_rate : rendement sans risque par pas (R)*/
CRRPricer::CRRPricer(Option* option,
    int depth,
    double asset_price,
    double up,
    double down,
    double interest_rate)
    : _option(option),
    _depth(depth),
    _S0(asset_price),
    _U(up),
    _D(down),
    _R(interest_rate),
    _q(0.0),
    _computed(false)
{
    // Verification pointeur option
    if (!_option) {
        throw std::invalid_argument("Null option pointer.");
    }

    // Le modele CRR ne supporte pas les options path-dependent (asiatiques)
    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRR Pricer does not support Asian options.");
    }

    if (_depth < 0) {
        throw std::invalid_argument("Depth must be non-negative.");
    }

    // Condition d'absence d'arbitrage : D < R < U
    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("Arbitrage condition violated: require D < R < U");
    }


    // Probabilite neutre au risque
    _q = (_R - _D) / (_U - _D);

    // Initialisation des arbres binomiaux
    _stockTree.setDepth(_depth);
    _priceTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);

  
    // Construction de l'arbre du sous-jacent, S(n,0) = S0 * (1+D)^n, and S(n,i+1) = S(n,i) * (1+U)/(1+D)
    const double u = 1.0 + _U;
    const double d = 1.0 + _D;
    const double ud = u / d;

    double d_pow_n = 1.0; // d^0
    for (int n = 0; n <= _depth; ++n) {
        double S = _S0 * d_pow_n; // S(n,0)
        for (int i = 0; i <= n; ++i) {
            _stockTree.setNode(n, i, S);
            _exerciseTree.setNode(n, i, false); // initialiser à faux
			S *= ud; //Prochain noeud au meme niveau
        }
		d_pow_n *= d; //Prochain niveau
    }
}

/*Constructeur CRR à partir de Black - Scholes
    On approxime le modele BS par CRR :
        -dt = T / N
        -U = exp(σ√dt) - 1
        -D = exp(-σ√dt) - 1
        -R = exp(r dt) - 1*/
CRRPricer::CRRPricer(Option* option,
    int depth,
    double asset_price,
    double r,
    double volatility)
    : CRRPricer(option,
        depth,
        asset_price,
        std::exp(volatility* std::sqrt(option->getExpiry() / depth)) - 1.0,   // U
        std::exp(-volatility * std::sqrt(option->getExpiry() / depth)) - 1.0,  // D
        std::exp(r* (option->getExpiry() / depth)) - 1.0)                     // R
{
}

// Construction complete de l'arbre de prix (backward induction)
void CRRPricer::compute() {
    // Si déjà calcule, on ne refait rien
    if (_computed) return;

    const int N = _depth;
    const bool isAmerican = _option->isAmericanOption();
    // Facteur d'actualisation par pas
    const double disc = 1.0 / (1.0 + _R);

	// Payoff a maturite
    for (int i = 0; i <= N; ++i) {
        const double ST = _stockTree.getNode(N, i);
        const double intrinsic = _option->payoff(ST);

        _priceTree.setNode(N, i, intrinsic);
        // À maturite, l'exercice est optimal si payoff > 0
        _exerciseTree.setNode(N, i, isAmerican && (intrinsic > 0.0));
    }

    // Backward induction
    for (int n = N - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            // Valeurs futures
            const double upVal = _priceTree.getNode(n + 1, i + 1);
            const double downVal = _priceTree.getNode(n + 1, i);

            // Valeur de continuation
            const double continuation = (_q * upVal + (1.0 - _q) * downVal) * disc;

            // Valeur intrinsèque au noeud courant
            const double S = _stockTree.getNode(n, i);
            const double intrinsic = _option->payoff(S);

            double nodeValue = continuation;
            bool exerciseNow = false;

            if (isAmerican) {
                if (intrinsic >= continuation) {
                    nodeValue = intrinsic;
                    exerciseNow = true;
                }
            }

            _priceTree.setNode(n, i, nodeValue);
            _exerciseTree.setNode(n, i, exerciseNow);
        }
    }

    _computed = true;
}

// Accès a la valeur au noeud (n,i)
double CRRPricer::get(int n, int i) const {
    return _priceTree.getNode(n, i);
}

// Prix final de l'option
//  - closed_form = true : formule binomiale fermer (EUROPEEN uniquement)
//  - sinon : valeur issue de l'arbre
double CRRPricer::operator()(bool closed_form) {
    const int N = _depth;

    // La formule fermer CRR n existe pas pour les options americaines
    if (closed_form && _option->isAmericanOption()) {
        throw std::invalid_argument("Closed-form CRR formula is not available for American options.");
    }

    //Formule fermer CRR(option européenne)
    if (closed_form) {
        double price = 0.0;

        for (int i = 0; i <= N; ++i) {
            const double ST = _stockTree.getNode(N, i);
            const double h = _option->payoff(ST);

            // Binomial coefficient C(N,i) 
            double comb = 1.0;
            for (int k = 1; k <= i; ++k) {
                comb = comb * (N - k + 1) / k;
            }

            // Probabilite binomiale
            const double prob = std::pow(_q, i) * std::pow(1.0 - _q, N - i);
            price += comb * prob * h;
        }

        // Actualisation finale
        return price / std::pow(1.0 + _R, N);
    }

    //Pricing par arbre
    compute();
    return _priceTree.getNode(0, 0);
}
