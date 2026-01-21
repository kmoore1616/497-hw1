#define US1TrigPin 2 // green jumper wires
#define US1EchoPin 4 // yellow jumper wires

#define US2TrigPin 32 // green jumper wires
#define US2EchoPin 35 // yellow jumper wires 


#define BUTTON_PIN 23 // button pin D23

#define EAST_LIGHT_GREEN 0
#define WEST_LIGHT_GREEN 1
#define EAST_LIGHT_RED 2
#define WEST_LIGHT_RED 4

int previousBtnState = HIGH; 
int currentBtnState;     
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

unsigned long StartTimerVal = 0;

const int SHORT_PRESS_TIME = 500; 

bool ButtonTooggleState;


enum TrafficLightState{
  ALL_STOP, // All Traffic STOP, L1 = RED_LED + L2 = RED_LED
  STOP_EAST, // L1 = RED_LED,  L2 = GREEN_LED
  STOP_WEST, // L2 = RED_LED, L1 = GREEN_LED
  GO_EAST, // L1 = GREEN_LED, L2 = RED_LED
  GO_WEST // L2 = GREEN_LED, L1 = RED_LED
};

TrafficLightState SystemState;

void setLights(uint_8 lights){
  if(lights & EAST_LIGHT_GREEN){
    // Turn east light green
  }else if(lights & EAST_LIGHT_RED){
    // Turn east light red
  }
  if(lights & WEST_LIGHT_GREEN){
    // Turn west light green
  }else if(lights & WEST_LIGHT_RED){
    // Turn west light red
  }
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

void TrafficLight(int SignalVarTL1, int SignalVarTL2)
{
  Serial.print("Distance TL 1 in inches 1: ");
  Serial.println(SignalVarTL1);
  Serial.print("Distance TL 2 in inches 1: ");
  Serial.println(SignalVarTL2);

  buttonPress(digitalRead(BUTTON_PIN));
  // Alternate
  if(digitalRead(BUTTON_PIN, LOW)){
    // GOTO Crosswalk
    SystemState = ALL_STOP;
  }

  // State Transitions
  switch(TrafficLightState)
  { 
    case ALL_STOP: 
        // Crosswalk
        // Turn All Lights RED
        delay(60000);
      break;
    case 

  }

  // State Actions
}
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(US1TrigPin,OUTPUT);
  pinMode(US1EchoPin,INPUT);

  pinMode(US2TrigPin,OUTPUT);
  pinMode(US2EchoPin,INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int signalTL1Var = USRead1();
  int signalTL2Var = USRead2();
  TrafficLight(signalTL1Var, signalTL2Var);
  delay(1500);
}
