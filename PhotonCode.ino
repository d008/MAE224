#include <string.h>

Servo myservo;   // Create servo object to control a servo 
int pos = 70;    // Store the position of the servo
int servoPin = -1;
int freq =2000;  // Set the frequency of the analogWrite()
int reading[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Store the memory of the read data or the written

int memory[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Store the memory of the read data or the written
int t = 100; //Set milliseconds of averaging
int read[18] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; //Stores whether the pin is reading (1), writing (0), or uninitialized(-1) or Servo (2)
String strTemp =""; //memory as string
String strTemp2 =""; // read as string
void setup()
{
Particle.function("move",slide);                //Particle function to relate slide to move
Particle.function("attachServo",attachServo);   //Particle function to attachServo
Particle.function("detachServo",detachServo);   //Particle function to detachServo
Particle.function("setInput",setInput);         //Particle function to set the pinMode to input
Particle.function("setOutput",setOutput);       //Particle function to set the pinMode to output
Particle.function("pinMode",pMode);             //Particle function to get the pinMode where Input is 1 and Output is 0
Particle.function("getPin",getPin);             //Particle function to translate string of pin to integer number
Particle.function("analogWrite",awrite);        //Particle function to write value to analog pin
Particle.function("analogRead",aread);          //Particle function to read analog pin
Particle.function("digitalWrite",dwrite);       //Particle function to write value to digital pin
Particle.function("digitalRead",dread);         //Particle function to read value from digital pin
Particle.function("setFreq",setf);              //Particle function to set the analog write frequency
Particle.function("getPulse",getPulse);         //Particle function to get the input frequency on an analog pin
Particle.function("setAvgTime",setAvgTime);         //Particle function to get the input frequency on an analog pin

Particle.variable("position",pos);              //Particle variable to get the position of the servo
Particle.variable("frequency",freq);            //Particle variable to get the frequency of the analog write
Particle.variable("String",strTemp);            //Particle variable to get the memory string
Particle.variable("String2",strTemp2);          //Particle variable to get the read string

}

void loop()
{
    //Read through the analog pins
for(int m =0;m<t;m++)
{
    for(int n = 10;n<=17;n++)
    {
        // If the pin is a writing pin
        if(read[n] == 0)
        {
            //Write the value
            analogWrite(n,memory[n],freq);
        }
        // If the pin is a reading pin
        else if (read[n] == 1)
        {
            //Iterate t times with a 1 ms delay to average out the analog reads
                reading[n] += analogRead(n);
                delay(1);
            //Store the average value into memory
        }
    }
}
 for(int n = 10;n<=17;n++)
    {
       if (read[n] == 1)
        {
        memory[n] = int(reading[n]/t);
        reading[n] = 0;
        }
    }
    
//Read through the digital pins
for(int n = 0;n<=7;n++)
{
//If a writing pin 
    if(read[n] == 0)
    {
        //Write the value to the pin
        digitalWrite(n,memory[n]);
    }
    //If a reading pin
   else if (read[n] == 1)
    {
        //Read the value of the pin
        memory[n]=digitalRead(n);
    }
}

// Initialize null strings

    strTemp = "";
    strTemp2 = "";
    
    //Concatenate array values into the string
    for(int n = 0;n<17;n+=1)
    {
        strTemp.concat(String(memory[n]));
        strTemp.concat(',');
        strTemp2.concat(String(read[n]));
        strTemp2.concat(',');
    }
    strTemp.concat(String(memory[17]));
    strTemp2.concat(String(read[17]));


}

int slide(String angle) //spark function slide will take the string it receives (turn) and output it as "angle"
{
    myservo.write(angle.toInt()); //convert angle to an integer and 
    pos = angle.toInt();  //set position as the angle
    delay(10);
    return pos; //necessary for the servo
}

int getPin(String pin) //Translate the string of the pin to an integer
{
    if(pin.equals("A1")){
        return A1;
    }
    else if(pin.equals("A2")){
        return A2;
    }
    else if(pin.equals("A3")){
        return A3;
    }
    else if(pin.equals("A4")){
        return A4;
    }
    else if(pin.equals("A5")){
        return A5;
    }
    else if(pin.equals("A6")){
        return A6;
    }
    else if(pin.equals("A7")){
        return A7;
    }
    else if(pin.equals("A0")){
        return A0;
    }
    else if(pin.equals("D0")){
        return D0;
    }
    else if(pin.equals("D1")){
        return D1;
    }
    else if(pin.equals("D2")){
        return D2;
    }
    else if(pin.equals("D3")){
        return D3;
    }
    else if(pin.equals("D4")){
        return D4;
    }
    else if(pin.equals("D5")){
        return D5;
    }
    else if(pin.equals("D6")){
        return D6;
    }
    else if(pin.equals("D7")){
        return D7;
    }
    else{
        return -1;
    }
}

int attachServo(String pin) //Attach a servo to a pin
{
    int p = getPin(pin); //convert pin to an integer  
    if(p>-1 && read[p]==-1)
    {
        myservo.attach(p); 
        servoPin = p;
        delay(10);
        read[p] = 2;
        return p; 
    }
        return -1;
}

int detachServo(String pin) //Detach a servo to a pin
{
        myservo.detach(); //convert pin to an integer  
        read[servoPin] = -1;
        servoPin = -1;
        delay(10);
        return 1; 
}

int setInput(String pin)    //Set the pin to an input pin
{
    int p = getPin(pin);
    if(p>-1)
    {
        pinMode(p, INPUT);
        read[p] = 1;
        return 1;
    }
        return -1;
}

int setOutput(String pin)   //Set the pin to an output pin
{
    int p = getPin(pin);
    if(p>-1)
    {
        pinMode(p, OUTPUT);
        read[p] = 0;
        return 1;
    }
        return -1;
}

int pMode(String pin)       //Get whether a pin is input(1) or output(0)
{
    int p = getPin(pin);
    if (getPinMode(p)==INPUT) 
    {
      return 1;
    }
    return 0;
}

int awrite(String pin)  //Takes an input argument with syntax "pin,value" ex. "A0,255"
{
    int t = pin.indexOf(",");
    int p = getPin(pin.substring(0,t));
    //int t2 = pin.indexOf(",",t);
    int t3 = pin.lastIndexOf(",");
    String p1 = pin.substring(t3+1);
    int val = p1.toInt();
    //pinMode(p,OUTPUT);

    read[p] = 0; 
    
    memory[p] = val;
    return val;
}

int aread(String pin) //Read analog value from a pin
{
    int p = getPin(pin);
    return memory[p];

}

int dwrite(String pin) //write a value to a digital pin
{
    int t = pin.indexOf(",");
    int p = getPin(pin.substring(0,t));
    //int t2 = pin.indexOf(",",t);
    int t3 = pin.lastIndexOf(",");
    String p1 = pin.substring(t3+1);
    int val = p1.toInt();

        if(abs(val)==0){
            memory[p] = 0;
            return 0;
        }
        else{
            memory[p] = 1;
                return 1;
        }

    return val;
}

int dread(String pin)   //Read a value from a digital pin
{

    int p = getPin(pin);
    return memory[p];

}

int setAvgTime(String val)   //Read a value from a digital pin
{

    int t = val.toInt();
    return t;

}

int setf(String fre)    //Set the frequency of the analog write
{
    freq =fre.toInt();  
    return freq; 
}

double getPulse(String pin) //get the tone of the analog read
{
    int p = getPin(pin);
    double duration = pulseIn(p, HIGH);    
    duration += pulseIn(p, LOW);   
    return 1.0/(duration/1000000.0);

}
