// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 12;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int analogPin = A0;
int val = 0; 
unsigned long interval = 10000;            // On period
unsigned long previousMillis = 0;
int pump = 0;
int PUMPPORT = 11;
int MOISTURE = 600;
int LowerMoisture = MOISTURE - 50;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);  
  pinMode(buttonPin, INPUT);
  pinMode(PUMPPORT, OUTPUT);
  digitalWrite(PUMPPORT, LOW);
  Serial.println(LowerMoisture);
  delay(1000);
}

void loop() {
  
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  
  unsigned long currentMillis = millis();
  
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  
  unsigned long currentmillis = millis();
  if ((unsigned long) (currentmillis - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        // do something here :-D
        ledState = !ledState;
        Serial.println("Switch On.");
        digitalWrite(ledPin, HIGH);
        delay(200);
        digitalWrite(ledPin, LOW);
        MOISTURE = MOISTURE + 20;
        Serial.print("Moisture = "); Serial.println(MOISTURE);
        
      }
    }
  }
  
  // set the LED:
  // digitalWrite(ledPin, ledState);
  
  
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  
  // put your main code here, to run repeatedly:
  val = analogRead(analogPin);    // read the input pin
  Serial.println(val);             // debug value


  if (val >= MOISTURE) {
    // Pump ON
    if (pump == 0) {
      digitalWrite(PUMPPORT, HIGH);
      Serial.println("Pump On.");
      pump = 1;
    }
    // After end of interval time turn pump off
    // DEBUG
    Serial.print(currentMillis - previousMillis);
    Serial.print(" >= ");
    Serial.println(interval);
    // End of DEBUG
    
    if ((unsigned long) (currentMillis - previousMillis) >= interval) {
      digitalWrite(PUMPPORT, LOW);
      pump = 0;
      previousMillis = currentMillis;
      Serial.println("Time out: Pump OFF.");
      delay(2000);
    }
  }
  else if (val <= LowerMoisture) {
    // Pump OFF
    if (pump != 0) {
      digitalWrite(PUMPPORT, LOW);
      Serial.println("Pump OFF.");
      pump = 0;
      
    }
    // reset time counter
    previousMillis = currentMillis;
  }
  else {
    // Stay
    // if pump is ON = 1 don't reset time counter
    if (pump == 0) {
      // reset time counter
      previousMillis = currentMillis;
    }
    else {
      if ((unsigned long) (currentMillis - previousMillis) >= interval) {
        digitalWrite(PUMPPORT, LOW);
        pump = 0;
        previousMillis = currentMillis;
        Serial.println("Time out: Pump OFF.");
        delay(2000);
      } 
    }
    Serial.println("Stay tune");
    delay(2000);
  }
  delay(500);
}
