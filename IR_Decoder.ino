// Michael Vinciguerra
// IR_Decoder.ino
// Research
// 11/2/21
// Receives the IR signals from IR_Morse and interprets the signals as morse code
//*************************************************************************************************

// More accurate timing libraries for Morse Code
#define F_CPU 16000000L
#include <avr/delay.h>

// Code repurposed from: https://create.arduino.cc/projecthub/electropeak/how-to-make-a-morse-code-translator-with-arduino-d6ecc8?ref=user&ref_id=573543&offset=1
// For full Morse Code table, check out: https://en.wikipedia.org/wiki/Morse_code
// S = 3 dots
// M = Two longs
// L = Dot, Dash, Dot, Dot
// We include the whole alphabet A-Z in case we decide to use more characters later on
static String morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
"..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", 
".....", "....-", "!"
};

String aboveBuilder = ""; // used to build our message during decoding (top sensor)
String sideBuilder = "";  // used to build our message during decoding (side sensor)

int noPowerReceive = 4; // D4 for one of the receiver pins
int powerReceive = 5; // D5 for the other receiver pin (for when LCE is actuated)

const float unit_delay = 25; // time between succcessive morse code symbols
const float extra_delay = 5; // just in case it is necessary

/* Structure to track the state of each break beam sensor */
// Format taken from candle2.ino by Scott Hudson
typedef struct breakTracker {
  int lastReading;       // last raw value read
  long lastChangeTime;   // last time the raw value changed
  byte pin;              // the pin this is tracking changes on
  byte state;            // state of the break beam sensor
  bool receiving;        // TRUE/FALSE for if it is receiving signal currently or not
} breakTrack;

/* Fill in a breakTrack structure to start tracking a sensor on a given pin
*/
// Taken from candle2.ino by Scott Hudson
void initBreakTrack(struct breakTracker &bk, int pin) {  
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH); // turn on pullup resistor
  bk.lastReading = digitalRead(pin);
  bk.lastChangeTime = millis();
  bk.pin = pin;
  bk.state = bk.lastReading;
  if (bk.state == 1) {
    bk.receiving = true;
  } else {
    bk.receiving = false;
  }
}

/*-------------------------------------------*/
/* Using a time based debounce, test whether a switch should be considered to have 
   changed state since the last call to this routine for the switch, and track the 
   current state of the switch in the given switch tracking structure (sw).  The switch 
   will be consided to be in a new state only after it has held that state for debounceTime msec 
   or longer.  sw.switchState will be set to 1 when the switch should be considered pressed 
   and 0 when it should be considered released.  This routine will return true when the 
   switch is in a new state compared to the last call to this routine, false otherwise.
   
   Note: this routine must be called frequently to operate correctly.
   Note: this code is here to make the demo a little more stand alone (nothing to import),
         but in other cases you might want to consider using the code from Bounce.h instead.
*/
// Taken from candle2.ino by Scott Hudson
boolean breakChange(struct breakTracker &bk) { 
  // default to no change until we find out otherwise
  boolean result = false; 
  
  // get the current raw reading from the switch
  int reading = digitalRead(bk.pin);

  // if the raw reading has changed (by noise or actual press/release) reset change time to now
  if (reading != bk.lastReading) bk.lastChangeTime = millis();
  bk.lastReading = reading;

  result = (reading != bk.state);

  bk.state = reading;
  
  return result;
}

// Decodes the symbols we have obtained into the appropriate character
void decode_builder(String builder) {
  for (int i = 0; i < 28; i++) { // Go through all letters of alphabet plus extra characters we define
    if (builder == morse[i]) {
      // Found a match!
      if (i == 26) {
        Serial.println('-'); // In Python Script, this is handled as a space, ' '.
      } else if (i == 27) {
        Serial.println('.');
      } else {
        Serial.println(char('A' + i)); // print the appropriate character based on offset from start of array
      }
      
      break; // only one character can be found
    }
  }
}

// Initialize the two trackers
// One will be hanging above the LCE device
// The other will be visible by the device when it actuates
breakTrack above;
breakTrack side;

void setup() {
  initBreakTrack(above, noPowerReceive);
  initBreakTrack(side, powerReceive);
  Serial.begin(9600);
}

void loop() {
  // Change in above sensor state?
  if (breakChange(above)) {
    // There is a change in the state!
    if (above.state == 1) { // We are receiving a signal!
      _delay_ms(unit_delay + extra_delay); // wait the length of a dot, then re-check the status

      if (breakChange(above)) { // no longer receiving signal, so it must be a dot
        aboveBuilder = aboveBuilder + ".";
      } else {
        aboveBuilder = aboveBuilder + "-";
        _delay_ms(2 * unit_delay); // delay for the remaining amount of time in a dash (should be 500 ms)
      }
    }
  } else { // No change in sensor state, perhaps it is not receiving? Or the IR LED is not sending?
    // Decode if there is no change in the sensor states after a long enough amount of time and data is stored in the builder
    if (aboveBuilder.length() != 0 && (millis() - above.lastChangeTime) > 4 * unit_delay) { // sync across each program
      Serial.print("a,");
      decode_builder(aboveBuilder); // decode the given morse code
      aboveBuilder = ""; // reset the string, since a character has been received
    }
  }

  // Change in side sensor state?
  if (breakChange(side)) {
    // There is a change in the state!
    if (side.state == 1) { // We are receiving a signal!
      _delay_ms(unit_delay + extra_delay); // wait the length of a dot, then re-check the status

      if (breakChange(side)) { // no longer receiving signal, so it must be a dot
        sideBuilder = sideBuilder + ".";
      } else {
        sideBuilder = sideBuilder + "-";
        _delay_ms(2 * unit_delay); // delay for the remaining amount of time in a dash (should be 500 ms)
      }
    }
  } else { // No change in sensor state, perhaps it is not receiving? Or the IR LED is not sending?
    // Decode if there is no change in the sensor states after a long enough amount of time and data is stored in the builder
    if (sideBuilder.length() != 0 && (millis() - side.lastChangeTime) > 4 * unit_delay) { // sync across each program
      Serial.print("s,");
      decode_builder(sideBuilder); // decode the given morse code
      sideBuilder = ""; // reset the string, since a character has been received
    }
  }
}
