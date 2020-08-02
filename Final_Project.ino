#include <LiquidCrystal.h>
#include <Keypad.h>

const byte rows = 4; 
const byte cols = 4;

//Layout of the Keypad
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[rows] = {2,12,11,10}; 
byte colPins[cols] = {9,8,7,6}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols); //Create the Keypad

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); 

int sensor1 = 13; //PIR sensor
int state = 0; //Current state of the PIR sensor
bool motionDetected = false;
bool locked = false;

int ledGreen = 3; 
int ledRed = 4; 

int buzzer = 5; 

String passcode = ""; //Passcode to lock or unlock the system
String input = ""; //Input compared to the passcode

void setup() {
  pinMode(sensor1, INPUT); 
  pinMode(ledGreen, OUTPUT); 
  pinMode(ledRed, OUTPUT); 
  pinMode(buzzer, OUTPUT); 
  lcd.begin(16,2); 
}

void loop() {
  if(locked == false){ //Check if the system is not locked
    digitalWrite(ledGreen, HIGH); 
    lcd.setCursor(0,0); 
    lcd.print("Disarmed"); 
    lcd.setCursor(0,1); 
    lcd.print("System OK"); 
    char key = keypad.getKey(); //Read key from Keypad
    if(key && key != '#'){ //Check if key is not equal to "#" 
        input += key; //Read passcode and store it in input
    }
    if(key == '#'){ //Check if "#" has been pressed on Keypad
      if(passcode == input){ //Check if the correct passcode was inputted
        locked = true; //Lock the system --> activate alarm system
        input = ""; 
        lcd.clear(); 
        lcd.setCursor(0,0);
        lcd.print("Arming");
        lcd.setCursor(0,1);
        lcd.print("Please Wait...");
        delay(2500);
        lcd.clear();
		lcd.setCursor(0,0);
        lcd.print("Premises");
        lcd.setCursor(0,1);
        lcd.print("Secured");
        delay(3500);
        lcd.clear();
      }
    }
  }
  else if(locked == true){ //Check if the system is locked.
    //digitalWrite(ledGreen, HIGH); // Turn on green LED.
    char key = keypad.getKey(); // Read key from keypad.
    if(key && key != '*'){ //Check if key from Keypad is not equal to "*"
        input += key; //Read passcode and store it in input
    }
    if(key == '*'){ //Check if "*" has been pressed on Keypad
      if(passcode == input){ //Check if the correct passcode was inputted
        locked = false; //Unlock the system --> deactivates the alarm system
        input = ""; 
        lcd.clear(); 
        delay(100);
        lcd.print("Disarming");
        lcd.setCursor(0,1);
        lcd.print("Please Wait...");
        delay(2500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("System");
        lcd.setCursor(0,1);
        lcd.print("Disarmed");
        delay(3500);
        lcd.clear();
      }
    }
    state = digitalRead(sensor1); //Check state of PIR sensor
    delay(100); 
    if(state == HIGH){ //Check if the PIR sensor has detected any motion 
      digitalWrite(ledGreen, LOW); 
      digitalWrite(ledRed, HIGH); 
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Manual Alarm!");
      lcd.setCursor(0,1);
      lcd.print("Motion Detected!"); 
      alarm(); 
      delay(50); 
      lcd.clear(); 

    }
    else{ //If no motion is detected by PIR sensor
      noTone(buzzer);
      lcd.setCursor(0,0); 
      lcd.print("Armed"); 
      lcd.setCursor(0,1); 
      lcd.print("System Safe!"); 
      digitalWrite(ledGreen, HIGH); 
      digitalWrite(ledRed, LOW); 
    }
  }
}
void alarm(){ //Function that makes the buzzer sound like an alarm
    tone(buzzer, 1500); 
}