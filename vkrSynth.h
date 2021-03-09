#include "GetMIDI.h"
#include "VoiceManager.h"
#include "IPlug_include_in_plug_hdr.h"

class vkrSynth : public IPlug
{
public:
	vkrSynth(IPlugInstanceInfo instanceInfo);
	~vkrSynth();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);


  void ProcessMidiMsg(IMidiMsg* pMsg);  

  inline int GetNumKeys() const { return GetMIDI.getNumKeys(); };
  inline bool GetKeyStatus(int key) const { return GetMIDI.getKeyStatus(key); };
  static const int virtualKeyboardMinimumNoteNumber = 48;
  int lastVirtualKeyboardNoteNumber;
  
private:
  //double mFrequency;
  void CreatePresets();
  GetMIDI GetMIDI; 
  IControl* VirtualKyeb;
  void processVirtualKeyboard();
  void CreateParams();
  void CreateGraphics();  
  VoiceManager voiceManager;
};


