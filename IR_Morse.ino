// Michael Vinciguerra
// IR_Morse.ino
// Research
// 11/2/21
// Blinks the lab name "SML" in morse code on an IR LED which needs to be interpreted by a receiver
//*************************************************************************************************

// More accurate timing libraries for Morse Code
#define F_CPU 16000000L
#include <avr/delay.h>

// Code repurposed from: https://create.arduino.cc/projecthub/electropeak/how-to-make-a-morse-code-translator-with-arduino-d6ecc8?ref=user&ref_id=573543&offset=1
// For full Morse Code table, check out: https://en.wikipedia.org/wiki/Morse_code

const int IRLED = 2; // D2 pin used as pin to drive IR LED
const int unit_delay = 25; // time between succcessive morse code symbols

// Function creates the "." character in morse code
void dot() {
  digitalWrite(IRLED, HIGH); // turn on the IR LED
  _delay_ms(unit_delay);
  digitalWrite(IRLED, LOW); // turn off the IR LED
  _delay_ms(unit_delay);
}

// Function creates the "-" character in morse code
void dash() {
  digitalWrite(IRLED, HIGH); // turn on the IR LED
  _delay_ms(3 * unit_delay); // dashes need to be longer
  digitalWrite(IRLED, LOW); // turn off the IR LED
  _delay_ms(unit_delay);
}

void A() {
  dot();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void B() {
  dash();
  dot();
  dot();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void C() {
  dash();
  dot();
  dash();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void D() {
  dash();
  dot();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void E() {
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void F_char() {
  dot();
  dot();
  dash();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void G() {
  dash();
  dash();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void H() {
  dot();
  dot();
  dot();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void I() {
  dot();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void J() {
  dot();
  dash();
  dash();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void K() {
  dash();
  dot();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void L() {
  dot();
  dash();
  dot();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void M() {
  dash();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void N() {
  dash();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void O() {
  dash();
  dash();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void P() {
  dot();
  dash();
  dash();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void Q() {
  dash();
  dash();
  dot();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void R() {
  dot();
  dash();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void S() {
  dot();
  dot();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void T() {
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void U() {
  dot();
  dot();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void V() {
  dot();
  dot();
  dot();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void W() {
  dot();
  dash();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void X() {
  dash();
  dot();
  dot();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void Y() {
  dash();
  dot();
  dash();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

void Z() {
  dash();
  dash();
  dot();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

// Typically this would be reserved for the number 5, but we will
// repurpose it for our application
void space() {
  dot();
  dot();
  dot();
  dot();
  dot();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

// Typically this would be reserved for the number 4, but we will
// repurpose it for our application
void period() {
  dot();
  dot();
  dot();
  dot();
  dash();
  _delay_ms(4 * unit_delay); // used to indicate end of character
}

/* Use an array of function pointers to make writing arbitrary
 *  messages easy. Relies on knowledge of C++ pointers.
 */
void (*morse[])() {
  A,
  B,
  C,
  D,
  E,
  F_char,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  space,
  period
};

void setup() {
  pinMode(IRLED, OUTPUT);
  digitalWrite(IRLED, LOW);
}

void loop() {
  //String message = "In view a humble vaudevillian veteran cast vicariously as both victim and villain by the vicissitudes of Fate. ... The only verdict is vengeance. A vendetta held as a votive not in vain for the value and veracity of such shall one day vindicate the vigilant and the virtuous. ";
  String message = "Hello world. ";
  message = prep_message(message);
  write_morse(message); // cast to upper case since those are the only supported letters
}

// Problem: Unicode characters for . and ' ' aren't in line with the rest.
// Solution: Since we aren't using '[' and '\' currently, use those since they
// come right after our upper case characters in the unicode table!
// Source: https://en.wikipedia.org/wiki/List_of_Unicode_characters
String prep_message(String message) {
  message.toUpperCase();
  message.replace(' ', '[');
  message.replace('.', '\\');
  return message;
}

void write_morse(String message) {
  for (int i = 0; i < message.length(); i++) {
    int index = int(message[i] - 'A');
    (*morse[index])();
  }
}
