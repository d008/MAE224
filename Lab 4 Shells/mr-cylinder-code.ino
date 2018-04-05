// This code reads in multiplexed pressure sensors for the MAE 224 LAB
// Cylinder experiment and outputs them to the cloud in arrays
#include "application.h"

//Mux A control pins
int acontrol[] = {D0, D1, D2, D3};
int asig = A0; //Mux A "SIG" pin
    //MUX A: CH 0-7 are 2.5kPa TOP; 8-11 1kPa TOP
    //MUX A: CH 12-15 are 1kPa Bottom
    //MUX B: CH 0-7 are 2.5 kPa Bottom
    //Last transducers on top and bottom not connected
// Read these pins on MUX A
int apins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

//MUX B Control Pins
int bcontrol[] = {D4, D5, D6, D7};
int bsig = A1; //MUX B "SIG" pin
// Read these pins on MUX B
int bpins[] = {0, 1, 2, 3, 4, 5, 6, 7};

// Find total number of pins to read
int sza = sizeof(apins) / sizeof(int);
int szb = sizeof(bpins) / sizeof(int);
int szt = sza + szb;

int t = 200; //Milliseconds to average over
int vals[32];//Storage for channel reads

char allPressures[621]; //String storage for Particle Cloud variables
char temp[621];
char finalPressures[621];

void setup(){
    
  Serial.begin(9600);
  Particle.variable("Pressure",finalPressures,STRING);
  Particle.variable("numbchannel",szt);

//Set up MUX A
  for(int i = 0; i<4; i++)
    {    
      pinMode(acontrol[i], OUTPUT);
      digitalWrite(acontrol[i], LOW);
    }
//Set up MUX B
  for(int i = 0; i<4; i++)
    {
      pinMode(bcontrol[i], OUTPUT);
      digitalWrite(bcontrol[i], LOW);
    }
  
}


void loop(){
    
  sprintf(allPressures,"");
  sprintf(temp,""); 

  //Loop through and read all MUX A channels
  for(int i=0; i<(sza); i++)
    {
        
        Serial.print("Value for MUX A at channel ");
        Serial.print(apins[i]);
        Serial.print(" is: ");
        vals[i] = (double)readMux(apins[i], asig, acontrol);
        Serial.println( vals[i] );
        delay(500);
        Serial.flush();

     
    }
    
  //Loop through and read all MUX B channels
  for(int i = 0; i < (szb); i++){
      Serial.print("Value for MUX B at channel ");
      Serial.print( bpins[i] );
      Serial.print(" is : ");
      vals[i+sza] = (double)readMux(bpins[i], bsig, bcontrol);
      Serial.println( vals[i+sza] );
      delay(500);
      Serial.flush();

  }
  
  //Write values to string      
  for(int i = 0; i < szt; i++){
      sprintf(temp,"%s",allPressures);
      sprintf(allPressures,"%s%d",temp,vals[i]);
    }
  
  sprintf(finalPressures,"%s",allPressures);
}


int readMux(int channel, int SIG_pin, int controlPin[]){

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
    };

  //loop through the 4 pins
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin and average over t samples
  int val = 0;
  for(int i=0; i<t; i++)
        {
            val += analogRead(SIG_pin);
            delay(1); //Wait 1ms between samples
        }    
    int fval = int(val/t);    
  //return the value
  return fval;
}
