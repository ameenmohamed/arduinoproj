//#include <NewPing.h>
 
//#define TRIGGER_PIN  12
//#define ECHO_PIN     13
//#define MAX_DISTANCE 500

int calibrationTime = 20;  
long unsigned int lowIn;         
long unsigned int pause = 5000;  
boolean lockLow = true;
boolean takeLowTime;  
const int buttonPin = 4; 
long twoMins = 1800; //120000 - 2 min , 180000 - 3 min 300000=5mins
unsigned  long twoMinpreviousTime = 0;
//int tiltState = 0;         // variable for reading the pushbutton status
//boolean senseTilt = false;
const String locID = "\"locid\":\"tvroom\"";
int switch1 = 2;
int switch2 = 5;

int pirPin = 3;    // PIR sensor's  signal high far to yellow jumper 10 sec min

const String ltons1 = "ltons1";
const String ltoffs1 = "ltoffs1";
const String ltons2 = "ltons2";
const String ltoffs2 = "ltoffs2";

 
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

 
void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(switch1, OUTPUT);
  pinMode(switch2, OUTPUT);
  digitalWrite(pirPin, LOW);
//  pinMode(buttonPin, INPUT);
  twoMinpreviousTime = millis();
   //give the sensor some time to calibrate
  //Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
   //   Serial.print(".");
      delay(100);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    digitalWrite(switch1, HIGH);
    digitalWrite(switch2, HIGH);
    delay(50);
}
 
void loop() {
  
   unsigned  long currTime = millis();
   String inComm;
 
  int motionSate = digitalRead(pirPin);  
  int lightVal = analogRead(A0); 
 // tiltState = digitalRead(buttonPin);
  //int uS = sonar.ping_cm();
  boolean isMoveDetected = isMotionDetected(motionSate);
  

     //The value to send over serial would be my string\n5 and the result would be str = "my string"
    inComm = Serial.readString();
    if(ltons1.equalsIgnoreCase(inComm)){
       digitalWrite(switch1, LOW);
    }
    if(ltoffs1.equalsIgnoreCase(inComm)){
      digitalWrite(switch1, HIGH);
    }
    
    
    
  if(isWaitTimeElapsed(motionSate,currTime)){
   // Serial.println("code:hallsecurity");
  String serialMsg = "{";
    serialMsg+=locID;
    serialMsg+=",";
    serialMsg+="\"code\":\"hallsecurity\"";
    serialMsg+=",";
    serialMsg+="\"mapSensors\":{";
    serialMsg+="\"light\":\"";serialMsg+=lightVal;
    serialMsg+="\",";
    serialMsg+="\"Motion\":\"";serialMsg+=motionSate;
    serialMsg+="\",";
//    serialMsg+="\"dist\":\"";serialMsg+=0;//uS;
//    serialMsg+="\",";
//    serialMsg+="\"Tilt\":\"";serialMsg+=0;//tiltState;
//    serialMsg+="\",";
    serialMsg+="\"Switch1\":\"";serialMsg+=digitalRead(switch1);
    serialMsg+="\",";
    serialMsg+="\"Switch2\":\"";serialMsg+=digitalRead(switch2);
    serialMsg+="\"},";
    serialMsg+="\"mid\":\"";
    serialMsg+= millis();
    serialMsg+="\",\"amsg\":\"\"}";
    
  Serial.println(serialMsg);
  }

 
}

  boolean isMotionDetected(int val) {
    if ( val == HIGH ) {
      //twoMinpreviousTime = millis();
      return true;
    } else {
      return false;
    }
  
  }

  boolean isWaitTimeElapsed(int val , unsigned long currTime) {
    unsigned long gap = currTime - twoMinpreviousTime ;
    if ( isMotionDetected(val) || gap >= twoMins) {
      twoMinpreviousTime = currTime;
    //  Serial.println("Elapsed time reset ");
      return true;
    } else {
      return false;
    }
  }
