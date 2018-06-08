#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include<NewPing.h>

#include "Types.h"
#include "Motors.h"
#include "Ultrasonic.h"

IRrecv irrecv(2);
IRsend irsend;
decode_results results;

NewPing sonar1(trig_1, echo_1, Max_Dist); 
NewPing sonar2(trig_2, echo_2, Max_Dist); 
NewPing sonar3(trig_3, echo_3, Max_Dist);

uint32 current_time = millis();
uint32 timer = millis();
uint32 timer_sensor = millis();
unsigned int state = 0;
uint8 Lt_dist;
uint8 Rt_dist;
uint8 Fwd_dist;
uint8 spd = 140; 
uint16 speed_vals;
uint16 speed_vald;
uint8 redPin = 41;
uint8 k = 10;
uint8 dist_wall = 8;
uint8 anterior_r = 10;
uint8 anterior_l = 10;
uint8 anterior_f = 10;
uint8 counter = 0;
uint8 counter_l = 0;
uint8 counter_r = 0;
uint8 counter_dance = 0;
uint8 medie = 8;

void setup() {
  // put your setup code here, to run once:
    pinMode(redPin, OUTPUT); 
    Serial.begin(9600);
}

//SYSH_30 - Right Hand Rule
void loop() {
  
    Serial.print(" Stanga: ");
    Serial.println(sonar1.ping_cm()); // senzor stanga
    Serial.print(" Dreapta:  ");
    Serial.println(sonar2.ping_cm()); //senzor dreapta
    Serial.print(" Fata:  ");
    Serial.println(sonar3.ping_cm()); //senzor fata

    //analogWrite(redPin, 255);
    Lt_dist = sonar1.ping_cm();
    Rt_dist = sonar2.ping_cm();
    Fwd_dist = sonar3.ping_cm();

    if(Lt_dist == 0)
    {
      Lt_dist = anterior_l;
    }

     if(Rt_dist == 0)
    {
      Rt_dist = anterior_r;
    }
    Serial.println("Curent0: " +String(Fwd_dist) + "Anterior0: " +String(anterior_f));
     if(Fwd_dist == 0)
    {
      Fwd_dist = anterior_f;
    }
    Serial.println("Curent1: " +String(Fwd_dist) + "Anterior1: " +String(anterior_f));
   // Serial.println("Curent1: " +String(Fwd_dist) + "Anterior1: " +String(anterior_f));
          //SYSH_14
    if(Lt_dist == 8 && Rt_dist == 8)
    {
      speed_vals = spd;
      speed_vald = spd;
      
    }
    if(Lt_dist < 8 )
    {
      //Serial.println("Lt_dist < 888888");
      //Serial.println(Lt_dist);
       speed_vals = spd + k * (dist_wall - Lt_dist);
       speed_vald = spd;
       //Serial.println(speed_vals);
       //Serial.println(speed_vald);
    }
    if(Rt_dist < 8)
    {
      //Serial.println("Rt_dist < 888888");
      //Serial.println(Rt_dist);
      speed_vald = spd + k * (dist_wall - Rt_dist);
      speed_vals = spd;
        //Serial.println(speed_vald);
        //Serial.println(speed_vals);
    }


  //SYSH_26
  if(state != 6) //daca nu e in starea de stop
  {
    if(millis() - timer_sensor < 100)
    {
    //led blink 
    digitalWrite(redPin, HIGH);
    }
    else
    {
     //led ON
    digitalWrite(redPin, LOW);
    }
    if(millis() - timer_sensor > 200)
        timer_sensor =  millis();
  }
  else
   //SYSH_27
   digitalWrite(redPin, HIGH);
   
  algo();
  anterior_f = Fwd_dist;
  anterior_r = Rt_dist;
  anterior_l = Lt_dist;
}

void go(uint16 rightmotor,uint16 leftmotor,uint16 dir)
{
  if(dir) //merge in fata
  {
     //motor dreapta
    analogWrite(MOTOR_Left_1,0);
    analogWrite(MOTOR_Right_1,rightmotor);

    //motor stanga
    analogWrite(MOTOR_Left_2,leftmotor);
    analogWrite(MOTOR_Right_2,0);  
  }
  else //merge in spate
  {
    //motor dreapta
    analogWrite(MOTOR_Left_1,leftmotor);
    analogWrite(MOTOR_Right_1,0);

    //motor stanga
    analogWrite(MOTOR_Left_2,0);
    analogWrite(MOTOR_Right_2,rightmotor);
  }
}

void turn_r()
{
  //motor dreapta
          analogWrite(MOTOR_Left_1,0);
          analogWrite(MOTOR_Right_1,70);
        //motor stanga
          analogWrite(MOTOR_Left_2,0);
          analogWrite(MOTOR_Right_2,70);
}

void turn_l()
{
        //motor dreapta
          analogWrite(MOTOR_Left_1,70);
          analogWrite(MOTOR_Right_1,0);
        //motor stanga
          analogWrite(MOTOR_Left_2,70);
          analogWrite(MOTOR_Right_2,0);
 
}

