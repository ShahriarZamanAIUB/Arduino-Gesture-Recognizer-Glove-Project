#include "Wire.h"
#include<SoftwareSerial.h>
#include "C:/Users/Asus/Downloads/mpu6050/mpu6050/mpu6050-master/src/I2Cdev.h"
#include "C:/Users/Asus/Downloads/mpu6050/mpu6050/mpu6050-master/src/MPU6050.h"

#include "C:/Users/Asus/Downloads/mpu6050/mpu6050/mpu6050-master/src/I2Cdev.cpp"
#include "C:/Users/Asus/Downloads/mpu6050/mpu6050/mpu6050-master/src/MPU6050.cpp"

#include "C:/Users/Asus/Downloads/Compressed/Paj7620/PAJ7620/paj7620.h"
#include "C:/Users/Asus/Downloads/Compressed/Paj7620/PAJ7620/paj7620.cpp"

 


#include <LiquidCrystal_I2C.h>

 int  delayShortener=0;
int columnAt=0, rowAt=0, repeatDelay=700;
bool soundSensed=false;

SoftwareSerial bluetoothModule(10,11);


MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

int indexFingerValue, middleFingerValue, thumbFingerValue, ringFingerValue, littleFingerValue;

struct MyData {
  byte X;
  byte Y;
  byte Z;

  byte gX;
  byte gY;
  byte gZ;
};

MyData data;

bool capsLock;
  String text;

  

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:




    Wire.begin();
  mpu.initialize();
    lcd.begin();
    pinMode(13,OUTPUT);
 //   pinMode(11,INPUT);
 //   pinMode(10,INPUT);
    pinMode(9,OUTPUT);
    pinMode(8,INPUT);
    pinMode(7,INPUT);
    pinMode(6,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(2,OUTPUT);

    digitalWrite(13,LOW);
    
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0);
 //printWelcomeMessage();
  lcd.clear();
  capsLock=true;
  text="";
    Serial.begin(9600);  //start serial port
    bluetoothModule.begin(9600);


      int error = paj7620Init();
	if(error) {
    Serial.print(F("(PAJ7620) Initialisation error code: "));
    Serial.println(error);
	}
	else {
		Serial.println(F("PAJ7620 is Ready!"));
	}



    TCCR0A = 0b00000000;
TCCR0B = 0b00000101; 
//setting pre-scaler for timer clock
TCNT0=0;

//attachInterrupt(digitalPinToInterrupt(7),bluetooth,CHANGE);
/* 
cli();

TCCR1A=0;
TCCR1B=0;

TCCR1B= 0b00000100;

TIMSK1= 0b00000010;

OCR1A=31250;

sei();
 */

 // attachInterrupt(digitalPinToInterrupt(8), loudNoiseDetected, FALLING);
 
}


/*
ISR(TIMER1_COMPA_vect)
{
  TCNT1=0;
  Serial.println("ISR Hello");

       if (digitalRead(8)==1) 
  {
      Serial.println("ISR sensed");
    soundSensed=true;
 }
 

}
*/

void(* resetFunc) (void) = 0;



void bluetoothSend()
{
Serial.println("BL Called");

if(text==""){  
  bluetoothModule.print("No Text");
  bluetoothModule.print(";");
  
  }

  else{
  bluetoothModule.print(text);
  bluetoothModule.print(";");
  lcd.clear();
  printBigMessage("Message Sent    !!!!!!!!!!!!!!!!");  
  delay_timer(1500); 

  
 lcd.clear(); printBigText(text);
  }
}

void bluetoothReceive()
{
String message="";
 char c;

 if(bluetoothModule.available()>0)
 { lcd.clear();
   digitalWrite(9,HIGH);

  while(bluetoothModule.available()>0)
  {
    c=bluetoothModule.read();
    message+=c;
    delay_timer(20);
  }
 /* printReceivedMessage("Message Received!!!!!!!!!!!!!!!!  "); */digitalWrite(13,HIGH); delay_timer(500); digitalWrite(13,LOW); digitalWrite(9,LOW); 

 /* printReceivedMessage(message); */

   delay_timer(3000);lcd.clear();  //printBigText(text);
   
 }

}

void printBigText(String txt)
{ lcd.clear();
  lcd.setCursor(0, 0);
   lcd.print(txt);

   if(txt.length()>16){ 
   lcd.setCursor(0, 1); 
   lcd.print(adjustText(txt));}
}


void printReceivedMessage(String txt)
{ lcd.clear();
  lcd.setCursor(0, 0);
   lcd.print(txt.substring(0,16));

   if(txt.length()>16)
   { 
   lcd.setCursor(0, 1); 
   lcd.print(txt.substring(16));
   
   lcd.setCursor(txt.substring(16).length()-2,1);
   lcd.print(" ");

   lcd.setCursor(txt.substring(16).length()-1,1);
   lcd.print(" ");
   
   return;
   }

   lcd.setCursor(txt.length()-2,0);
   lcd.print(" ");

   lcd.setCursor(txt.length()-1,0);
   lcd.print(" ");
     return;

}

