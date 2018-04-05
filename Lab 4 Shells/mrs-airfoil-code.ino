// This code reads in multiplexed pressure sensors for the MAE 224 LAB
// Airfoil experiment and outputs them to the cloud in arrays
//#include "application.h"

//Mux A control pins
int acontrol[] = {D0, D1, D2, D3};
int asig = A0; //Mux A "SIG" pin
// Read these pins on MUX A
int apins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

//MUX B Control Pins
int bcontrol[] = {D4, D5, D6, D7};
int bsig = A1; //MUX B "SIG" pin
// Read these pins on MUX B
int bpins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
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
// Find total number of pins to read
int sza = sizeof(apins) / sizeof(int);
int szb = sizeof(bpins) / sizeof(int);
int szt = sza + szb;

int t = 1; //Milliseconds to average over
int vals[32];//Storage for channel reads
double aoa = 0.0; //Ang meas is with sparkfun ADXL335
int inclinometer = A2;
int incsense = 410; // Sensitivity of accelerometer in bits/1g (is ratiometric to supply voltage) 410 bits/g for Vs=3.33V
int inczero = 2048; // Bit output when AoA is zero (i.e. 0g acceleration reading on x axis)

char allPressures[621]; //String storage for Particle Cloud variables
char temp[621];

void setup(){
    
  Serial.begin(9600);
  Particle.variable("Pressure",allPressures);
  Particle.variable("numbchannel",szt);
  Particle.variable("Angle",aoa);
  pinMode(asig,INPUT);
  pinMode(bsig,INPUT);
//Set up MUX A
  for(int i = 0; i<4; i++)
    {    
      pinMode(acontrol[i], OUTPUT);
      digitalWrite(acontrol[i], HIGH);
    }
//Set up MUX B
  for(int i = 0; i<4; i++)
    {
      pinMode(bcontrol[i], OUTPUT);
      digitalWrite(bcontrol[i], HIGH);
    }
  
}


void loop(){
    
  sprintf(allPressures,"");
  sprintf(temp,""); 

  //Loop through and read all MUX A channels
  for(int i=0; i<(sza); i++)
    {
        vals[i]=0;
        vals[i] = (double)readMux(apins[i], asig, acontrol);
        /*Serial.print("Value for MUX A at channel ");
        Serial.print(apins[i]);
        Serial.print(" is: ");
        Serial.println( vals[i] );
        delay(500);
        Serial.flush();
        */

     
    }
    
  //Loop through and read all MUX B channels
  for(int i = 0; i < (szb); i++){
    vals[i+sza]=0;
      vals[i+sza] = (double)readMux(bpins[i], bsig, bcontrol);
      /*Serial.print("Value for MUX B at channel ");
      Serial.print( bpins[i] );
      Serial.print(" is : ");
      Serial.println( vals[i+sza] );
      delay(100);
      Serial.flush();
      */

  }
  
  //Write values to string      
  for(int i = 0; i < szt-1; i++){
      sprintf(temp,"%s",allPressures);
      sprintf(allPressures,"%s%d,",temp,vals[i]);  
  }
  //Write last value into final array, no delimiter afterwards
  sprintf(allPressures,"%s%d",allPressures,vals[szt]);
  
  //sprintf(finalPressures,"%s",allPressures);
  //Serial.print(finalPressures);
  
  //Read in angle of attack
  int temp2 = analogRead(inclinometer);
  aoa = (double)(90.0/incsense)*temp2 - (90.0/incsense)*inczero; //Output in degrees
  //aoa = (((double)temp/4095.0-0.5)*2.0+0.01)/0.2;
 Serial.print("Angle of Attack is ");
 Serial.println(aoa);
 

}

int readMux(int channel, int SIG_pin, int controlPin[]){
  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
    digitalWrite(controlPin[i], muxChannel[channel][i]);

  }
    delay(100);
  //read the value at the SIG pin
  int val;
  if(SIG_pin == A0)
  {
  val = analogRead(A1);
  }
  else
  {
  val = analogRead(A1);
  }
  //return the value
  Serial.println(val);
  return val;
}


int readMux2(int channel, int SIG_pin, int controlPin[]){

  //loop through the 4 pins
  for(int i = 0; i < 4; i ++){
      int dum = muxChannel[channel][i];
      int dum2 = controlPin[i];
    digitalWrite(dum2, dum);
    Serial.print(dum);
    //digitalWrite(controlPin[i], 0);
  }
Serial.println(channel);
  //read the value at the SIG pin and average over t samples
  int val = 0;
  for(int i=0; i<t; i++)
        {
            val += analogRead(SIG_pin);
              //delay(1); //Wait 1ms between samples
        }    
        Serial.println(val);    
    int fval = int(val/t);
  //return the value
 return analogRead(SIG_pin);
  //return val;
}
