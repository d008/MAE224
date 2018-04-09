// This code reads in multiplexed pressure sensors for the MAE 224 LAB
// Airfoil experiment and outputs them to the cloud in arrays
//#include "application.h"


//Mux control top pins
int t0 = D0;
int t1 = D1;
int t2 = D2;
int t3 = D3;
//Mux control bottom pins
int b0 = D4;
int b1 = D5;
int b2 = D6;
int b3 = D7;


//Mux Top control pins
int topControl[] = {t0, t1, t2, t3};

int tsig = A0; //Mux A "SIG" pin
// Read these pins on MUX A

//MUX Bottom Control Pins
int bottomControl[] = {b0, b1, b2, b3};
int bsig = A1; //MUX B "SIG" pin
// Read these pins on MUX B
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
//Zeros, saved here so we can update them if needed outside the photon.m file    
int ptz[16] = {2055,2045,2045,2053,2061,2050,2052,2051,2046,2050,2056,2056,2057,2049,2042,2046};  
int pbz[16] = {2051,2054,2054,2055,0,2055,2053,2051,0,0,0,0,0,0,0,0};
//int ptz[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
//int pbz[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



int avg = 200; //Milliseconds to average over
int vals[32];//Storage for channel reads

char allPressures[621]; //String storage for Particle Cloud variables
char temp[621];
char finalPressures[621];
int dum = 0;

void setup(){
    
  Serial.begin(9600);
  Particle.variable("Pressure",finalPressures);
  pinMode(tsig,INPUT);
  pinMode(bsig,INPUT);
//Set up MUX A
  for(int i = 0; i<4; i++)
    {    
      pinMode(topControl[i], OUTPUT);
      digitalWrite(topControl[i], LOW);
    }
//Set up MUX B
  for(int i = 0; i<4; i++)
    {
      pinMode(bottomControl[i], OUTPUT);
      digitalWrite(bottomControl[i], LOW);
    }

}


void loop(){
    for(int i = 0; i < 32; i ++){
      vals[i]=0;
    }
  dum = 0;    
  sprintf(allPressures,"");
  sprintf(temp,""); 

  //Loop through and read all Muxer channels
  for(int t = 0; t< avg;t++){
  for(int i = 0; i < 16; i++){
    vals[i] += (double)readMux(1,i);
    vals[i+16] += (double)readMux(0,i);
     }
    }
    sprintf(allPressures,"");
    sprintf(temp,"");
    for(int i = 0; i < 31; i ++){
    sprintf(temp,"%s",allPressures);
    if(i<16){
       dum = round( vals[i]/avg) - ptz[i];
    }
    else{
       dum = round(vals[i]/avg) - pbz[i-16];
    }
    sprintf(allPressures,"%s%d,",temp,dum);
  }
    sprintf(temp,"%s",allPressures);
    dum = round(vals[31]/avg) - pbz[15];
    sprintf(finalPressures,"%s%d",temp,dum);
      
  //sprintf(finalPressures,"%s",allPressures);
  //Serial.print(finalPressures);
  


}

int readMux(bool isTop, int channel){
  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(topControl[i], muxChannel[channel][i]);
    digitalWrite(bottomControl[i], muxChannel[channel][i]);

  }
   // Serial.println(channel);
  //read the value at the SIG pin
  int val;
  if(isTop)
  {
  val = analogRead(tsig);
  }
  else
  {
  val = analogRead(bsig);
  }
  //return the value
  Serial.println(val);
  return val;
}
