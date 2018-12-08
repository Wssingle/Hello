/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mbed.h"
#include "M2XStreamClient.h"
#include "EthernetInterface.h"

// Initialize Serial port
Serial pc(USBTX, USBRX);
Serial device(PTC17, PTC16);  // tx, rx

// Set Sensor Stream details
char deviceId[] = "6fd70edbd61b35d3d275a278bb7c33ca"; // Device you want to push to
char streamAcc[] = "acc_rms"; // Stream you want to push to
char streamMag[] = "mag_rms"; // Stream you want to push to
//char streamGyr[] = "gyr_rms"; // Stream you want to push to
char m2xKey[] = "0182418b121ceaf57cfccc689d511198"; // Your M2X API Key or Master API Key

int main()
{
    
printf("Begin Data Acquisition My Dude....\r\n\r\n");
// Intialize Ethernet connection
EthernetInterface eth;
eth.init();
eth.connect();
printf("Success. Connected!. Device IP Address is %s\r\n", eth.getIPAddress());

// Initialize the M2X client
Client client;
M2XStreamClient m2xClient(&client, m2xKey);
int ret;
 
    printf("Begin Data Acquisition My Dude2....\r\n\r\n");
    wait(0.5);

    printf("Slave Ready\r\n\r\n");
    //printf("\rSPL Over 70?\r\n\r\n");

    pc.baud(9600);                      //setting pc baud rate to 9600
    device.baud(9600);
    char ch;
    //double x = 10 ^3;
   // double y = 10 ^ 5;
   // double z =10^10;
      
     while(1){
      
   
        if(device.readable()) {
            ch = device.getc();
            //printf("%c",ch);
            }          
            
      if(ch == 'A'){
      printf("\n\nSPL Exceeded 90dB\n\r");
      ret = m2xClient.updateStreamValue(deviceId, streamAcc, 1);//accel_rms);
      printf("send() returned %d\r\n", ret);
      }
                
      if(ch == '8'){
      printf("\n\nDust Concentration Exceeded 0.05 mg/m^3\n\r");
      ret = m2xClient.updateStreamValue(deviceId, streamMag, 1);//accel_rms);
      printf("send() returned %d\r\n", ret);
     }
      
      ch = 0;
      //ret = m2xClient.updateStreamValue(deviceId, streamAcc, blue2);//accel_rms);
      //printf("send() returned %d\r\n", ret);
      //ret = m2xClient.updateStreamValue(deviceId, streamMag, 4); //mag_rms);
      //printf("send() returned %d\r\n", ret);
      //ret = m2xClient.updateStreamValue(deviceId, streamGyr, 5); //gyro_rms);
      //printf("send() returned %d\r\n", ret);
      //wait(1);
    }

}

          