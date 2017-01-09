

#include "SIM900.h"
#include <SoftwareSerial.h>





//If you want to use the Arduino functions to manage SMS, uncomment the lines below.
#include "sms.h"
#include "gsmRTK.h"
#include <MCMCU.h>
#include <GSMINTERFACE.h>
#include <GSMMCU.h>



SMSGSM sms;
MCMCU mobileControlledMCU;
GSMI gsmInterface;
GSMMCU GSMMicroControllerUnit;

//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.

//Simple sketch to send and receive SMS.

int numdata;
boolean started=false;

char timeSendString[25];
void setup()
{
     if(1){
     char number[20] = "21541561351";
     //Serial connection.
     Serial.begin(115200);
     Serial.println("GSM Shield testing.");
     Serial.print("MCMCU:");
     mobileControlledMCU.begin();
     gsmInterface.begin();
     GSMMicroControllerUnit.begin(&gsm,&sms,&mobileControlledMCU,&gsmInterface);
     
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin(19200)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");
     gsmInterface.getUserNumber(number);
     Serial.println(number);
     gsmInterface.getUserNumber(number);
     Serial.println(number);
     Serial.print("status: Block:");
     Serial.print(mobileControlledMCU.getBlockingMode());
     Serial.print(" UnitID: ");
     Serial.println(mobileControlledMCU.getUnitId());
     }

    // strcpy(timeSendString,"16/01/06,15:40:10+01");
    // SetTimeStampString(timeSendString);
};

void loop()
{
  
     if(started) {

       GSMMicroControllerUnit.incommingHandler();
       GSMMicroControllerUnit.outgoingHandler();
       delay(1000);
     }
};




