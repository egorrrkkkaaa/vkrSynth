
#include "GetMIDI.h"

void GetMIDI::onMessageReceived(IMidiMsg* midiMessage)
{
    IMidiMsg::EStatusMsg status = midiMessage->StatusMsg();
    if(status == IMidiMsg::kNoteOn || status == IMidiMsg::kNoteOff) 
	{
        MidiQueue.Add(midiMessage);
    }
}

void GetMIDI::advance()
{
    while (!MidiQueue.Empty()) 
	{
        IMidiMsg* midiMessage = MidiQueue.Peek();
        if (midiMessage->mOffset > Offset) break;
        
        IMidiMsg::EStatusMsg status = midiMessage->StatusMsg();
        int noteNumber = midiMessage->NoteNumber();
        int velocity = midiMessage->Velocity();
        if (status == IMidiMsg::kNoteOn && velocity)
		{
            if(KeyStatus[noteNumber] == false) 
			{
                KeyStatus[noteNumber] = true;
                NumKeys += 1;
                noteOn(noteNumber, velocity);
            }
        } else 
		{
            if(KeyStatus[noteNumber] == true) 
			{
                KeyStatus[noteNumber] = false;
                NumKeys -= 1;
                noteOff(noteNumber, velocity);
            }
        }
        MidiQueue.Remove();
    }
    Offset++;
}