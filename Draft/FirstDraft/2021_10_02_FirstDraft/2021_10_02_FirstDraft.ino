#include "MIDIUSB.h"
#include <Keyboard.h>
#include <Mouse.h>


#define KEY_KEYPAD_0            0x62
#define KEY_KEYPAD_1            0x59
#define KEY_KEYPAD_2            0x5A
#define KEY_KEYPAD_3            0x5B
#define KEY_KEYPAD_4            0x5C
#define KEY_KEYPAD_5            0x5D
#define KEY_KEYPAD_6            0x5E
#define KEY_KEYPAD_7            0x5F
#define KEY_KEYPAD_8            0x60
#define KEY_KEYPAD_9            0x61
#define KEY_KEYPAD_DECIMAL      0x63
#define KEY_KEYPAD_ENTER        0x58
#define KEY_KEYPAD_PLUS     0x57
#define KEY_KEYPAD_MINUS        0x56
#define KEY_KEYPAD_MULTIPLY     0x55
#define KEY_KEYPAD_DIVIDE       0x54
#define KEY_KEYPAD_NUMLOCK      0x53
#define KEY_KEYPAD_EQUALS       0x67

const int leftButton = MOUSE_LEFT;
const int rightButton = MOUSE_RIGHT;
const int middleButton = MOUSE_MIDDLE;


bool m_midiTrueOrKeyboardFalse = false;

struct BooleanChangeState {
  public:
    bool m_value;
    void SetValue() {
      m_value = true;
    }
    bool IsTrue() {
      return m_value;
    }
    bool SetValueReturnChanged(bool newValue) {
      if (newValue != m_value) {
        m_value = newValue;
        return true;
      }
      return false;
    }
  private:
};
struct PinToBooleanState {
  public:
    bool m_isListened;
    int m_pinObserved;
    BooleanChangeState m_state;
  private:
};



//////////////////////////////////////////////////////////// KEYBOARD START
struct PinToKeyboard {
  public:
    bool m_isEnable;
    int m_pinObserved;
    int m_onSwitchKey;
    bool m_inverseOnOff;
  private:
};
int m_pinToKeyboardCount = 12;
PinToKeyboard m_pinToKeyboard [] = {
  {false, 3, KEY_KEYPAD_0 , false},
  {false, 4, KEY_KEYPAD_1 , false},
  {false, 5, KEY_KEYPAD_2 , false},
  {true, 6, KEY_KEYPAD_3, false },
  {true, 7, KEY_KEYPAD_4 , false},
  {true, 8, KEY_KEYPAD_5, false },
  {true, 9, KEY_KEYPAD_6, false },
  {true, 10, KEY_KEYPAD_7 , false},
  {true, 11, KEY_KEYPAD_8 , false},
  {true, 12, KEY_KEYPAD_9, false },
  {true, 13, KEY_KEYPAD_PLUS, false }
};


//////////////////////////////////////////////////////////// KEYBOARD END

//////////////////////////////////////////////////////////// MOUSE START
struct PinToMouse {
  public:
    bool m_isEnable;
    int m_pinObserved;
    // 0 Left 1 Mid 2 Right
    int m_onSwitchMouseId;
    bool m_inverseOnOff;
  private:
};

int m_pinToMouseCount = 3;
PinToMouse m_pinToMouse [] = {

  {true, 3, 0 , false},
  {true, 4, 2 , false},
  {true, 5, 1 , false}

};

//////////////////////////////////////////////////////////// MOUSE END
//////////////////////////////////////////////////////////// MIDI START
struct PinToMidiNote {
  public:
    bool m_isEnable;
    int m_pinObserved;
    int m_channel;
    int m_onSwitchNote;
    bool m_inverseOnOff;
  private:
};

//https://github.com/arduino/tutorials/blob/master/ArduinoZeroMidi/PitchToNote.h
int m_pinToMidiCount = 12;
PinToMidiNote m_pinToMidi [] = {
  {true, 3, 0, 1 , false},
  {true, 4, 0, 2 , false},
  {true, 5, 0, 3 , false},
  {true, 6, 0, 4, false },
  {true, 7, 0, 5 , false},
  {true, 8, 0, 6, false },
  {true, 9, 0, 7, false },
  {true, 10, 0, 8 , false},
  {true, 11, 0, 9 , false},
  {true, 12, 0, 10, false },
  {true, 13, 0, 11, false }
};



//////////////////////////////////////////////////////////// MIDI END

long m_frame = 0;
int m_pinObservedCount = 12;




PinToBooleanState m_pinObserved []  = {
  { true , 3 , {false} },
  { true , 4 , {false} },
  { true , 5 , {false} },
  { true , 6 , {false} },
  { true , 7 , {false} },
  { true , 8 , {false} },
  { true , 9 , {false} },
  { true , 10 , {false} },
  { true , 11 , {false} },
  { true , 12 , {false} },
  { true , 13 , {false} }
};

bool m_printDebug = true;

