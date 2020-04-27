// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 2-8: Oscillators

// B0 : Controls Waveform Generator waveform type
// B1 : Controls the mixer , which source is selected : Sources: Waveform , Frequency modulated OSC , Regular sinewave OSC , Pink Noise
// B2 : Controls Envolope Generator
// A2 : Controls waveform generator frequency
// A3 : Controls modulation frequency



///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// Teensy Control Panel 04/26/2020
int _A1 = 0; 
int _A2 = 0;   
int _A3 = 0;     
int _B0 = 0;      
int _B1 = 0;    
int _B2 = 0;

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=261,351
AudioSynthNoisePink      pink1;          //xy=266,570
AudioSynthWaveformSine   sine1;          //xy=270,486
AudioSynthWaveformSineModulated sine_fm1;       //xy=434,390
AudioMixer4              mixer1;         //xy=710,353
AudioEffectEnvelope      envelope1;      //xy=850,457
AudioMixer4              mixer2;         //xy=994,350
AudioOutputI2S           i2s1;           //xy=1200,355
AudioConnection          patchCord1(waveform1, sine_fm1);
AudioConnection          patchCord2(waveform1, 0, mixer1, 0);
AudioConnection          patchCord3(pink1, 0, mixer1, 3);
AudioConnection          patchCord4(sine1, 0, mixer1, 2);
AudioConnection          patchCord5(sine_fm1, 0, mixer1, 1);
AudioConnection          patchCord6(mixer1, envelope1);
AudioConnection          patchCord7(mixer1, 0, mixer2, 0);
AudioConnection          patchCord8(envelope1, 0, mixer2, 1);
AudioConnection          patchCord9(mixer2, 0, i2s1, 0);
AudioConnection          patchCord10(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=738,625
// GUItool: end automatically generated code




void setup() {
  // Teensy Control Panel 04/26/2020  
  Serial.begin(115200);
  
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.32);
  
  //pinMode(0, INPUT_PULLUP);
  //pinMode(1, INPUT_PULLUP);
  //pinMode(2, INPUT_PULLUP);
  
  mixer1.gain(0, 0.75);
  mixer1.gain(1, 0.0);
  mixer1.gain(2, 0.0);
  mixer1.gain(3, 0.0);
  mixer2.gain(0, 0.15);
  mixer2.gain(1, 0.0);
  mixer2.gain(2, 0.0);
  mixer2.gain(3, 0.0);
  waveform1.begin(WAVEFORM_SAWTOOTH);
  waveform1.amplitude(0.75);
  waveform1.frequency(50);
  waveform1.pulseWidth(0.15);
  sine_fm1.frequency(440);
  sine_fm1.amplitude(0.75);
  sine1.frequency(200);
  sine1.amplitude(0.75);
  pink1.amplitude(0.75);
  envelope1.attack(10);
  envelope1.hold(10);
  envelope1.decay(25);
  envelope1.sustain(0.4);
  envelope1.release(70);
}

int waveform_type = WAVEFORM_SAWTOOTH;
int mixer1_setting = 0;
int mixer2_setting = 0;
elapsedMillis timeout = 0;
bool mixer2_envelope = false;

void loop()
{
   // Teensy Control Panel 04/26/2020 changes  
  if (Serial.available() > 0) 
  {
	   ctr_inputs();
  }   
 
  // Volume  , added Volume control , Teensy Control Panel 04/26/2020 changes  
  sgtl5000_1.volume((float)(_A1 / 1023.0));
   
  // Left changes the type of control waveform
  if (_B0)  // Teensy Control Panel 04/26/2020 changes
  {
    Serial.print("Control waveform: ");
    if (waveform_type == WAVEFORM_SAWTOOTH) {
      waveform_type = WAVEFORM_SINE;
      Serial.println("Sine");
    } else if (waveform_type == WAVEFORM_SINE) {
      waveform_type = WAVEFORM_SQUARE;
      Serial.println("Square");
    } else if (waveform_type == WAVEFORM_SQUARE) {
      waveform_type = WAVEFORM_TRIANGLE;
      Serial.println("Triangle");
    } else if (waveform_type == WAVEFORM_TRIANGLE) {
      waveform_type = WAVEFORM_PULSE;
      Serial.println("Pulse");
    } else if (waveform_type == WAVEFORM_PULSE) {
      waveform_type = WAVEFORM_SAWTOOTH;
      Serial.println("Sawtooth");
    }
    waveform1.begin(waveform_type);
  }

  // middle button switch which source we hear from mixer1
  if (_B1)  // Teensy Control Panel 04/26/2020 changes
  {
    if (mixer1_setting == 0) {
      mixer1.gain(0, 0.75);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.0);
      Serial.println("Mixer1: Control oscillator");
      mixer1_setting = 1;
    } else if (mixer1_setting == 1) {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.75);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.0);
      Serial.println("Mixer1: Frequency Modulated Oscillator");
      mixer1_setting = 2;
    } else if (mixer1_setting == 2) {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.75);
      mixer1.gain(3, 0.0);
      Serial.println("Mixer1: Regular Sine Wave Oscillator");
      mixer1_setting = 3;
    } else if (mixer1_setting == 3) {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.75);
      Serial.println("Mixer1: Pink Noise");
      mixer1_setting = 0;
    }
  }

  // Right button activates the envelope 
  if (_B2) // Teensy Control Panel 04/26/2020 changes
  {
    Serial.println("Envelope ON/OFF");
    mixer2.gain(0, 0.0);
    mixer2.gain(1, 1.0);
    mixer2_envelope = true;
    timeout = 0;
    envelope1.noteOn();   
    timeout = 0;
    delay(200);
    envelope1.noteOff();
    mixer2.gain(0, 0.15);
    mixer2.gain(1, 0.0);
    mixer2_envelope = false;   
  }
  //else //_B2 = 0
  //{
  //  envelope1.noteOff();
  //  timeout = 0;
  //}

  // after 4 seconds of inactivity, go back to
  // steady listening intead of the envelope
 // if (mixer2_envelope == true && timeout > 4000) {
 //   mixer2.gain(0, 0.15);
 //   mixer2.gain(1, 0.0);
 //   mixer2_envelope = false;
 // }

  // use the knobs to adjust parameters
  //float knob1 = (float)analogRead(A1) / 1023.0;
  float knob2 = (float)_A2 / 1023.0;
  float knob3 = (float)_A3 / 1023.0;
  waveform1.frequency(360 * knob2 + 0.25);
  sine_fm1.frequency(knob3 * 1500 + 50);
  sine1.frequency(knob3 * 1500 + 50);

// Teensy Control Panel 04/26/2020
  _B0 = 0;      
  _B1 = 0;    
  _B2 = 0;

  
} // End of Loop


// Teensy Control Panel 04/26/2020
void ctr_inputs()
{  
   int _a1 = Serial.parseInt();    
   int _a2 = Serial.parseInt();  
   int _a3 = Serial.parseInt();  
   int _b0 = Serial.parseInt();  
   int _b1 = Serial.parseInt();  
   int _b2 = Serial.parseInt();
   if (Serial.read() == '\n') 
   {
      _A1 = _a1;    
      _A2 = _a2;  
      _A3 = _a3;  
      _B0 = _b0;  
      _B1 = _b1;  
      _B2 = _b2;     
   }
}
        
  
