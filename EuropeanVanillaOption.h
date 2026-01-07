#pragma once
#include "Option.h"

class EuropeanVanillaOption : public Option {
private :
	double _strike;
	friend class BlackScholesPricer;

public :
	enum optionType { Call, Put }; // Enum used to identify option type 

	EuropeanVanillaOption(double expiry, double strike);

	double getStrike() const;

	virtual optionType GetOptionType() const = 0;

	virtual ~EuropeanVanillaOption() = default;
};