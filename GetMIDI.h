#pragma clang diagnostic push
#pragma clang diagnostic ignored 
#include "IPlug_include_in_plug_hdr.h"
#pragma clang diagnostic pop

#include "IMidiQueue.h"

#include "SignalFramework.h"
using Gallant::Signal2;

class GetMIDI
{
private:
    IMidiQueue MidiQueue;
    static const int keyCount = 128;
    int NumKeys; 
    bool KeyStatus[keyCount]; 
    int Offset;
    
public:
	GetMIDI() :
    NumKeys(0),
    Offset(0) {
        for (int i = 0; i < keyCount; i++)
		{
            KeyStatus[i] = false;
        }
    };
    
    inline bool getKeyStatus(int keyIndex) const { return KeyStatus[keyIndex]; }
    inline int getNumKeys() const { return NumKeys; }
    void advance();
    void onMessageReceived(IMidiMsg* midiMessage);
    inline void Flush(int nFrames) { MidiQueue.Flush(nFrames); Offset = 0; }
    inline void Resize(int blockSize) { MidiQueue.Resize(blockSize); }
    
    Signal2< int, int > noteOn;
    Signal2< int, int > noteOff;
};

