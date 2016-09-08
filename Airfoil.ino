//switch mux to channel 15 and read the value
 //int val = readMux(15);
 
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
int vals[32];
double ang=0.0;
//Mux in "SIG" pin
int topSig = A1;
//Mux in "SIG" pin
int botSig = A0;
int inclinometer = A2;
int topControl[] = {t0, t1, t2, t3};
int bottomControl[] = {b0, b1, b2, b3};
double avg = 1000.0;
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

char allPressures[621];
char temp[621];
  
void setup(){
  pinMode(t0, OUTPUT); 
  pinMode(t1, OUTPUT); 
  pinMode(t2, OUTPUT); 
  pinMode(t3, OUTPUT);
  
  pinMode(b0, OUTPUT); 
  pinMode(b1, OUTPUT); 
  pinMode(b2, OUTPUT); 
  pinMode(b3, OUTPUT);
  
  pinMode(topSig,INPUT);
  pinMode(botSig,INPUT);
  
  digitalWrite(t0, LOW);
  digitalWrite(t1, LOW);
  digitalWrite(t2, LOW);
  digitalWrite(t3, LOW);

  digitalWrite(b0, LOW);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
  digitalWrite(b3, LOW);
  
  for(int i = 0; i < 32; i ++){
      vals[i]=0;
  }
  Spark.variable("Pressure",allPressures,STRING);
  Spark.function("getPressure",getP);
  Spark.variable("Angle",&ang,DOUBLE);
}


void loop(){
      for(int i = 0; i < 32; i ++){
      vals[i]=0;
  }
  //Loop through and read all 32 values
  //Reports back Value at channel 6 is: 346
  for(int t = 0; t< avg;t++){
  for(int i = 0; i < 16; i++){
    vals[i] += (double)readMux(0,i);
    vals[i+16] += (double)readMux(1,i);
  }
}
    sprintf(allPressures,"");
    sprintf(temp,"");
    for(int i = 0; i < 31; i ++){
    sprintf(temp,"%s",allPressures);
    sprintf(allPressures,"%s%d,",temp,round(vals[i]/avg));
  }
    sprintf(temp,"%s",allPressures);
    sprintf(allPressures,"%s%d",temp,round(vals[31]/avg));
      
    int temp = analogRead(inclinometer);
    ang = (((double)temp/4095.0-0.5)*2.0+0.01)/0.2;
    
}


int readMux(bool isTop, int channel){
  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(topControl[i], muxChannel[channel][i]);
    digitalWrite(bottomControl[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val;
  if(isTop)
  {
  val = analogRead(topSig);
  }
  else
  {
  val = analogRead(botSig);
  }
  //return the value
  return val;
}

int getP(String args)
{
    bool isT;
    int channel = args.toInt();
    if(channel>=0 && channel<32)
    {
        isT = 0;
        if(channel>15)
        {
            isT =1;
            channel =channel-16;
        }
        return readMux(isT,channel);
    }
    else
    {
        return -1;
    }
    
}
