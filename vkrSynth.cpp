#include "vkrSynth.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmain"
#include "IPlug_include_in_plug_src.h"
#pragma clang diagnostic pop
#include "IControl.h"
#include "IKeyboardControl.h"
#include "resource.h"
#include <math.h>
#include <algorithm>
#include <functional>

const int kNumPrograms = 5;
const double parameterStep = 0.001;

enum EParams
{ 
	  pFirstOsc = 0, pSecondOsc, pMixOsc,  
	  LowCut, HighCut, pMixLFO, 
	  pFreqLFO, pAttack, pDecay,
	  pSustain, pRelease,  
	  pCount
};

typedef struct 
{
	  const char* name;
	  const int x;
	  const int y;
	  const double def;
	  const double min;
	  const double max;
} paramStruct;

const paramStruct paramProperties[pCount] =
{
	  {"Osc 1 Waveform", 35, 71},
	  {"Osc 2 Waveform", 35,  333}, 
	  {"Osc Mix",  35, 202,  0.5,  0.0,  1.0},
	  {"Low cut", 379,  71,  0.99,  0.0,  0.99},
	  {"High cut", 495,  71,  0.0,  0.0,  0.99},
	  {"LFO Mix", 495,  202,  0.0,  0.0,  1.0},
	  {"LFO Frequency", 379, 202, 0.0005, 0.0001, 0.003},
	  {"Attack", 149, 71, 0.01, 0.01, 10.0},
	  {"Decay", 264, 71, 0.5, 0.01, 15.0},
	  {"Sustain", 149, 202, 0.1, 0.001, 1.0},
	  {"Release", 264, 202, 1.0, 0.01, 15.0}
};


enum ELayout
{ 
  kKeybX = 150,
  kKeybY = 333
};

vkrSynth::vkrSynth(IPlugInstanceInfo instanceInfo) : IPLUG_CTOR(pCount, kNumPrograms, instanceInfo), lastVirtualKeyboardNoteNumber(virtualKeyboardMinimumNoteNumber - 1) 
{
  TRACE;  
  CreateParams();
  CreateGraphics();
  CreatePresets();  
  GetMIDI.noteOn.Connect(&voiceManager, &VoiceManager::onNoteOn);
  GetMIDI.noteOff.Connect(&voiceManager, &VoiceManager::onNoteOff);
}

vkrSynth::~vkrSynth() {}

void vkrSynth::CreateParams() 
{
  for (int i = 0; i < pCount; i++)
  {
    IParam* param = GetParam(i);
    const paramStruct& properties = paramProperties[i];
    switch (i) {
      case pFirstOsc:
      case pSecondOsc:
        param->InitEnum(properties.name,
                        Oscillator::Saw,
                        Oscillator::pCount);
        param->SetDisplayText(0, properties.name);
        break;    
      
      default:
        param->InitDouble(properties.name,
                          properties.def,
                          properties.min,
                          properties.max,
                          parameterStep);
        break;
    }
  }

  GetParam(LowCut)->SetShape(2);
  GetParam(pAttack)->SetShape(3);
  GetParam(pDecay)->SetShape(3);
  GetParam(pSustain)->SetShape(2);
  GetParam(pRelease)->SetShape(3);
  for (int i = 0; i < pCount; i++) {
    OnParamChange(i);
  }
}

void vkrSynth::CreateGraphics()
{
  IGraphics* pGraphics = MakeGraphics(this, WIDTH, HEIGHT);
  pGraphics->AttachBackground(BG_ID, BG_FN);
  IBitmap whiteKeyImage = pGraphics->LoadIBitmap(WK_ID, WK_FN, 6);
  IBitmap blackKeyImage = pGraphics->LoadIBitmap(BK_ID, BK_FN);
  int keyCoordinates[12] = { 0, 10, 17, 30, 35, 52, 61, 68, 79, 85, 97, 102 };
  VirtualKyeb = new IKeyboardControl(this, kKeybX, kKeybY, virtualKeyboardMinimumNoteNumber, /* octaves: */ 3, &whiteKeyImage, &blackKeyImage, keyCoordinates);
  pGraphics->AttachControl(VirtualKyeb);
  IBitmap waveformBitmap = pGraphics->LoadIBitmap(WF_ID, WF_FN, 4);
  IBitmap knobBitmap = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN);
  for (int i = 0; i < pCount; i++) 
  {
    const paramStruct& properties = paramProperties[i];
    IControl* control;
    IBitmap* graphic;
    switch (i)
	{      
      case pFirstOsc:
      case pSecondOsc:     
	  graphic = &waveformBitmap;
	  control = new ISwitchControl(this, properties.x, properties.y, i, graphic);
	  break;
      default:
        graphic = &knobBitmap;
        control = new IKnobRotaterControl(this, properties.x, properties.y, i, graphic);
        break;
    }
    pGraphics->AttachControl(control);
  }
  AttachGraphics(pGraphics);
}

