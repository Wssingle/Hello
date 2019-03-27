
#include "mbed.h"
#include "LidarLitev2.h"

#define filter_const 10

LidarLitev2 Lidar(I2C_SDA, I2C_SCL); //can add true for fast data
Serial pc(USBTX,USBRX);
Serial device(PTC7, PTC6); //added // tx, rx
DigitalIn pb(SW3); //Push button sw3
DigitalOut gpo(D2); //LED flash

int my_filter(void); //     My_funcs
void j_table(int out); //   

Timer dt; //Instance of timer to do stats 
int main()
{   
    device.baud(115200); //add
    pc.baud(115200);
    Lidar.configure();
    dt.start();
    int out;
    char flag = 0;
    pc.printf("Press SW3 to DRILL\n");
    
    while(1){
         /////////Main loop
    
    
     if(pb != 1)
        flag = 1;
      
      
  
    if(flag == 1){     
         
        // printf("State Change\n\n");
         
         for(int i = 0; i < filter_const; i++)
           out += my_filter();
           
       out /= filter_const;
       //pc.printf("Lowpass Distance: %d\n", out);
       j_table(out); 
    
    }
    ////////////////Main loop end
    }
}

int my_filter(void){
    
    int x = Lidar.distance();
    //pc.printf("distance = %d cm frequency = %.2f Hz\n", Lidar.distance(), 1/dt.read());
    //pc.printf("distance = %d cm frequency = %.2f Hz\n", Lidar.distance(), 1/dt.read());
    dt.reset();
    
    return x; 
    
    }


unsigned char F = 0; 
int Far = 100;
int Mid = 70;
int Close = 30;

unsigned char Stopped = 0;
unsigned char Fast_CCW = 3;
unsigned char Slow_CCW = 1;
unsigned char Stop = 2;

unsigned char Fast_CW = 5;
unsigned char Slow_CW = 4;

unsigned char state = 0;

unsigned char lock = 0;

void j_table(int out){
    
    
    if(out > Far && F == Stopped){
        F = Slow_CCW;
        device.printf("%c",F);
         wait(1);
    }
    
    else if(out > Mid && out < Far && F == Slow_CCW && state == 0){
        F = Fast_CCW;
        device.printf("%c",F);
         //wait(2);
        state = 1;
    
        }
    
    else if(out > Close && out < Mid && F == Fast_CCW){
        F = Slow_CCW;
        device.printf("%c",F);
     wait(1);
        }
        
    else if( out < Close && F == Slow_CCW){
        F = Stop;
        device.printf("%c",F);
        F = Slow_CW;
        gpo = 1;
        wait(5);
        gpo = 0;
    
        }
    else if(out < Close && F == Slow_CW && lock == 0){
        device.printf("%c",F);
     wait(2);
     lock = 1;
        }  
        
    else if( out > Close && out < Mid && F == Slow_CW && state == 1 && lock == 1){
        F = Fast_CW;
        device.printf("%c",F);
        wait(2);
        state = 0;
        }
        
        else if(out > Mid && out < Far && F == Fast_CW){
            
            F = Slow_CW;
           device.printf("%c",F);
        wait(2); 
            }
            
    else if( out > Far && F == Slow_CW){
        F = Stop;
        device.printf("%c",F);
        wait(2);
       // F = 0;
       pc.printf("Done: Reset To Drill Again\n\n");
    
        }
    
    pc.printf("%d\n", out);
    //device.putc(out);
    
    }



/*
I2C i2c(I2C_SDA , I2C_SCL );

int main()
{
      
      printf("HI \n\n");

     //  const int addr7bit = 0x48;      // 7 bit I2C address
     //  const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90
      
      
      
       const int addr7bit = 0x62;      // 7 bit I2C address
       const int addr8bit = 0x62 << 1; // 8bit I2C address, 0x90
       
      char cmd[2];
    
    while (1) {
    
        cmd[0] = 0x01;
        cmd[1] = 0x00;
        i2c.write(addr8bit, cmd, 2);
 
        wait(0.5);
 
        cmd[0] = 0x00;
        i2c.write(addr8bit, cmd, 1);
        i2c.read( addr8bit, cmd, 2);
      
      printf("%c    %c", cmd[0], cmd[1]);
    }
    
}
*/