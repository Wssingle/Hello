#include "mbed.h"
#include "TextLCD.h"
#include "math.h"

///////////////////////////////////////////////////////////////////LCD Garbage
//* TextLCD lcd(p10, p12, p15, p16, p29, p30); // rs, e, d4-d7
//TextLCD lcd(PTC17,PTB9,PTB23,PTA2,PTC2,PTC3,TextLCD::LCD16x2);
//TextLCD lcd(D1,D2,D6,D5,D4,D3,TextLCD::LCD16x2);
//TextLCD lcd(p1,p2,p6,p5,p4,p3,TextLCD::LCD16x2);
//gitalOut gpo(D0); //Voltage
//DigitalOut led(LED_RED);
//////////////////////////////////////////////////////////////////////////////////////

//LCD
TextLCD lcd(PTC4,PTB9,PTC2,PTA2,PTB23,PTA1,TextLCD::LCD16x2);  //Interfaces with LCD class
DigitalOut gpo(D15); //Blue 
DigitalOut gpo2(D7); //Green 
DigitalOut gpo3(D8); //Red
//LCD END

//Dust Sensor
DigitalOut gpoA(D14);
AnalogIn Ain(A5);
//Dust Sensor End

//SPL Sensor
DigitalOut gpoS(D13);
AnalogIn AinS(A4);
//SPL Sensor End

//Bluetooth
Serial pc(USBTX, USBRX );
Serial blue(PTC15, PTC14); //tx, rx
//pc.baud(9600);//Bluetooth
//blue.baud(9600);//Bluetooth
//blue.putc(c); to send over bluetooth  
//Bluetooth End

//Serial Output to FRDM kw41z START
Serial device(PTC17, PTC16);  // tx, rx
//Serial Output to FRDM kw41z END

int main(){
 
 //Main Program Setup/////////////////////////////////////////////
 pc.baud(9600);//Bluetooth
 blue.baud(9600);//Bluetooth
 
 device.baud(9600);//Serial Output to FRDM kw41z
       
 wait(5);//Dust sensor  
 float Dust_Ain;//Dust sensor 
 float Dust_Level;//Dust sensor   

//SPL START 
 int avg_mod = 1;
 double dB; //Decibels measured
 double SPL; //Sound Pressure Level
 //const double BaseV = 1.65; //Constant REAL output offset
 const double BaseVnorm = 0.42;//0.5;//0.5542992807309; //Constant Normalized output offset 
 const double MicSenRMS = 0.00794343282; //Mic RMS Voltage based on sensativity
 const double PreGain = 67; //Pre-Amp Gain  
 const int MicSen = -42; //Mic sensativity in dB
 const int Voodoo = 94; //Sound guy voodoo number
 double sum = 0;
 double avg;
 double v; //voltage
 gpoS = !gpoS; //Vcc for sensor
 gpo2 = !gpo2;
 //double max = 0;
//SPL END

//LCD Vars START
int Dust_State = 0;
int SPL_State = 0;
int WaitVar;
int WaitVar2;
int WaitVar3 = -1;
int Jmp = 0;
int Jmp2 = 0;
//LCD Vars END

//Main Program Setup END/////////////////////////////////////////


//Main Program Start///////////////////////////////////////////// 
do{ 

//SPL Sensor///////////////////////////////////////////////////// 
    for(int i = 0; i < avg_mod; i++){

    v = AinS.read(); // Reads analog input from sensor
  
    //if(v > max)
    //    max = v;
        
    //printf("Analog In From Sensor: %lf\r\n\n\n",v);
    //printf("%lf\r\n\n\n",v);
    sum+= v;
    }
  
   avg = sum / avg_mod;
   sum = 0;
   
   // printf("average of 10 : %lf \r\n\n\n", avg); 
   v = avg;
   //v = max;
   //max = 0;
  
   //printf("Normalized to 3.3v: %f\r\n", v); //Prints the detected voltage divided by 3.3V
   //printf("Actual Input Voltage: %lf\r\n\n\n", v * 3.3);//Prints actual Voltage   
 
   //printf("Delta V: %lf \r\n\n\n", (((v - BaseVnorm))* 3.3)); //Displays Change in Voltage 
 
   dB = 20.0 * log( (((v - BaseVnorm))* 3.3) / (MicSenRMS)); //Decibel Equation
   SPL = MicSen + dB + Voodoo - PreGain; //Sound Pressure Level Equation 
   //printf("Sound Pressure Level: %lf\r\n",SPL);
  
  //printf("dB:  %lf\r\n", dB); //Prints Decibels 
  printf("%lf\r\n", SPL); //Prints Sound Pressure Level  
//SPL Sensor END////////////////////////////////////////////////// 
   
//Dust sensor/////////////////////////////////////////////////////     
    gpoA = !gpoA;     //Starts IR pulse
    wait_us(0.28*1000); //Waits reccomnded 0.28ms 
    Dust_Ain = Ain.read(); // Reads analog input from sensor
    Dust_Level = ((Dust_Ain * 5.0f) - 0.8f)/(5.1f);
    //printf("Normalized to 3.3v: %f\r\n", Dust_Ain); //Prints the detected voltage divided by 3.3V
    //printf("Actual Input Voltage: %f\r\n", Dust_Ain * 5.0f); //Prints actual Voltage
    //printf("%f\r\n", Dust_Ain * 5.0f);
    printf("%f\r\n", Dust_Level);
    
    //printf("Dust Concentration mg/m^3: %f\r\n\n", Dust_Level);
    wait_us(0.04*1000);  //Waits for some reason
    gpoA = !gpoA; //Turns off IR pulse 
    wait_us(9.68*1000); //Waits remainder of recommended period, can pulse again after this period (maybe) 
//Dust sensor END//////////////////////////////////////////////////      
    
//LCD Control, and Bluetooth Send////////////////////////////////// 
    
    if(SPL > 80){ //Set to 80 for demo purposes
    SPL_State = 1; //High
    blue.putc('1'); //added for bluetooth
    device.putc('Z'); //Serial Output to FRDM kw41z
    }
    else{ 
    SPL_State = 0;
    }
    
    if(Dust_Level > 0.1f){  //Should be 0.05 but set to 0.1 for demo purposes
    Dust_State = 1; //High
    blue.putc('D'); //added for bluetooth
    device.putc('X'); //Serial Output to FRDM kw41z
    }
    else{ 
    Dust_State = 0;
    }
    
    if(Jmp == 0){
   if(SPL_State == 1){
   Jmp = 1;    
   gpo2 = 0; //Turns off Green
   gpo3 = 1; //Turns on Red
   WaitVar = 10^8;
   WaitVar2 = 10^5;
   if(Jmp2 == 0){
   lcd.cls();
   wait(0.001);
   }
   lcd.printf("Above 90dB! :(");
   //lcd.printf("\nToo Dusty! :(");
   //lcd.locate(0,1);
   //lcd.printf("it works!!!");

   //blue.putc('1'); //added for bluetooth
   //wait(5);  
   //wait_us(5*1000);  
   //lcd.cls();
   }
   }
   if(Jmp2 == 0){
   if(Dust_State == 1){
    Jmp2 = 1;
    WaitVar = 10^8;   
     WaitVar2 = 10^5;
   gpo2 = 0; //Turns off Green
   gpo3 = 1; //Turns on Red
   if(Jmp == 0)
   lcd.cls();
   wait(0.001);
   lcd.printf("\nToo Dusty :(!");
   if(Jmp == 0){
   lcd.printf("\n");
   }
       }
       }
       
   if(WaitVar < 0){
       WaitVar2--;    
       if(WaitVar2 < 0){
           WaitVar2 = -1;
           }
   if(WaitVar2 < 0){
   if(SPL_State == 0 && Dust_State == 0){
   Jmp = 0;
   Jmp2 = 0;    
   gpo2 = 1; //Turns on Green
   gpo3 = 0; //Turns off Red
   lcd.cls();
   wait(0.001);
   lcd.printf("Safe! :)");
   }
   }
   }
   
   WaitVar--;
   if(WaitVar < 0){
   WaitVar = -1;
   }
 //  if(SPL < 70){

  //     lcd.cls();
  //     wait(0.001);
   //    lcd.printf("Safe! :)\n");
   //    gpo2 = 1; //Turns on Green 
   //    gpo3 = 0; //Turns off Red
     //blue.putc('0'); //added for bluetooth  
  //  } 
//LCD Control, and Bluetooth Send END//////////////////////////////////   

if(WaitVar3 < 0){
if(blue.readable()){ //Joke: It's Island TIME!!!
if(blue.getc() == 'I'){
    gpo = !gpo;
    WaitVar3 = 214;
    }
    }
}
if(WaitVar3 > -1){
WaitVar3--;
}
if(WaitVar3 < 0){
WaitVar3 = -1;
}

////////////////////End Island Time

    }while(1);
//Main Program End/////////////////////////////////////////////////////
}


