
//#define DUE_EEPROM

#include "SIM900.h"
//#include <SoftwareSerial.h>





//If you want to use the Arduino functions to manage SMS, uncomment the lines below.
#include "sms.h"
#include "gsmRTK.h"
#include <MCMCU.h>
#include <GSMINTERFACE.h>
#include <GSMMCU.h>
#include <GPRSMCU.h>
#include <TimerThree.h>
#include "gps.h"
#define FALSE 0
#define TRUE 1

SMSGSM sms;
MCMCU mobileControlledMCU;
GSMI gsmInterface;
GSMMCU GSMMicroControllerUnit;
GPRSMCU GPRSMicroControllerUnit;

GPSGSM gps;
//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.

//Simple sketch to send and receive SMS.
volatile bool semaphoreUART1 = FALSE;
volatile int queueUART1 = 0;
int numdata;
boolean started=false;

char sendstring[200];
char sendstringcheckBuffer[200];
//sendstring[0]='\0';
//sendstringcheckBuffer[0]='\0';
int tempQueueUART1;


void print_device_info(){
     char number[20] = "21541561351";
     gsmInterface.getAdminNumber(number);
     Serial.println(number);
     gsmInterface.getUserNumber(number);
     Serial.println(number);
     Serial.print("status: Block:");
     Serial.print(mobileControlledMCU.getBlockingMode());
     Serial.print(" UnitID: ");
     Serial.println(mobileControlledMCU.getUnitId());  
}



volatile unsigned long masterTimer=0;
void masterClock(void){
  interrupts();
 // Serial.print("i");
  switch (masterTimer){
    case 400://GPS location 
      queueUART1 += 1;
    break;  
    case 800:// Send GPRS
      queueUART1 += 2;
    break;
    case 1000://Incomming SMS
      queueUART1 += 4;
    break;/*
    case 800: // Incomming SMS
      queueUART1 += 8;
    break;*/        
    break;  
  }
  if(masterTimer>1100){masterTimer=0;}
 // if(masterTimer%10){Serial.println("i");}
  masterTimer++;
}




void setup()
{
     //Serial connection.
     Serial.begin(115200);
     mobileControlledMCU.begin();
     gsmInterface.begin();
     GSMMicroControllerUnit.begin(&gsm,&sms,&mobileControlledMCU,&gsmInterface);
     //Start configuration of shield with baudrate.
     if (gsm.begin(19200)) {
          Serial.println("\nstatus=GSMREADY");
          started=true;
     } else Serial.println("\nstatus=IDLE");  
     if (gps.attachGPS()){
         Serial.println("status=GPSREADY");}
     else{ Serial.println("status=ERROR");}
     print_device_info();
     delay(5000);

     Timer3.initialize(150000);//Timer in us
     Timer3.attachInterrupt(masterClock); // blinkLED to run every 0.15 seconds
};



void loop()
{

//gps.sendLocationSMS(&GSMMicroControllerUnit, &gsmInterface);
 //Serial.println("test");
 //delay(1000);


 if(started) {
    // Serial.println("Joow");
     //gsm.WhileSimpleRead();
       
       for(int i=0;i<8;i++){
         tempQueueUART1 = queueUART1;
         tempQueueUART1 = (tempQueueUART1>>i) & (0x01);
         tempQueueUART1 += i*10;
         switch (tempQueueUART1){
           case 01:
             Serial.println("case01");
             if(!gps.getLocationString(sendstringcheckBuffer)){
                 strcpy(sendstring,sendstringcheckBuffer);
                  Serial.println("NEW COORDINATE");
             }
             
            // gps.getLocationString(sendstring);
             Serial.println(sendstring);
             queueUART1 = queueUART1 & 0xFE;
             break;  
         case 11:
             queueUART1 =queueUART1 & 0xFD;
             GSMMicroControllerUnit.incommingHandler(sendstring);
             
             Serial.println("case11");
             break;          
         case 21:
             queueUART1 =queueUART1 & 0xFB;
             GSMMicroControllerUnit.outgoingHandler();
             Serial.println("case21");
             break;                       
         default:
           break;  
        }
      }
      //delay(100);
      //Serial.println(queueUART1);
 }
};
 

    //}
    
        
        
     
     //
      //  GSMMicroControllerUnit.outgoingHandler();
      //  GPRSMicroControllerUnit.GPRSSendHandler();        
      //  Serial.print("SENDSTRING:"); Serial.println(sendstring);
      //  gps.getLocationString(sendstring);
      //  GPRSMicroControllerUnit.GPRSSendDataToSink(sendstring);
        
       //delay(6000);

