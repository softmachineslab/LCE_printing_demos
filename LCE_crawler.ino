// Michael Vinciguerra
// Research
// 5/5/22
// LCE-Crawler
//*************************************************************

#define PHOTO A1

const int HEATER1 = 2;
const int HEATER2 = 5;
const int HEATER3 = 12;

const int impulse[] = {5000, 5000, 5000}; // turn on the heater for 2 seconds at a time
const int seg_delay = 100; // seconds of delay in between each heater being triggered
const int threshold = 100; // the expected change to trigger a photoresistor

int power = 0; // controls whether the crawler receives heat or not
int valInit = 0; // initial reading for photoresistor

void setup() {
  pinMode(PHOTO, INPUT);
  
  pinMode(HEATER1, OUTPUT);
  pinMode(HEATER2, OUTPUT);
  pinMode(HEATER3, OUTPUT);

  digitalWrite(HEATER1, LOW);
  digitalWrite(HEATER2, LOW);
  digitalWrite(HEATER3, LOW);

  valInit = analogRead(PHOTO);

  Serial.begin(9600); // for debugging purposes
}

void loop() {

  // Get readings from photoresistors and check to see if
  // the values have changed significantly
  int photoVal = analogRead(PHOTO);

  if(valInit - photoVal >= threshold) {
    power = ~power;
  }

  if (power != 0) {
    go();
  }
}

// Move the crawler to the left
void go() {
  Serial.println("Moving to the left...");
  
  digitalWrite(HEATER1, HIGH);
  Serial.println("Heater 1 Active");

  for (int i = 0; i < 10; i++) {
    delay(impulse[0]/10);
    int photoVal = analogRead(PHOTO);

    if(valInit - photoVal >= threshold) {
      power = ~power;
      digitalWrite(HEATER1, LOW);
      Serial.println("TURNING OFF CRAWLER\n");
      delay(2000);
      return;
    }
  }

  //delay(impulse[0]);

  digitalWrite(HEATER1, LOW);
  Serial.println("Heater 1 Inactive");

  delay(seg_delay);

  digitalWrite(HEATER2, HIGH);
  Serial.println("Heater 2 Active");

  for (int i = 0; i < 10; i++) {
    delay(impulse[1]/10);

    int photoVal = analogRead(PHOTO);

    if(valInit - photoVal >= threshold) {
      power = ~power;
      digitalWrite(HEATER2, LOW);
      Serial.println("TURNING OFF CRAWLER\n");
      delay(2000);
      return;
    }
  }

  //delay(impulse[1]);

  digitalWrite(HEATER2, LOW);
  Serial.println("Heater 2 Inactive");

  delay(seg_delay);

  digitalWrite(HEATER3, HIGH);
  Serial.println("Heater 3 Active");

  for (int i = 0; i < 10; i++) {
    delay(impulse[2]/10);

    int photoVal = analogRead(PHOTO);

    if(valInit - photoVal >= threshold) {
      power = ~power;
      digitalWrite(HEATER3, LOW);
      Serial.println("TURNING OFF CRAWLER\n");
      delay(2000);
      return;
    }
  }

  //delay(impulse[2]);

  digitalWrite(HEATER3, LOW);
  Serial.println("Heater 3 Inactive");
  Serial.println();
}
