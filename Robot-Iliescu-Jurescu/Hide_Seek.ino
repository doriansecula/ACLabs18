#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include "Types.h"
#include "Motors.h"
#include "Ultrasonic.h"

IRrecv irrecv(2);
IRsend irsend;
decode_results results;

NewPing sonar1(trig_1, echo_1, Max_Dist); //left
NewPing sonar2(trig_2, echo_2, Max_Dist); //right
NewPing sonar3(trig_3, echo_3, Max_Dist); //forward

uint16 value;

int BLUE = 39;
int YELLOW = 41;
int RED = 43;

int speed_aux;
int state = 0;
int right, right_aux;
int forward, forward_aux;
int left, left_aux;
int initial_time = millis();
int var;
int back_var;
int sw;
int rotate_var;

void go(uint8 speed_right, uint8 speed_left, int flag){
  if(flag == 1) //move forward
  {
    analogWrite(MOTOR_Right_1, speed_right); 
    analogWrite(MOTOR_Right_2, 0);
    analogWrite(MOTOR_Left_1, speed_left);
    analogWrite(MOTOR_Left_2, 0);
  }
  else{
    analogWrite(MOTOR_Right_1, 0); 
    analogWrite(MOTOR_Right_2, speed_right);
    analogWrite(MOTOR_Left_1, 0);
    analogWrite(MOTOR_Left_2, speed_left);
  }
}

void rotate(uint8 speed_right, uint8 speed_left){
    analogWrite(MOTOR_Right_1, 0); 
    analogWrite(MOTOR_Right_2, speed_right);
    analogWrite(MOTOR_Left_1, speed_left);
    analogWrite(MOTOR_Left_2, 0);
}

void led(){
  digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, HIGH);
    delay(500);
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    delay(500);
}

void led_on(){
  digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, HIGH);
}

void setup() {
  // put your setup code here, to run once:

   // irrecv.enableIRIn(); // Start the receiver
    
    pingTimer1 = millis() + pingSpeed; // Sensor 1 fires after 1 second (pingSpeed)
    pingTimer2 = pingTimer1 + delay_ping; // Sensor 2 fires 35ms later
    pingTimer3 = pingTimer2 + delay_ping;
    pinMode(RED, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(BLUE, OUTPUT);
    
Serial.begin(9600);
}

void loop() {
  
speed_aux = 88 - 8*(12-left);
  
//filtram rezultatele
if((left_aux = sonar1.ping_cm()) != 0)
  left = left_aux;

if((right_aux = sonar2.ping_cm()) != 0)
  right = right_aux;
  
if((forward_aux = sonar3.ping_cm()) != 0)
  forward = forward_aux;

switch(state){
  case 0: {
    if(millis() - initial_time > 5000){
     if (right > 12)
     {
      state = 2;
     }
     else if (forward >= 20)
       {
          state = 1; 
       }
     else 
       {
        var = millis();
        state = 3;
       }
    }
     break;  
  }
  
  //move forward
  
  case 1: {
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, HIGH);
    go(speed_aux, 80, 1);
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    
    if(right > 12)
    {
      state = 2;
    }
    else if(forward < 20 && left > 12)
    {
      var = millis();
      state=3;
    }
    else if (left <= 12 && forward < 20 && right <= 12)
    {
    state=4;
    }
    break;
  }
  
  //move right
  
  case 2: {
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, HIGH);
      go(45, 165, 1);
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    
    if ( right < 12 && forward >= 20)
      {
        state = 1;
      }
      else if(left > 12 && right <= 12)
      {
        var = millis();
        state = 3;
      }
    break;  
  }
  
  // move left
  
  case 3: {
    if((millis() - var) < 1250)
    {
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, HIGH);
      go(165, 45, 1);
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    }
    else  if ( right < 12 && forward >= 20)
      {
        state = 1;
      }
      else if ( right >= 12 )
      {
        state = 2;
      }
      
    break;  
  }
  
  // hide
  case 4:{

     go(50,50,1);
     if(millis() - initial_time > 35000)
        {
          state=5;
        }
      else{
        rotate_var=millis();
        state = 6;
      }
    break;
    }
   //stop
  case 5: {
    if (forward > 4)
    {
      go(50,50,1);
    }
    led_on();
    irsend.sendSony(0x76656f6e656572, 12);
    go(0, 0, 1);
    break;
  }

   //rotate
  case 6: {
  if( (millis() - rotate_var) < 4000 )
  {
    rotate(80, 80);
  }
  state=1;
  break;
 }}}