void algo()
{ 
  Serial.println("Valori anterioare: " + String(anterior_f) + " " + String(anterior_l) + " " + String(anterior_r));
  switch(state)
  {
    case 0:
    
        //SYSH_22
        if(millis() - current_time > 5000)
        { 
          Serial.println("Start");
          if((Rt_dist <= 10) &&( Fwd_dist > 12))
          {
            state = 1;
            Serial.println(state);
            Serial.println("Caz0:Merge in fata: ");
            //Serial.println(sonar3.ping_cm());
          }
          else  if((Fwd_dist < 12  && Rt_dist <= 10 && Lt_dist>12))
          {
            state = 3;
            Serial.println(state);
            Serial.println("Caz0:Merge in stanga: ");  
            //Serial.println(sonar1.ping_cm());
          }
      
          else if(Rt_dist > 12)
          {
            /*state = 2;
            Serial.println(state);
            Serial.println("Caz0:Merge in dreapta: ");*/
            //Serial.println(sonar2.ping_cm()); 

             
            counter_r++;
           // if(Rt_dist > 30)
           if(counter_r<7) //12
           {
            go(160,spd/3,1); //135
            Serial.println("2.Viraj dreapta........: counter/sensor:");
            Serial.println(counter_r);
            Serial.println(sonar2.ping_cm());
            }
           // if(Rt_dist <= 11){
           else
           {
            counter_r = 0;
              Serial.println(sonar2.ping_cm());
              state = 1;
            }        
          }
      }
      else
      {
        if(counter_dance <= 5)
        {
        
         turn_r();
         counter_dance++;
        } 

        if((counter_dance > 5) &&(counter_dance <= 10))
        {
          turn_l();
          counter_dance++;
        }
        if((counter_dance >10) && (counter_dance <= 15))
        {
          turn_r();
         counter_dance++;
        } 
        if((counter_dance > 15) &&(counter_dance <= 20))
        {
         turn_l();
         counter_dance++;
        }
         if((counter_dance > 20) && (counter_dance <= 25))
        {
          turn_r();
          counter_dance++;
        } 
        if((counter_dance > 25) &&(counter_dance <= 30))
        {
          turn_l();
          counter_dance++;
        }
        if((counter_dance > 30) && (counter_dance <= 35))
        {
          turn_r();
         counter_dance++;
        } 
        if((counter_dance > 35) &&(counter_dance <= 40))
        {
          turn_l();
          counter_dance++;
        }
        if((counter_dance > 40) && (counter_dance <= 45))
        {
          turn_r();
         counter_dance++;
        } 
        if((counter_dance > 45) &&(counter_dance <= 50))
        {
         turn_l();
          counter_dance++;
        }
        
        if(counter_dance > 50)
        {
        go(0,0,1);
        }
      }
      
      break;
      
      case 1:
      
      if(Rt_dist > 26)
      {
          if((Fwd_dist > 22) && (Fwd_dist < 29)) //17 25
          {
           go(80,80,1);
          }
          else
          {
          state = 2;
          Serial.println("1.Virare dreapta");
          }
      }
      else if((Fwd_dist < 27) && (Lt_dist > 15))
      { 
        state = 3;
        Serial.println("1.Virare stanga");
      }
      else
      {
        go(speed_vals,speed_vald,1);
        Serial.println("1.Merge in fata");
      }
     
      if( (Fwd_dist <= 12) && (Rt_dist <= 10) && (Lt_dist <= 10))
      {
        go(80,80,1);
        if(millis()-timer > 30000)
        {
          Serial.println("Timp de cand a inceput sa ruleze:");
          Serial.println(millis()-timer);
          //go(50,50,1);
          state = 5;
        }
        else
        {
          state = 4; 
        } 
      }
      break;
      
    case 2: 
     
       counter_r++;
       // if(Rt_dist > 30)
       if(counter_r<8) //12
       {
        go(160,spd/4,1); //135
        Serial.println("2.Viraj dreapta........: counter/sensor:");
        Serial.println(counter_r);
        Serial.println(sonar2.ping_cm());
        }
       // if(Rt_dist <= 11){
       else{
        counter_r = 0;
        Serial.println(sonar2.ping_cm());
        state = 1;
        }
    break;
    
    case 3: 
       counter_l++;
       //if(Lt_dist > 30) 
       if(counter_l < 8) //16
       {
          go(spd/4,160,1); //135
          Serial.println("3.Viraj stanga......: counter/sensor:");
          Serial.println(counter_l);
          Serial.println(sonar1.ping_cm());
        }
        // if(Lt_dist <= 10){
        else 
        {
          counter_l = 0;
          Serial.println("3.Merge in stare 1->fata");
          Serial.println(sonar1.ping_cm());
          state = 1;
          }
    
    break;
    
    case 4: //reverse
    
        Serial.println("TUUUUUUUUURN");
        counter++;
        if(counter < 24)
        {
          //motor dreapta
          analogWrite(MOTOR_Left_1,0);
          analogWrite(MOTOR_Right_1,80);
          //motor stanga
          analogWrite(MOTOR_Left_2,0);
          analogWrite(MOTOR_Right_2,80);
        }
        else
        {
          Serial.println("Oprireeeeeee");
          //go(0,0,0);
          state = 1;
          counter = 0;
        }

    break;
    
    case 5: //Hide
    
      //SYSH_19
      if(Fwd_dist < 8 )
      {
        state = 6;
        Serial.println("caz5:Trece in caz 6-Stop");
      }
      
    break;
    
    case 6: //Stop
    
      Serial.println("STOOOOOOOOOOOOOOOOOOOP");
      Serial.println(millis()-timer);
      //SYSH_20
      irsend.sendSony(0x76656f6e656572, 12);
      go(0,0,0);

    break;
  }
  }
  