void printBigMessage(String txt)
{ lcd.clear();
  lcd.setCursor(0, 0);
   lcd.print(txt.substring(0,16));

   if(txt.length()>16 )
   { 
   lcd.setCursor(0, 1); 
   lcd.print(txt.substring(16));

 

 
   
   return;
   }



 
     return;

}




String checkLeftHand()
{ 
  // Create a variable to hold the value of any gesture recognised
	byte gesture;
  // Error variable holds any error code
	int Error;
  // Read Reg 0x43 of Bank 0 to get result of any recognised gesture, and store in 'gesture' variable
	Error = paj7620ReadReg(0x43, 1, &gesture);
 
	if(!Error) {
		switch (gesture) {
			case GES_RIGHT_FLAG:
			  Serial.println(F("Up"));          
				break;
			case GES_LEFT_FLAG: 
				Serial.println(F("Down"));         
				break;
			case GES_UP_FLAG:
				Serial.println(F("Left"));        
				break;
			case GES_DOWN_FLAG:
				Serial.println(F("Right"));         
				break;
			case GES_FORWARD_FLAG:
				Serial.println(F("Forward"));
				break;
			case GES_BACKWARD_FLAG:		  
				Serial.println(F("Backward"));
				break;

 


      // Library also defines GES_CLOCKWISE_FLAG GES_COUNT_CLOCKWISE_FLAG and GES_WAVE_FLAG, but I found these unreliable
			default:
				break;
		}
	}
 else {
    Serial.print(F("Error code: "));
    Serial.println(Error);

   //  TWCR = 0;
//Wire.begin();
 }
  // Introduce small delay before next polling the sensor 
 // delay(100);
}

void backSpace()
{
     // if(text.length()<=16){
      text.remove(text.length()-1,1); 
 
      lcd.clear(); lcd.setCursor(0,0); lcd.print(text);  
      
      lcd.setCursor(columnAt, rowAt); lcd.print(adjustText(text));
   // }
         //  delayShortener=350;
  
       if(text==""){capsLock=true; rowAt=0; }
} 

void printWelcomeMessage()
{

  lcd.print(" Welcome To Our");
  lcd.setCursor(0, 1);
  lcd.print(" Micro Project");
  delay(3500);
  lcd.clear();


  lcd.print("   By MAES (G)");
  lcd.setCursor(0, 1);
  lcd.print("  Group No. 5");
  delay(3500);
  lcd.clear();

}


int rateIndexFingerValue(long value) {
  if (value >= 25) {
    return 1;
  }

  else if (value >= 20 && value < 25) {
    return 2;
  }

  else if (value < 20) {
    return 3;
  }
}

int rateMiddleFingerValue(long value) {
  if (value >= 27) {
    return 1;
  }

  else if (value >= 22 && value < 27) {
    return 2;
  }

  else if (value < 22) {
    return 3;
  }
}

int rateThumbFingerValue(long value) {
  if (value >= 28) {
    return 1;
  }

  else if (value >= 22 && value < 28) {
    return 2;
  }

  else if (value < 22) {
    return 3;
  }
}


int rateRingFingerValue(long value) {
  if (value >= 33) {
    return 1;
  }

  else if (value >= 19 && value < 33) {
    return 2;
  }

  else if (value < 19) {
    return 3;
  }
}



int rateLittleFingerValue(long value) {
  if (value >= 26) {
    return 1;
  }

  else if (value >= 16 && value < 26) {
    return 2;
  }

  else if (value < 16) {
    return 3;
  }
}





 

int rateHandPositionValue(byte ax, byte ay, byte az)
{  
  if(ax>30 && ax <210 && ay>30 && ay <170 && az>210 || az <20 ) // Palm facing forward
  {return 1;}

  else if(ax>210 && ax <260 && ay>40 && ay <180 && az>=70 && az <=235 ) // Palm facing down
  {return 2;}

    else if(ax>40 && ax <210 && ay>0 && ay <45 && az>=120 && az <=190 ) // Palm facing left
  {return 3;}

  else if(ax>0 && ax <50 && ay>50 && ay <190 && az>=120 && az <=220 ) // Palm facing up
  {return 4;}

  else if(ax>90 && ax <220 && ay>200 && ay <260 && az>=130 && az <=190 ) // Palm facing right
  {return 5;}

   else if(ax>110 && ax <220 && ay>110 && ay <180 && az>=20 && az <=70 ) // Palm facing backward
  {return 6;}

  else
  {return 0;}
}


