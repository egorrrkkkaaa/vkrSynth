#include "VoiceManager.h"

Voice* VoiceManager::findFreeVoice() 
{
    Voice* freeVoice = NULL;
    for (int i = 0; i < NumberOfVoices; i++) 
	{
        if (!voices[i].isActive) {
            freeVoice = &(voices[i]);
            break;
        }
    }
    return freeVoice;
}

void VoiceManager::onNoteOn(int noteNumber, int velocity) 
{
    Voice* voice = findFreeVoice();
    if (!voice) {
        return;
    }
    voice->reset();
    voice->setNoteNumber(noteNumber);
    voice->Velocity = velocity;
    voice->isActive = true;
    voice->EnvVolume.ChangeStage(Envelope::Attack);
    voice->EnvVolume.ChangeStage(Envelope::Attack);
}

void VoiceManager::onNoteOff(int noteNumber, int velocity)
{
    for (int i = 0; i < NumberOfVoices; i++) {
        Voice& voice = voices[i];
        if (voice.isActive && voice.NoteNumber == noteNumber)
		{
            voice.EnvVolume.ChangeStage(Envelope::Release);
            voice.EnvVolume.ChangeStage(Envelope::Release);
        }
    }
}

double VoiceManager::nextSample() 
{
    double output = 0.0;
    for (int i = 0; i < NumberOfVoices; i++) 
	{
        Voice& voice = voices[i];
        output += voice.nextSample();
    }
    return output;
}

