#include <stdint.h>

#define US1TrigPin 2 // green jumper wires
#define US1EchoPin 4 // yellow jumper wires

#define US2TrigPin 32 // green jumper wires
#define US2EchoPin 35 // yellow jumper wires 


#define BUTTON_PIN 23 // button pin D23

#define LIGHT_GREEN_WEST 25
#define LIGHT_RED_WEST 26

#define LIGHT_RED_EAST 18
#define LIGHT_GREEN_EAST 19

#define EAST_LIGHT_GREEN 1
#define WEST_LIGHT_GREEN 2

int previousBtnState = HIGH; 
int currentBtnState;     
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

unsigned long StartTimerVal = 0;

const int SHORT_PRESS_TIME = 500; 

bool ButtonTooggleState;

bool alternateStartingPattern = false;   



enum TrafficLightState{
  X_Walk,
  ALTERNATE,
  HARD_GREEN_EAST,
  HARD_GREEN_WEST,
  SOFT_GREEN_EAST,
  SOFT_GREEN_WEST
};

TrafficLightState SystemState;

hw_timer_t *timer = NULL;
volatile bool alarm_triggered = false;

/*
  To set alarm:
  timerAlarm(timer, ALARM_MS, false, 0);
  where delay_ms is the alarm time in milliseconds
*/

void ARDUINO_ISR_ATTR onTimer() {
  alarm_triggered = true;
}



int USRead1()
{
  digitalWrite(US1TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(US1TrigPin, LOW);

  int duration = pulseIn(US1EchoPin, HIGH);

  // Distance in inches:
  return(duration / 148);
}

int USRead2()
{
  digitalWrite(US2TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(US2TrigPin, LOW);

  int duration = pulseIn(US2EchoPin, HIGH);

  // Distance in inches:
  return(duration / 148);
}

void buttonPress(int sensorVal)
{
    // currentBtnState = sensorVal;
    // if(previousBtnState == HIGH && currentBtnState == LOW)        
    // {
    //     pressedTime = millis();
    // }
    // else if(previousBtnState == LOW && currentBtnState == HIGH) 
    // { 
    //     releasedTime = millis();
    //     long pressDuration = releasedTime - pressedTime;
    //     if( pressDuration < SHORT_PRESS_TIME )
    //     {
    //         if(ButtonTooggleState == false)
    //         {
    //             Serial.println("Button ON");
    //             ButtonTooggleState = true;
    //         } 
    //         else 
    //         {
    //             Serial.println("Button OFF");
    //             ButtonTooggleState = false;
    //         }
    //     }
    // }
    // // always update previous button state to the old currentBtnState before returning back to top of function call
    // previousBtnState = currentBtnState;
}

void setLights(uint8_t lights){
  if(lights & EAST_LIGHT_GREEN){
    // Turn east light green
    digitalWrite(LIGHT_GREEN_EAST,HIGH);
    digitalWrite(LIGHT_RED_EAST,LOW);
    
  }else { //if(lights & !EAST_LIGHT_GREEN){
    // Turn east light red
    digitalWrite(LIGHT_GREEN_EAST,LOW);
    digitalWrite(LIGHT_RED_EAST,HIGH);
  }
  if(lights & WEST_LIGHT_GREEN){
    // Turn west light green
    digitalWrite(LIGHT_GREEN_WEST,HIGH);
    digitalWrite(LIGHT_RED_WEST,LOW);
  }else {// if(lights & !WEST_LIGHT_GREEN){
    // Turn west light red
     digitalWrite(LIGHT_GREEN_WEST,LOW);
    digitalWrite(LIGHT_RED_WEST,HIGH);
  }
}

void TrafficLight(int SignalVarEAST, int SignalVarWEST)
{
  Serial.print("Distance TL 1 in inches 1: ");
  Serial.println(SignalVarEAST);
  Serial.print("Distance TL 2 in inches 1: ");
  Serial.println(SignalVarWEST);

  buttonPress(digitalRead(BUTTON_PIN));
  
  // Cross Walk State
  if(digitalRead(BUTTON_PIN) == LOW){
    // GOTO Crosswalk
    SystemState = X_Walk;
  }


    Serial.print("STATE:");
    Serial.println(SystemState);


  switch(SystemState){ 
    case ALTERNATE:    
        uint8_t startPattern = WEST_LIGHT_GREEN | !EAST_LIGHT_GREEN;    
        if(SignalVarEAST <= 10)
        {
          Serial.println("INCOMING CAR EAST");
          SystemState = HARD_GREEN_EAST;
        }
        else if(SignalVarWEST <= 10)
        {
          Serial.println("INCOMING CAR WEST");
            SystemState = HARD_GREEN_WEST;
        }

        if(!alternateStartingPattern)
        {
          setLights(WEST_LIGHT_GREEN | !EAST_LIGHT_GREEN);
          alternateStartingPattern = true; 
          timerAlarm(timer, 5000, true, 0);
        }
        
        if(alarm_triggered == true)
        {
          startPattern = startPattern ^ 3;
          alarm_triggered = false; 
          Serial.print("Trigger:");
        }
        // Turn on east green
        // 30 second alarm
        // Crosswalk
        // Turn All Lights RED

      break;
    // case HARD_GREEN_EAST:
    //   break;
    // case HARD_GREEN_WEST:
    //   break;
    // case X_Walk:
    //   break;
  }

}
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(US1TrigPin,OUTPUT);
  pinMode(US1EchoPin,INPUT);

  pinMode(US2TrigPin,OUTPUT);
  pinMode(US2EchoPin,INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LIGHT_RED_EAST,OUTPUT);
  pinMode(LIGHT_GREEN_EAST,OUTPUT);
  pinMode(LIGHT_RED_WEST,OUTPUT);
  pinMode(LIGHT_GREEN_WEST,OUTPUT);

  // digitalWrite(LIGHT_RED_EAST,HIGH);
  // digitalWrite(LIGHT_GREEN_EAST,HIGH);
  // digitalWrite(LIGHT_RED_WEST,HIGH);
  // digitalWrite(LIGHT_GREEN_WEST,HIGH);

  SystemState = ALTERNATE;

  timer = timerBegin(1000); // Timer goes for 1 ms per tick
  timerAttachInterrupt(timer, &onTimer);
  delay(1000);
}

void loop() {
  int signalTL1Var = USRead1();
  int signalTL2Var = USRead2();
  TrafficLight(signalTL1Var, signalTL2Var);
  delay(10);
}