int delay_timer (int milliseconds)
{
int count = 0;
while(1)
{
if(TCNT0 >= 16) 
 // Checking if 16 milliseconds have passed
{TCNT0=0;
count++; 

      if (digitalRead(8)==1 && digitalRead(7)==0 ) 
  {
      //backSpace();
     Serial.println("Loud Noise      Detected dl!!!!!!!!   "); 
     printBigMessage("Loud Noise       Detected!!!!!!!!!!!!!!!!");  

    digitalWrite(9,HIGH); digitalWrite(13,HIGH); delay(50); digitalWrite(9,LOW); digitalWrite(13,LOW); 

    printBigText(text);
    
  //  soundSensed=true;

 } 
 
//else{ // delayShortener=0; 
  //    digitalWrite(9,LOW); digitalWrite(13,LOW);
//    }

if (count == milliseconds) 
 //checking if required milliseconds delay //has passed
{count=0;
break; // exits the loop
}}}





return 0;
}




String rateCase(String  A)
{
   if(capsLock==true){A.toUpperCase();   }

   else{   A.toLowerCase(); }

    return A;
}

bool matchCase(String A, String B)
{
 

  if(A.equalsIgnoreCase(B)==true)
  {
    return true;
  }
  else
  {
    return false;
  }

}

bool checkIfAlreadyPrintedTwice(String txt, char myChar)
{

if(txt.length()<1 )
{
  return false;
}

else if((txt.charAt(txt.length()-1)==myChar || txt.charAt(txt.length()-1)==tolower(myChar)) && (txt.charAt(txt.length()-2)==myChar  || txt.charAt(txt.length()-2)==tolower(myChar)))
{
  return true;
}

else{return false;}
}


bool checkIfAlreadyPrintedOnce(String txt, char myChar)
{

if(txt.length()<1 )
{
  return false;
}

else if(((txt.charAt(txt.length()-1)==myChar) || (txt.charAt(txt.length()-1)==tolower(myChar))  ))
{
  return true;
}

else{return false;}
}


String adjustText(String s)
{
  if(rowAt==1){ return s.substring(16); }
  else{return s;}
}

bool noFingersConnected()
{
   if(digitalRead(2)!=1 && digitalRead(3)!=1 && digitalRead(4)!=1 && digitalRead(5)!=1)
   {return true; }
   else{return false;}

}


bool anyFingersConnected()
{
   if(digitalRead(2)==1 || digitalRead(3)==1  || digitalRead(5)==1)
   {return true; }
   else{return false;}

}


bool isA()
{ 

   if (rateIndexFingerValue(indexFingerValue) == 3 && rateMiddleFingerValue(middleFingerValue) == 3 && 
       rateRingFingerValue(ringFingerValue) == 3   && rateLittleFingerValue(littleFingerValue) == 3 && 
      rateThumbFingerValue(thumbFingerValue) == 1 && rateHandPositionValue(data.X, data.Y, data.Z)==1) 
      {
         return true;
      }

  else{ return false;}

}

bool isB()
{ 
  
     if (rateIndexFingerValue(indexFingerValue) == 1 && 
         rateThumbFingerValue(thumbFingerValue) >=2 && 
         rateMiddleFingerValue(middleFingerValue) == 1 && 
         rateRingFingerValue(ringFingerValue) == 1 && 
         rateLittleFingerValue(littleFingerValue) == 1) 
      {
         return true;
      }

  else{ return false;}


  
}

bool isC()
{ 

if (noFingersConnected()==true && rateIndexFingerValue(indexFingerValue) == 2  && 
    rateThumbFingerValue(thumbFingerValue) == 2 && rateMiddleFingerValue(middleFingerValue) == 2 && 
    rateRingFingerValue(ringFingerValue) == 2  && rateLittleFingerValue(littleFingerValue) == 2)
    {return true;}

else{ return false; }           
  
}

bool isD()
{ 

if (digitalRead(3)==1 &&  digitalRead(2)!=1  && rateHandPositionValue(data.X, data.Y, data.Z)==1 &&
    rateIndexFingerValue(indexFingerValue) == 1      && rateRingFingerValue(ringFingerValue) > 1  &&  
    rateLittleFingerValue(littleFingerValue) > 1)
    {
      return true;
    }

    else{ return false; }  
  
}

bool isE()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==3 && digitalRead(2)!=1 && rateIndexFingerValue(indexFingerValue) >= 2  && 
               rateThumbFingerValue(thumbFingerValue) >= 2 && rateMiddleFingerValue(middleFingerValue) == 1 && 
                rateRingFingerValue(ringFingerValue) == 1  && rateLittleFingerValue(littleFingerValue) == 1)
    {
      return true;
    }

else{ return false; }  
  
}

bool isF()
{ 
if (digitalRead(2)==1 && digitalRead(3)!=1  && rateMiddleFingerValue(middleFingerValue) == 1 
    && rateRingFingerValue(ringFingerValue) == 1  && rateLittleFingerValue(littleFingerValue) == 1 
    && rateHandPositionValue(data.X, data.Y, data.Z)!=3)
    {
      return true;
    }

else{ return false; } 
  
}

