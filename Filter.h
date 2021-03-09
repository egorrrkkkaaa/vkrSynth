#include <math.h>

class Filter 
{
	public:
		Filter() :
			HighCutFreq(0.99),
			LowCutFreq(0.99),
			b0(0.0),
			b1(0.0),
			b2(0.0),
			b3(0.0),
			b10(0.0),
			b11(0.0),
			b12(0.0),
			b13(0.0),
			mPI(2 * acos(0.0)),
			twoPI(2 * mPI),
			PhaseIncrease(0.0003),
			Phase(0.0)

		{};
		double OutVoice(double inputValue);
		double HighCut(double inputValue);
		double LowCut(double inputValue);
		double Mod(double inputValue);
		inline void setCutoff(double newCutoff) { HighCutFreq = newCutoff; };
		inline void setCutoff2(double newCutoff2) { LowCutFreq = newCutoff2; };
		inline void setLFOMix(double newLFOMix) { ModMix = newLFOMix; };
		inline void setFreqLFO(double newFreqLFO) { FreqLFO = newFreqLFO; };
		void reset() 
		{
			b0 = b1 = b2 = b3 = 
				b10 = b11 = b12 = b13 = 0.0;
		}
	private:
		double Phase;
		double FreqLFO;
		double PhaseIncrease;
		const double mPI;
		const double twoPI;
		double ModMix;
		double HighCutFreq;
		double LowCutFreq;     
		double b0;
		double b1;
		double b2;
		double b3;    
		double b10;
		double b11;
		double b12;
		double b13;   
};


