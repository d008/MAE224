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
int ptz[16] = {2029,2028,2033,2029,2030,2042,2044,2056,2056,2048,2051,2048,2061,2056,2057,2051};  
int pbz[16] = {2060,2060,2055,2049,2048,2052,2063,2053,2043,2132,2047,2050,2041,2040,2051,2041};
//int ptz[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
//int pbz[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



int avg = 500; //Milliseconds to average over
int vals[32];//Storage for channel reads
double aoa = 0.0; //Ang meas is with sparkfun ADXL335
int inclinometer = A2;
int incsense = 410; // Sensitivity of accelerometer in bits/1g (is ratiometric to supply voltage) 410 bits/g for Vs=3.33V
//int inczero = 0; // Bit output when AoA is zero (i.e. 0g acceleration reading on x axis)
int inczero = 2008; // Visual zero
//int inczero = 1997; // Using force balance at Lift = 0
char allPressures[621]; //String storage for Particle Cloud variables
char temp[621];
char finalPressures[621];
int dum = 0;

void setup(){
    
  Serial.begin(9600);
  Particle.variable("Pressure",finalPressures);
  Particle.variable("Angle",aoa);
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
  
  //Read in angle of attack
  int temp2 = 0;
  for(int t = 0; t< avg; t++){
  temp2 += analogRead(inclinometer);
  }
  //aoa = temp2/avg; // Use this to read aoa when setting bit-value zero
  
  aoa = (double)(90.0/incsense)*temp2/avg - (90.0/incsense)*inczero; //Output in degrees
  
  //aoa = (((double)temp/4095.0-0.5)*2.0+0.01)/0.2;
 Serial.print("Angle of Attack is ");
 Serial.println(aoa);
 

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
