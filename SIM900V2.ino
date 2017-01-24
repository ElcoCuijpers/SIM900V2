
//#define DUE_EEPROM

#include "SIM900.h"
//#include <SoftwareSerial.h>
#include "gps.h"




//If you want to use the Arduino functions to manage SMS, uncomment the lines below.
#include "sms.h"
#include "gsmRTK.h"
#include <MCMCU.h>
#include <GSMINTERFACE.h>
#include <GSMMCU.h>
#include <GPRSMCU.h>



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

int numdata;
boolean started=false;

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
     GPRSMicroControllerUnit.begin(&gsm,&mobileControlledMCU,&gsmInterface);
     if (gps.attachGPS()){
         Serial.println("status=GPSREADY");}
     else{ Serial.println("status=ERROR");}
     print_device_info();
     delay(5000);
};



void loop()
{
 char sendstring[200];
 Serial.println("test");
 delay(1000);
 if(started) {
       Serial.println("Joow");
       gsm.WhileSimpleRead();
       //GPRSMicroControllerUnit.GPRSSendDataToSink("blabla");
       gps.getLocationString(sendstring);
       GPRSMicroControllerUnit.GPRSSendDataToSink(sendstring);
       //gps.sendLocationGPRS(&GPRSMicroControllerUnit);
       //gps.sendLocationSMS(&GSMMicroControllerUnit, &gsmInterface);
       GSMMicroControllerUnit.incommingHandler();
       GSMMicroControllerUnit.outgoingHandler();
       GPRSMicroControllerUnit.GPRSSendHandler();
       delay(600000);
 
     }
};
 