bool isG()
{ 
if (rateIndexFingerValue(indexFingerValue) == 2 && digitalRead(3)==0 && rateMiddleFingerValue(middleFingerValue) == 3 
              && rateThumbFingerValue(thumbFingerValue) < 3 && rateRingFingerValue(ringFingerValue) == 3 && rateLittleFingerValue(littleFingerValue) == 3 
              && rateHandPositionValue(data.X, data.Y, data.Z)==3) 
    {
      return true;
    }

else{ return false; } 
  
}

bool isH()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==3 &&
              rateIndexFingerValue(indexFingerValue) == 1 && 
              rateMiddleFingerValue(middleFingerValue) == 1 && 
              rateThumbFingerValue(thumbFingerValue) > 1 && 
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue) == 3)
    {
      return true;
    }

else{ return false; } 
  
}

bool isI()
{ 
if (rateIndexFingerValue(indexFingerValue) == 3 && rateMiddleFingerValue(middleFingerValue) == 3 && 
    rateThumbFingerValue(thumbFingerValue) >= 2 && rateRingFingerValue(ringFingerValue) == 3 && 
    rateLittleFingerValue(littleFingerValue) == 1)
    {
      return true;
    }

else{ return false; } 
  
}

bool isJ()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==3 && rateIndexFingerValue(indexFingerValue) == 3 && rateMiddleFingerValue(middleFingerValue) == 3 
         && rateThumbFingerValue(thumbFingerValue) > 1 && rateRingFingerValue(ringFingerValue) == 3 && 
         rateLittleFingerValue(littleFingerValue) == 2)
    {
      return true;
    }

else{ return false; } 
  
}

bool isK()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==1 && rateIndexFingerValue(indexFingerValue) == 1 && rateMiddleFingerValue(middleFingerValue) == 1 
     && rateThumbFingerValue(thumbFingerValue) == 1 && rateRingFingerValue(ringFingerValue) == 3 && rateLittleFingerValue(littleFingerValue) == 3)
    {
      return true;
    }

else{ return false; } 
  
}


bool isL()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==1 && rateIndexFingerValue(indexFingerValue) == 1 && rateMiddleFingerValue(middleFingerValue) == 3 && rateThumbFingerValue(thumbFingerValue) == 1 && rateRingFingerValue(ringFingerValue) == 3 && rateLittleFingerValue(littleFingerValue) == 3)
    {
      return true;
    }

else{ return false; } 
  
}


bool isM()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==3 &&
              rateIndexFingerValue(indexFingerValue) == 2 && 
              rateMiddleFingerValue(middleFingerValue) == 2 && 
              rateThumbFingerValue(thumbFingerValue) > 0 && 
              rateRingFingerValue(ringFingerValue) == 2 && 
              rateLittleFingerValue(littleFingerValue) == 3)
    {
      return true;
    }

else{ return false; } 
  
}


bool isN()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==3 &&
              rateIndexFingerValue(indexFingerValue) == 2 && 
              rateMiddleFingerValue(middleFingerValue) == 2 && 
              rateThumbFingerValue(thumbFingerValue) > 0 && 
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue) == 3)
    {
      return true;
    }

else{ return false; } 
  
}


bool isO()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==1 && digitalRead(2)==HIGH && rateIndexFingerValue(indexFingerValue) >= 2  && 
    rateThumbFingerValue(thumbFingerValue) == 2 && rateMiddleFingerValue(middleFingerValue) >= 2 && 
    rateRingFingerValue(ringFingerValue) >= 2  && rateLittleFingerValue(littleFingerValue) >= 2)
    {
      return true;
    }

else{ return false; } 
  
}


bool isP()
{ 
if (digitalRead(3)==1 &&  digitalRead(2)!=1  && rateHandPositionValue(data.X, data.Y, data.Z)==2 &&
    rateIndexFingerValue(indexFingerValue) == 1      && rateRingFingerValue(ringFingerValue) >= 2  &&  
    rateLittleFingerValue(littleFingerValue) >= 2)
    {
      return true;
    }

else{ return false; } 
  
}


bool isQ()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==2 && rateIndexFingerValue(indexFingerValue) == 1 && 
          rateMiddleFingerValue(middleFingerValue) == 3 && rateThumbFingerValue(thumbFingerValue) == 1 && 
          rateRingFingerValue(ringFingerValue) == 3 && rateLittleFingerValue(littleFingerValue) == 3)
    {
      return true;
    }

else{ return false; } 
  
}


bool isR()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==3 &&
              rateIndexFingerValue(indexFingerValue) == 1 && 
              rateMiddleFingerValue(middleFingerValue) == 1 && 
              rateThumbFingerValue(thumbFingerValue) > 0 && 
              rateRingFingerValue(ringFingerValue) == 1 && 
              rateLittleFingerValue(littleFingerValue) > 1)
    {
      return true;
    }

else{ return false; } 
  
}