void vkrSynth::CreatePresets()
{
}

void vkrSynth::ProcessDoubleReplacing(double** inputs,double** outputs, int nFrames)
{  
  double *leftOutput = outputs[0];
  double *rightOutput = outputs[1];
  processVirtualKeyboard();
  for (int i = 0; i < nFrames; ++i) 
  {
	  GetMIDI.advance();
    leftOutput[i] = rightOutput[i] = voiceManager.nextSample();
  }
  
  GetMIDI.Flush(nFrames);
}

void vkrSynth::Reset()
{
  TRACE;
  IMutexLock lock(this);
  double sampleRate = GetSampleRate();
  voiceManager.setSampleRate(sampleRate);
}

void vkrSynth::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);
  IParam* param = GetParam(paramIdx); 

    using std::tr1::placeholders::_1;
    using std::tr1::bind;
    VoiceManager::VoiceChangerFunction changer;
    switch(paramIdx) 
	{
      case pFirstOsc:
        changer = bind(&VoiceManager::setType, _1, 1,
                       static_cast<Oscillator::Type>(param->Int()));
        break;   
      case pSecondOsc:
        changer = bind(&VoiceManager::setType, _1, 2, static_cast<Oscillator::Type>(param->Int()));
        break;     
      case pMixOsc:
        changer = bind(&VoiceManager::setOscillatorMix, _1, param->Value());
        break;       
      case LowCut:
        changer = bind(&VoiceManager::setFilterCutoff, _1, param->Value());
        break;
	  case HighCut:
		  changer = bind(&VoiceManager::setFilterCutoff2, _1, param->Value());
		  break;    	  
      case pMixLFO:
        changer = bind(&VoiceManager::setLFOMix, _1, param->Value());
        break;  
	  case pFreqLFO:
		  changer = bind(&VoiceManager::setFreqLFO, _1, param->Value());
		  break;
      case pAttack:
        changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, Envelope::Attack, param->Value());
        break;
      case pDecay:
        changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, Envelope::Decay, param->Value());
        break;
      case pSustain:
        changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, Envelope::Sustain, param->Value());
        break;
      case pRelease:
        changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, Envelope::Release, param->Value());
        break;  	  
    }
    voiceManager.changeAllVoices(changer);
 
}

void vkrSynth::ProcessMidiMsg(IMidiMsg* pMsg)
{
	GetMIDI.onMessageReceived(pMsg);
	VirtualKyeb->SetDirty();
}

void vkrSynth::processVirtualKeyboard() 
{
  IKeyboardControl* virtualKeyboard = (IKeyboardControl*)VirtualKyeb;
  int virtualKeyboardNoteNumber = virtualKeyboard->GetKey() + virtualKeyboardMinimumNoteNumber;
  
  if(lastVirtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber)
  {
    IMidiMsg midiMessage;
    midiMessage.MakeNoteOffMsg(lastVirtualKeyboardNoteNumber, 0);
	GetMIDI.onMessageReceived(&midiMessage);
  }
  
  if (virtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber)
  {
    IMidiMsg midiMessage;
    midiMessage.MakeNoteOnMsg(virtualKeyboardNoteNumber, virtualKeyboard->GetVelocity(), 0);
	GetMIDI.onMessageReceived(&midiMessage);
  }
  
  lastVirtualKeyboardNoteNumber = virtualKeyboardNoteNumber;
}