//////////////////////////////////////////////////////////Additional Demos 
//Dust Sensor Demo /////////////////
/*
 wait(5);   
 float x;
  
 do{
     
    gpoA = !gpoA;     //Starts IR pulse
    wait_us(0.28*1000); //Waits reccomnded 0.28ms 
    x = Ain.read(); // Reads analog input from sensor
    //printf("Normalized to 3.3v: %f\r\n", x); //Prints the detected voltage divided by 3.3V
    //printf("Actual Input Voltage: %f\r\n", x * 5.0f); //Prints actual Voltage
    printf("%f\r\n", x * 5.0f);
    printf("mg/m^3 %f\r\n\n", ((x * 5.0f) - 0.8)/(5.1));
    wait_us(0.04*1000);  //Waits for some reason
    gpoA = !gpoA; //Turns off IR pulse 
    wait_us(9.68*1000); //Waits remainder of recommended period, can pulse again after this period (maybe) 
    
    //lcd.cls();
    //wait(0.001);
    //lcd.printf("hello world it  is %.3f!!",x*3.3f); 
    //gpo2 = !gpo2;
   // wait(1); //Waits because why would this be polling all the time    
     
     }while(1);

//END Dust Sensor Demo /////////////////
*/

//ADC Demo /////////////////////////
/*  
    gpoA = !gpoA;
    do{
      
    printf("Normalized to 3.3v: %f\t", Ain.read());
    printf("\nActual Input Voltage: %f\t", Ain.read()*3.3f);     
    
    wait(0.3);    
        
    }while(1);
*/
//End ADC Demo //////////////////////


//LCD Demo /////////////////////////////
/* 
    lcd.cls();
    wait(0.001);
    lcd.printf("hello world");
    lcd.locate(0,1);
    lcd.printf("it works!!!");
    
    while(1){
    
    lcd.cls();
    wait(0.001);
    gpo = !gpo;
    lcd.printf("Will is Cool in Blue");
    wait(4);
    lcd.cls();
    wait(0.001);
    gpo = gpo;
    wait(0.001);
    
    gpo2= !gpo2;
    lcd.printf("Will is Cool in Greeen");
    wait(4);
    lcd.cls();
    wait(0.001);
    gpo2= gpo2;
    wait(0.001);
    
    gpo3 = !gpo3;
    lcd.printf("Will is Cool in Rojo");
    wait(4);
    lcd.cls();
    wait(0.001);
    gpo3= gpo3;
    wait(0.001);
    
    }
  */ 
//End of LCD demo ////////////////////////////////