bool isS()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==4 &&
              rateIndexFingerValue(indexFingerValue) == 2 && 
              rateMiddleFingerValue(middleFingerValue) == 2 && 
              rateThumbFingerValue(thumbFingerValue) > 1 && 
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue) == 3)
    {
      return true;
    }

else{ return false; } 
  
}



bool isT()
{ 
if ( rateHandPositionValue(data.X, data.Y, data.Z)==3  &&
              rateThumbFingerValue(thumbFingerValue) ==1 && 
              rateIndexFingerValue(indexFingerValue) == 2 && 
              rateMiddleFingerValue(middleFingerValue) == 3 && 
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue) ==3)
    {
      return true;
    }

else{ return false; } 
  
}


bool isU()
{ 
if (  rateIndexFingerValue(indexFingerValue) == 1 && rateMiddleFingerValue(middleFingerValue) >= 2 
      && rateThumbFingerValue(thumbFingerValue) >= 1 && rateRingFingerValue(ringFingerValue) == 1 
      && rateLittleFingerValue(littleFingerValue) >= 2)
    {
      return true;
    }

else{ return false; } 
  
}


bool isV()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==1 && rateIndexFingerValue(indexFingerValue) == 1 
            && rateMiddleFingerValue(middleFingerValue) == 1 && rateThumbFingerValue(thumbFingerValue) > 1 
            && rateRingFingerValue(ringFingerValue) >= 2 && rateLittleFingerValue(littleFingerValue) >=2)
    {
      return true;
    }

else{ return false; } 
  
}


bool isW()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==1 && rateIndexFingerValue(indexFingerValue) == 1 
            && rateMiddleFingerValue(middleFingerValue) == 1 && rateThumbFingerValue(thumbFingerValue) > 0 
            && rateRingFingerValue(ringFingerValue) == 1 && rateLittleFingerValue(littleFingerValue) >=2)
    {
      return true;
    }

else{ return false; } 
  
}


bool isX()
{ 
if ( rateHandPositionValue(data.X, data.Y, data.Z)==1 &&
              rateThumbFingerValue(thumbFingerValue) ==1 && 
              rateIndexFingerValue(indexFingerValue) == 2 && 
              rateMiddleFingerValue(middleFingerValue) == 3 && 
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue) ==3)
    {
      return true;
    }

else{ return false; } 
  
}

bool isY()
{ 
if (rateIndexFingerValue(indexFingerValue) >= 2 && rateMiddleFingerValue(middleFingerValue) >= 2 
        && rateThumbFingerValue(thumbFingerValue) == 1 && rateRingFingerValue(ringFingerValue) == 3 
        && rateLittleFingerValue(littleFingerValue) == 1)
    {
      return true;
    }

else{ return false; } 
  
}


bool isZ()
{ 
if (       
              digitalRead(2) == 1 && digitalRead(3) == 0 && 
              rateMiddleFingerValue(middleFingerValue) == 1 &&
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue)==3)
    {
      return true;
    }

else{ return false; } 
  
}

bool is1()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 1 &&
              rateThumbFingerValue(thumbFingerValue) >= 2 &&
              rateMiddleFingerValue(middleFingerValue) == 3 &&
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue)==3)
    {
      return true;
    }

else{ return false; } 
  
}

bool is2()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 1 &&
              rateThumbFingerValue(thumbFingerValue) >= 2 &&
              rateMiddleFingerValue(middleFingerValue) == 2 &&
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue)==3)
    {
      return true;
    }

else{ return false; } 
  
}


bool is3()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 1 &&
              rateThumbFingerValue(thumbFingerValue) >= 2 &&
              rateMiddleFingerValue(middleFingerValue) == 1 &&
              rateRingFingerValue(ringFingerValue) == 1 && 
              rateLittleFingerValue(littleFingerValue)==3)
    {
      return true;
    }

else{ return false; } 
  
}


bool is4()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 1 &&
              rateThumbFingerValue(thumbFingerValue) >= 2 &&
              rateMiddleFingerValue(middleFingerValue) == 1 &&
              rateRingFingerValue(ringFingerValue) == 1 && 
              rateLittleFingerValue(littleFingerValue)==1)
    {
      return true;
    }

else{ return false; } 
  
}

bool is5()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 1 &&
              rateThumbFingerValue(thumbFingerValue) == 1 &&
              rateMiddleFingerValue(middleFingerValue) == 1 &&
              rateRingFingerValue(ringFingerValue) == 1 && 
              rateLittleFingerValue(littleFingerValue)==1)
    {
      return true;
    }

else{ return false; } 
  
}

bool is6()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 3 &&
              rateThumbFingerValue(thumbFingerValue) == 1 &&
              rateMiddleFingerValue(middleFingerValue) == 3 &&
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue)==3)
    {
      return true;
    }

else{ return false; } 
  
}

bool is7()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 1 &&
              rateThumbFingerValue(thumbFingerValue) == 1 &&
              rateMiddleFingerValue(middleFingerValue) == 3 &&
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue)==3)
    {
      return true;
    }

