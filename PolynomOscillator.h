#include "Oscillator.h"

class PolynomOscillator : public Oscillator
{
public:
	PolynomOscillator() : lastOutput(0.0) { updateIncrement(); };
    double nextSample();
private:
    double poly_blep(double t);
    double lastOutput;
};

