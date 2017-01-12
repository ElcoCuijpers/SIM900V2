

#include "SIM900.h"
#include <SoftwareSerial.h>
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
 char timeString[300];
  char timeString2[300];
 char sendingString[300];
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
      GPRSMicroControllerUnit.begin(&gsm,&mobileControlledMCU,&gsmInterface);
      delay(5000);
     if (gps.attachGPS()){
         Serial.println("status=GPSREADY");}
      else{ Serial.println("status=ERROR");}
      delay(5000);
   // gsm.SimpleWriteln("AT+CIFSR");
    //GPRSMicroControllerUnit.GPRSGetTime();
   // strcpy(timeSendString,"17/01/10,11:45:10+01");
   // SetTimeStampString(timeSendString);
  
  
};
char sendstring[200];
int stat;
char lon[15];
char lat[15];
char alt[15];
char timee[20];
char vel[15];


void loop()
{
  
 if(started) {
      gsm.WhileSimpleRead();
      //strcpy(sendingString,"time=");
     // GetTimeStampString(timeString);
      //unsafe_URL_conversion(timeString,timeString2);
      
      //strcat(sendingString,timeString2);
      //strcat(sendingString, "");
      //strcpy(sendingString,"p1=0&p2=0&p10=0&p11=0&p13=0");
      
     // GPRSMicroControllerUnit.GPRSGetSetupParameter("time=12");
      strcpy(sendstring,"stat=");
          stat=gps.getStat();
          if(stat==1){
               strcat(sendstring,"NOT FIXED");
               Serial.println("NOT FIXED");}
          else if(stat==0){
               strcat(sendstring,"GPS OFF");
               Serial.println("GPS OFF");}
          else if(stat==2){
               strcat(sendstring,"2D FIXED");
               Serial.println("2D FIXED");}
          else if(stat==3){
               strcat(sendstring,"3D FIXED");
               Serial.println("3D FIXED");}
          delay(5000);
          //Get data from GPS
          gps.getParRMC(lon,lat,alt,timee,vel);
        /*  Serial.println(lon);
          Serial.println(lat);
          Serial.println(alt);
          Serial.println(time);
          Serial.println(vel);*/
          
      strcat(sendstring,"&lon=");
      strcat(sendstring,lon);
      strcat(sendstring,"&lat=");
      strcat(sendstring,lat);
      strcat(sendstring,"&alt=");
      strcat(sendstring,alt);
      strcat(sendstring,"&time=");
      strcat(sendstring,timee);
      strcat(sendstring,"&vel=");
      strcat(sendstring,vel);
      
      GPRSMicroControllerUnit.GPRSSendDataToSink(sendstring);
      Serial.print("string:");
      Serial.println(sendstring);
       
       //GSMMicroControllerUnit.incommingHandler();
       //GSMMicroControllerUnit.outgoingHandler();
       GPRSMicroControllerUnit.GPRSSendHandler();
       delay(20000);
 
     }
};