void setup() {
  Keyboard.begin();
  Mouse.begin();
  
  m_midiTrueOrKeyboardFalse = false;
  Serial.begin(9600);
  Serial.println("Hello World");
  pinMode (2 , INPUT_PULLUP);
  Keyboard.print("Hello World\n");
  for (int i = 0; i < m_pinObservedCount ; i++) {
    //if(m_pinObserved[i].m_m_isListened)
    pinMode (m_pinObserved[i].m_pinObserved , INPUT_PULLUP);
    if (IsDetectedChange(m_pinObserved[i])) {

      if (m_printDebug) {

        Serial.print(" Pin:" + m_pinObserved[i].m_pinObserved);
        Serial.print(" State: " + m_pinObserved[i].m_state.IsTrue());
        Serial.println("\n");

      }

    }
  }
          Mouse.release(MOUSE_RIGHT );
          Mouse.release(MOUSE_MIDDLE );
          Mouse.release(MOUSE_LEFT);
          Keyboard.releaseAll();


}



void loop() {

  m_midiTrueOrKeyboardFalse = digitalRead (2) == HIGH;
  //if(m_printDebug){
  //  Serial.print("Frame:");
  //  Serial.print(m_frame);
  //  Serial.println();
  //}
  for (int i = 0; i < m_pinObservedCount ; i++) {
    if (m_pinObserved[i].m_isListened && IsDetectedChange(m_pinObserved[i])) {
      if (m_printDebug) {
        if (m_midiTrueOrKeyboardFalse)
          Serial.print(" Midi, ");
        else
          Serial.print(" Keyboard, ");
        Serial.print(" Pin: ");
        Serial.print( m_pinObserved[i].m_pinObserved);
        Serial.print(" State: ");
        Serial.print(m_pinObserved[i].m_state.IsTrue());
        Serial.println();
      }
      if(m_frame!=0){
      ApplyChange(m_pinObserved[i].m_pinObserved, m_pinObserved[i].m_state.IsTrue());

    }
  }

  delay(10);

  m_frame++;


  /*
    for   (int i =0; i<24; i++){
      noteOn(0,i,64);
      MidiUSB.flush();
      delay(200);
      noteOff(0,i,64);
      MidiUSB.flush();

      delay(200);
      delay(100);
    }
    //*/
}

void ApplyChange(int pin, bool value) {

  //  m_pinToKeyboard   m_pinToMouse   m_pinToMidi
  // m_isEnable m_pinObserved m_inverseOnOff

  bool valueToApply = value;

  if (m_midiTrueOrKeyboardFalse) {
    for (int i = 0; i < m_pinToMidiCount; i++) {
      if ( m_pinToMidi[i].m_isEnable && m_pinToMidi[i].m_pinObserved == pin ) {
        if (m_pinToMidi[i].m_inverseOnOff) {
          valueToApply = !value;
        }

        if (valueToApply) {
          noteOn(m_pinToMidi[i].m_channel, m_pinToMidi[i].m_onSwitchNote, 127);
          MidiUSB.flush();
        }
        else {
          noteOff(m_pinToMidi[i].m_channel, m_pinToMidi[i].m_onSwitchNote, 0 );
          MidiUSB.flush();
        }
      }
    }
  } else {


    for (int i = 0; i < m_pinToKeyboardCount; i++) {
      if ( m_pinToKeyboard[i].m_isEnable && m_pinToKeyboard[i].m_pinObserved == pin) {
        if (m_pinToKeyboard[i].m_inverseOnOff) {
          valueToApply = !value;
        }
        if (valueToApply) {
          Keyboard.press(m_pinToKeyboard[i].m_onSwitchKey);
        }
        else {
          Keyboard.release(m_pinToKeyboard[i].m_onSwitchKey);
        }

      }
    }
    for (int i = 0; i < m_pinToMouseCount; i++) {
      if ( m_pinToMouse[i].m_isEnable && m_pinToMouse[i].m_pinObserved == pin) {
        if (m_pinToMouse[i].m_inverseOnOff) {
          valueToApply = !value;
        }

        if (m_pinToMouse[i].m_onSwitchMouseId == 0 && valueToApply)
          Mouse.press(MOUSE_LEFT);
        if (m_pinToMouse[i].m_onSwitchMouseId == 0 && !valueToApply)
          Mouse.release(MOUSE_LEFT);
        if (m_pinToMouse[i].m_onSwitchMouseId == 1 && valueToApply)
          Mouse.press(MOUSE_MIDDLE );
        if (m_pinToMouse[i].m_onSwitchMouseId == 1 && !valueToApply)
          Mouse.release(MOUSE_MIDDLE );
        if (m_pinToMouse[i].m_onSwitchMouseId == 2 && valueToApply)
          Mouse.press(MOUSE_RIGHT );
        if (m_pinToMouse[i].m_onSwitchMouseId == 2 && !valueToApply)
          Mouse.release(MOUSE_RIGHT );
      }
    }



  }
}



void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
void noteChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0x80 | channel, control, value};
  MidiUSB.sendMIDI(event);
}



bool IsDetectedChange(PinToBooleanState & observed) {
  bool newValue = digitalRead (observed.m_pinObserved) == LOW;
  return observed.m_state.SetValueReturnChanged(newValue);
}
