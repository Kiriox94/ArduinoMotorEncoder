#include "motors_controller.h"

#define encoderA 2
#define encoderB 4

const int ticksForATurn = 460;

volatile int count = 0; // comptage de tick d'encoder  qui sera incrémenté sur interruption " On change " sur l'interruption matériel 0 qui se fait sur le pin 2
volatile int turns = 0; // vitesse du moteur 
volatile byte laststate = 0;  // etat précédent de l'encodeur 

void setup() {
  setupMotors();
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  attachInterrupt(0,counter, CHANGE);

  Serial.begin(9600); // Définit vitesse de transmission série

  moveMotors();
}

void loop() {
  // if(count >= 460) {
  //   turns++;
  // }

  if(turns >= 1) {
    moveMotors(motorStates::Stop);
    Serial.println(count);
    
  }
}

void counter()
{
  byte state=PIND;
  
  state|=B11101011;  // mask pour ne regarder que les changements sur 2 et 4 
  // Modifier le MASK  B01111011 par BXXXXXXXX mettre des 0 là où sont les pins utilisés par l'encodeur
  if( state!=laststate)
  {
    (((state&(1<<encoderA))>>encoderA)^((state&(1<<encoderA))>>encoderA))?count--:count++;

    if(count >= ticksForATurn) {
      turns++;
    }

    laststate=state;
  }
}