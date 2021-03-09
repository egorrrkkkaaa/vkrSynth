#define PLUG_CLASS_NAME vkrSynth

#define PLUG_MFR "Koshelev"
#define PLUG_NAME "vkrSynth"

#define PLUG_IS_INST 1
#define PLUG_DOES_MIDI 1


#define BUNDLE_MFR "Koshelev"
#define BUNDLE_NAME "vkrSynth"



#define PLUG_ENTRY vkrSynth_Entry
#define PLUG_VIEW_ENTRY vkrSynth_ViewEntry

#define PLUG_ENTRY_STR "vkrSynth_Entry"
#define PLUG_VIEW_ENTRY_STR "vkrSynth_ViewEntry"

#define VIEW_CLASS vkrSynth_View
#define VIEW_CLASS_STR "vkrSynth_View"


#define PLUG_VER 0x00010000
#define VST3_VER_STR "1.0.0"

#define PLUG_UNIQUE_ID 'uniq'
#define PLUG_MFR_ID 'mfr'



#if (defined(AAX_API) || defined(RTAS_API)) && !defined(_PIDS_)
  #define _PIDS_
  const int PLUG_TYPE_IDS[2] = {'EFN1', 'EFN2'};
  const int PLUG_TYPE_IDS_AS[2] = {'EFA1', 'EFA2'}; // AudioSuite
#endif

#define PLUG_MFR_PT "Koshelev"
#define PLUG_NAME_PT "vkrSynth"
#define PLUG_TYPE_PT "Effect"
#define PLUG_DOES_AUDIOSUITE 1


#if (defined(AAX_API) || defined(RTAS_API))
#define PLUG_CHANNEL_IO "1-1 2-2"
#else
#define PLUG_CHANNEL_IO "0-1 0-2"
#endif

#define PLUG_LATENCY 0
   
#define PLUG_DOES_STATE_CHUNKS 0
    
#define BG_ID  101
#define WK_ID  102
#define BK_ID  103
#define WF_ID   104
#define KNOB_ID 105 


#define BG_FN  "resources/img/bg.png"
#define WK_FN  "resources/img/wk.png"
#define BK_FN  "resources/img/bk.png"
#define WF_FN   "resources/img/waveform.png"
#define KNOB_FN  "resources/img/knob.png"

   
#define WIDTH 593
#define HEIGHT 450

#if defined(SA_API) && !defined(OS_IOS)
#include "app_wrapper/app_resource.h"
#endif

// vst3 stuff
#define MFR_URL "www.site.com"
#define MFR_EMAIL "email@mail.ru"
#define EFFECT_TYPE_VST3 "Instrument|Synth"