else{ return false; } 
  
}


bool is8()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 1 &&
              rateThumbFingerValue(thumbFingerValue) == 1 &&
              rateMiddleFingerValue(middleFingerValue) == 1 &&
              rateRingFingerValue(ringFingerValue) == 3 && 
              rateLittleFingerValue(littleFingerValue)==3)
    {
      return true;
    }

else{ return false; } 
  
}

bool is9()
{ 
if (         rateHandPositionValue(data.X, data.Y, data.Z)==4 && 
              noFingersConnected()==true &&
              rateIndexFingerValue(indexFingerValue) == 1 &&
              rateThumbFingerValue(thumbFingerValue) == 1 &&
              rateMiddleFingerValue(middleFingerValue) == 1 &&
              rateRingFingerValue(ringFingerValue) == 1 && 
              rateLittleFingerValue(littleFingerValue)==3)
    {
      return true;
    }

else{ return false; } 
  
}

bool is0()
{ 
if (rateHandPositionValue(data.X, data.Y, data.Z)==2 && digitalRead(2)==HIGH && rateIndexFingerValue(indexFingerValue) >= 2  && 
    rateThumbFingerValue(thumbFingerValue) == 2 && rateMiddleFingerValue(middleFingerValue) >= 2 && 
    rateRingFingerValue(ringFingerValue) >= 2  && rateLittleFingerValue(littleFingerValue) >= 2)
    {
      return true;
    }

else{ return false; } 
  
}

