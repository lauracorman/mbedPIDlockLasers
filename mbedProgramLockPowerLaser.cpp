#include "mbed.h"
#include "PID.h"

#define RATE 1.0e-8

DigitalOut isOn(p20);
DigitalOut myled2(LED3);

DigitalIn isPIDon(p19);

//Kc, Ti, Td, interval
PID controller(0.05, 0.6e-8, 0.0, RATE);
//PID controller(0.25, 0.000001, 0.0001, RATE);
AnalogIn pv(p15);
AnalogIn setPt(p16);
AnalogOut signal(p18);
PwmOut signal2(p26);

int main() {
  //Analog input from 0.0 to 3.3V
  controller.setInputLimits(0.0, 1.0);
  //Pwm output from 0.0 to 1.0
  float outmax = 1.6/3.3;
  controller.setOutputLimits(0.0, outmax);
  //If there's a bias.
  //controller.setBias(0.3);
  controller.setMode(AUTO_MODE);
  //We want the process variable to be 1V at the begining for test
  controller.setSetPoint(1.0);
  myled2 = 1;
  signal2.period_us(500);
  float value = 0.7/3.3;
  wait(1);
  int counts1 = 0;
  int counts0 = 0;
  signal.write(value);
    while(1) {
        if(isPIDon){
            isOn = 1;
            myled2 = 1;
            controller.setSetPoint(setPt.read());
            //Update the process variable.
            controller.setProcessValue(pv.read());
            //Set the new output.
            value = controller.compute();
            signal.write(value);
            //Checks if there is a saturation and lights up the LED if it is the case
            if (value==outmax){
                counts1 = counts1 + 1;
                counts0 = 0;
            }
            else if (value==0.0){
                counts0 = counts0 + 1;
                counts1 = 0;
            }
            else {
                counts0 = 0;
                counts1 = 0;
            }
                
            if (counts0>100){
                signal2 = 1.0;
            }
            if (counts1>100){
                signal2 = 1.0;
            }
        }
        else{
            isOn = 0;
            signal2 = 0.0;
            myled2 = 0;
            signal.write(setPt.read());
        }
    }
}

