#include "Voice.h"
#include <functional>
class VoiceManager 
{
public:
    void onNoteOn(int noteNumber, int velocity);
    void onNoteOff(int noteNumber, int velocity);
    double nextSample();
    
    void setSampleRate(double sampleRate) 
	{
        Envelope::setSampleRate(sampleRate);
        for (int i = 0; i < NumberOfVoices; i++) 
		{
            Voice& voice = voices[i];
            voice.FirstOsc.setSampleRate(sampleRate);
            voice.SecondOsc.setSampleRate(sampleRate);
        }
       
    }

    
    typedef std::tr1::function<void (Voice&)> VoiceChangerFunction;
    inline void changeAllVoices(VoiceChangerFunction changer) 
	{
        for (int i = 0; i < NumberOfVoices; i++) 
		{
            changer(voices[i]);
        }
    }

    static void setVolumeEnvelopeStageValue(Voice& voice, Envelope::Stages stage, double value) {
        voice.EnvVolume.setStageValue(stage, value);
    }
  
    static void setType(Voice& voice, int oscillatorNumber, Oscillator::Type type) {
        switch (oscillatorNumber) {
            case 1:
                voice.FirstOsc.SetType(type);
                break;
            case 2:
                voice.SecondOsc.SetType(type);
                break;
        }
    }

    static void setOscillatorMix(Voice& voice, double value) 
	{
        voice.setOscillatorMix(value);
    }
    static void setFilterCutoff(Voice& voice, double cutoff) 
	{
        voice.Filter.setCutoff(cutoff);
    }
	static void setFilterCutoff2(Voice& voice, double cutoff2) 
	{
		voice.Filter.setCutoff2(cutoff2);
	}


    static void setLFOMix(Voice& voice, double mix) {
        voice.Filter.setLFOMix(mix);
    }

	static void setFreqLFO(Voice& voice, double freq) {
		voice.Filter.setFreqLFO(freq);
	}
	
private:
    static const int NumberOfVoices = 64;
    Voice voices[NumberOfVoices]; 
    Voice* findFreeVoice();
};