void printAllValues()
{
    Serial.print("Thumb Finger : ");
  //Serial.print( rateThumbFingerValue(thumbFingerValue));
  Serial.print(thumbFingerValue);
   //lcd.print(rateThumbFingerValue(thumbFingerValue));
  Serial.print("  ");



  Serial.print("Index Finger : ");
 // Serial.print(rateIndexFingerValue(indexFingerValue));
  Serial.print(indexFingerValue);

  Serial.print("  ");

  Serial.print("Middle Finger : ");
 // Serial.print(rateMiddleFingerValue(middleFingerValue));
  Serial.print(middleFingerValue);
  Serial.print("  ");

    Serial.print("Ring Finger : ");
  //Serial.print(rateRingFingerValue(ringFingerValue));
 Serial.print(ringFingerValue);
  Serial.print("  ");


    Serial.print("Little Finger : ");
 // Serial.print(rateLittleFingerValue(littleFingerValue));
  Serial.print(littleFingerValue);
  Serial.print("  ");



  
   Serial.print("X = ");
  Serial.print(data.X);
 /* lcd.setCursor(0, 0);
   lcd.print("X");
  lcd.print(data.X);
 
  lcd.print(" ");
  
  */
  
  Serial.print("  ");
  
  Serial.print("Y = ");
  Serial.print(data.Y);
  /*  lcd.setCursor(5, 0);
   lcd.print("Y");
  lcd.print(data.Y);
  lcd.print(" ");
  */

  Serial.print("  ");
  Serial.print("Z  = ");
  Serial.print(data.Z);
 /*   lcd.setCursor(10, 0);
   lcd.print("Z");
  lcd.print(data.Z);
  lcd.print(" ");
*/

  //lcd.clear();
 
  Serial.print(" gX = ");
  Serial.print(data.gX);
  Serial.print("  ");
  Serial.print("gY = ");
  Serial.print(data.gY);
  Serial.print("  ");
  Serial.print("gZ  = ");
  Serial.print(data.gZ);
 

 /*
  lcd.setCursor(0,0);
  lcd.print(data.X);
  lcd.print(" ");
   lcd.setCursor(5,0);
  lcd.print(data.Y);
  lcd.print(" ");
   lcd.setCursor(9,0);
  lcd.print(data.Z);


  lcd.setCursor(0,1);
  lcd.print(data.gX);
  lcd.print(" ");
   lcd.setCursor(5,1);
  lcd.print(data.gY);
  lcd.print(" ");
   lcd.setCursor(9,1);
  lcd.print(data.gZ); */ 
     //lcd.setCursor(0,0);
  //lcd.print(" P");
 // lcd.print(rateHandPositionValue(data.X, data.Y, data.Z));

 

     Serial.print(" HP : ");
          Serial.println(rateHandPositionValue(data.X, data.Y, data.Z));
     //lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
 
 
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  data.X = map(ax, -17000, 17000, 0, 255);  // X axis data
  data.Y = map(ay, -17000, 17000, 0, 255);
  data.Z = map(az, -17000, 17000, 0, 255);  // Y axis data

  data.gX = map(gx, -17000, 17000, 0, 255);  // X axis data
  data.gY = map(gy, -17000, 17000, 0, 255);
  data.gZ = map(gz, -17000, 17000, 0, 255);  // Y axis data

  //lcd.print("Hello");
   indexFingerValue = analogRead(A12);  //Read sensor voltage
   middleFingerValue = analogRead(A13);
   thumbFingerValue = analogRead(A11);

   ringFingerValue = analogRead(A14);
   littleFingerValue = analogRead(A15);


 

  printAllValues();

     
/*
 
   if (isA()==true) 
  {
    
   if( checkIfAlreadyPrintedTwice(text,'A')==true )
   { }

   else if( checkIfAlreadyPrintedOnce(text,'A')==true )
   { /*
     delay_timer(700);

    if (isA()==true) 
    {
    text.concat(rateCase("A"));
    lcd.setCursor(columnAt,rowAt);
    lcd.print(adjustText(text));
    }


   }

   else{
   text.concat(rateCase("A"));
    lcd.setCursor(columnAt,rowAt);
    lcd.print(adjustText(text));
   }


   }




    else if (isB()==true) {
        delay_timer(300);
   
   if( checkIfAlreadyPrintedTwice(text,'B')==true )
   { }
   
   
    else if( checkIfAlreadyPrintedOnce(text,'B')==true )
   { /*
     delay_timer(repeatDelay);

    if (isB()==true) 
    {
    text.concat(rateCase("B"));
    lcd.setCursor(columnAt,rowAt);
    lcd.print(adjustText(text));
    }
 
   }

   
   else{ 
   text.concat(rateCase("B"));
    lcd.setCursor(columnAt,rowAt);
    lcd.print(adjustText(text));
   
   }
   
   }


  
  else if (isC()==true) {

  delay_timer(200);
    
  if( checkIfAlreadyPrintedTwice(text,'C')==true )
   { }


    else if( checkIfAlreadyPrintedOnce(text,'C')==true )
   { 
     delay_timer(repeatDelay);

    if (isC()==true) 
    {
      text.concat(rateCase("C"));
      lcd.setCursor(columnAt,rowAt);
      lcd.print(adjustText(text));
    }

   }


    else{  if(isC()){      
    text.concat(rateCase("C"));
    lcd.setCursor(columnAt,rowAt);
    lcd.print(adjustText(text));
     }}
  }
 
 
   else if (isD()==true) {

  if( checkIfAlreadyPrintedTwice(text,'D')==true )
   { }

      else if( checkIfAlreadyPrintedOnce(text,'D')==true )
   { 
     delay_timer(repeatDelay);

    if (isD()==true) 
    {
      text.concat(rateCase("D"));
      lcd.setCursor(columnAt,rowAt);
      lcd.print(adjustText(text));
    }

   }

   else{
     text.concat(rateCase("D"));
    lcd.setCursor(columnAt,rowAt);
    lcd.print(adjustText(text));
   }
  }

 else if(isE()==true) {
             
  if( checkIfAlreadyPrintedTwice(text,'E')==true )
   { }


        else if( checkIfAlreadyPrintedOnce(text,'E')==true )
   { 
     delay_timer(repeatDelay);

    if (isE()==true) 
    {
      text.concat(rateCase("E"));
      lcd.setCursor(columnAt,rowAt);
      lcd.print(adjustText(text));
    }

   }


   else{
     text.concat(rateCase("E"));
    lcd.setCursor(columnAt,rowAt);
    lcd.print(adjustText(text));
   }
  }


  else if (isF()==true) {
             
  if( checkIfAlreadyPrintedTwice(text,'F')==true )
   { }

  else if( checkIfAlreadyPrintedOnce(text,'F')==true )
   { 
     delay_timer(repeatDelay);

    if (isF()==true) 
    {
      text.concat(rateCase("F"));
      lcd.setCursor(columnAt,rowAt);
      lcd.print(adjustText(text));
    }

   }

   else{
     text.concat(rateCase("F"));
    lcd.setCursor(columnAt,rowAt);
    lcd.print(adjustText(text));
   }
  }


 

     else if (isG()==true)
     {
        delay_timer(350);

      if(isG()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'G')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'G')==true )
          { 
            delay_timer(repeatDelay);

            if (isG()==true) 
            {
              text.concat(rateCase("G"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("G"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }
    
    else if  (isH()==true) 
      { 
        if( checkIfAlreadyPrintedOnce(text,'H')==true )
          { }
             
        else{
              text.concat(rateCase("H"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    
    

    else if  (isI()==true) 
      { 
        if( checkIfAlreadyPrintedOnce(text,'I')==true )
          { }
             
        else{
              text.concat(rateCase("I"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      
    }


    else if  (isJ()==true) 
      { 
        if( checkIfAlreadyPrintedOnce(text,'J')==true )
          { }
             
        else{
              text.concat(rateCase("J"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    

 


    else if  (isK()==true) 
      { delay_timer(300);
        if( checkIfAlreadyPrintedOnce(text,'K')==true )
          { }
             
        else{ delay_timer(repeatDelay);
              if(isK())
              {
                text.concat(rateCase("K"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
              }
            }
      }
    
 


     else if (isL()==true)
     {
        //delay_timer(350);

       
        if( checkIfAlreadyPrintedTwice(text,'L')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'L')==true )
          { 
            delay_timer(repeatDelay);

            if (isL()==true) 
            {
              text.concat(rateCase("L"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("L"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      
    }


     else if (isM()==true)
     {
         

      if(isM()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'M')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'M')==true )
          { 
            delay_timer(repeatDelay);

            if (isM()==true) 
            {
              text.concat(rateCase("M"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("M"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }



     else if (isN()==true)
     {
         

      if(isN()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'N')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'N')==true )
          { 
            delay_timer(repeatDelay);

            if (isN()==true) 
            {
              text.concat(rateCase("N"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("N"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }


     else if (isO()==true)
     {
         

      if(isO()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'O')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'O')==true )
          { 
            delay_timer(repeatDelay);

            if (isO()==true) 
            {
              text.concat(rateCase("O"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("O"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }



     else if (isP()==true)
     {
         

      if(isP()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'P')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'P')==true )
          { 
            delay_timer(repeatDelay);

            if (isP()==true) 
            {
              text.concat(rateCase("P"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("P"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }



     else if (isQ()==true)
     {
         

      if(isQ()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'Q')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'Q')==true )
          { 
            delay_timer(repeatDelay);

            if (isQ()==true) 
            {
              text.concat(rateCase("Q"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("Q"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }



     else if (isR()==true)
     {
         

      if(isR()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'R')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'R')==true )
          { 
            delay_timer(repeatDelay);

            if (isR()==true) 
            {
              text.concat(rateCase("R"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("R"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }


     else if (isS()==true)
     {
         

      if(isS()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'S')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'S')==true )
          { 
            delay_timer(repeatDelay);

            if (isS()==true) 
            {
              text.concat(rateCase("S"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("S"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }

      else if (isT()==true)
     {
         

      if(isT()==true)
      { 
        if( checkIfAlreadyPrintedTwice(text,'T')==true )
          { }

        else if( checkIfAlreadyPrintedOnce(text,'T')==true )
          { 
            delay_timer(repeatDelay);

            if (isT()==true) 
            {
              text.concat(rateCase("T"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

              
        else{
              text.concat(rateCase("T"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }
      }
    }   



else if (isU()==true) {
    
  if( checkIfAlreadyPrintedOnce(text,'U')==true )
   { }

             
   else{
     text.concat(rateCase("U"));
     lcd.setCursor(columnAt,rowAt);
     lcd.print(adjustText(text));
      }
    }


 else if (isV()==true) {
    
      if( checkIfAlreadyPrintedOnce(text,'V')==true )
       { }
          
   else{
     text.concat(rateCase("V"));
     lcd.setCursor(columnAt,rowAt);
     lcd.print(adjustText(text));
      }
    }


   else if (isW()==true) {
    
      if( checkIfAlreadyPrintedOnce(text,'W')==true )
       { }
          
   else{
     text.concat(rateCase("W"));
     lcd.setCursor(columnAt,rowAt);
     lcd.print(adjustText(text));
      }
    }  

  
  
  
  else if (isX()==true) {
    delay_timer(300);
      if( checkIfAlreadyPrintedOnce(text,'X')==true )
       { }
          
   else{ if(isX()){
     text.concat(rateCase("X"));
     lcd.setCursor(columnAt,rowAt);
     lcd.print(adjustText(text));
   }
   }
    }




  else if (isY()==true) {
      
      if( checkIfAlreadyPrintedOnce(text,'Y')==true )
       { }
          
   else{
     text.concat(rateCase("Y"));
     lcd.setCursor(columnAt,rowAt);
     lcd.print(adjustText(text));
      }
    }


  else if (isZ()==true) {
      
      if( checkIfAlreadyPrintedTwice(text,'Z')==true )
       { }

      else if( checkIfAlreadyPrintedOnce(text,'Z')==true )
          { 
            delay_timer(repeatDelay);

            if (isZ()==true) 
            {
              text.concat(rateCase("Z"));
              lcd.setCursor(columnAt,rowAt);
              lcd.print(adjustText(text));
            }

          }

          
   else{
     text.concat(rateCase("Z"));
     lcd.setCursor(columnAt,rowAt);
     lcd.print(adjustText(text));
      }
    }

    else if(digitalRead(4)==1 && digitalRead(2)==0 && digitalRead(3)==0)
    {backSpace();}

    else{ /*delayShortener=400*/; //}


     if(digitalRead(7)==HIGH){bluetoothSend();  }

     bluetoothReceive();

 

  
 
    if(text!=""){capsLock=false; }


    if(text.length()>=16){ rowAt=1; }
    else{rowAt=0;}


      if(text.length()>32){ text=text.substring(0, 31); }
   
 //checkLeftHand();


    //  mpu.initialize(); 
  delay_timer(600 ); // delay_timer(400-delayShortener);

}


 