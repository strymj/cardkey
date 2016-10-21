// directry : rosserial_mbed/example

#define COMPILE_SERVOCONTROL_CODE_ROSSERIAL
#ifdef  COMPILE_SERVOCONTROL_CODE_ROSSERIAL

/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 *
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library
 * Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */

#include "mbed.h"
#include "Servo.h"
#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>

#define PWM_PERIOD 0.02
#define CENTER_PWM 1500
#define RESET_INTERVAL 180   // [s]
#define RESET_LED_FLASH 3
#define FLASH_INTERVAL 100   // [ms]
#define LOOP_INTERVAL 500    // [ms]

Timer runtime;
Timer LEDtime;

ros::NodeHandle nh;

PwmOut cam_servo(p26);
DigitalOut loop_led(LED1);
DigitalOut subscribe_led(LED2);
DigitalOut reset_led(LED3);

int t_sum = 0;
bool subscribed = false;

void servoCallback(const std_msgs::Int32 &PWM_msg)
{
    cam_servo.pulsewidth_us(PWM_msg.data);
    subscribed = true;
}

ros::Subscriber<std_msgs::Int32> servo_sub("/servoPWM", servoCallback);

void reset_mbed()
{
        if (runtime.read() > RESET_INTERVAL) {
            for (int i=0; i<RESET_LED_FLASH; i++) {
                reset_led = !reset_led;
                wait_ms(FLASH_INTERVAL);
                reset_led = !reset_led;
                wait_ms(FLASH_INTERVAL);
            }
            NVIC_SystemReset();
            runtime.reset();
        }
}

void LED1_flash(int time)
{
    static int STEP = LOOP_INTERVAL/2;
         if (time <= STEP * 1) loop_led = 1;
    else if (time <= STEP * 2) loop_led = 0;
    
}

void LED2_flash(int time)
{
    static int STEP = LOOP_INTERVAL/8;
    if (subscribed) {
             if (time <= STEP * 1) subscribe_led = 1;
        else if (time <= STEP * 2) subscribe_led = 0;
        else if (time <= STEP * 3) subscribe_led = 1;
        else if (time <= STEP * 4) subscribe_led = 0;
    }
}

void LED_wait()
{
    static int step = 10;   //ms
    for (int i=0; i<LOOP_INTERVAL; i+=step)
    {
        LED1_flash(i);
        LED2_flash(i);
        wait_ms(step);
    }

}

void init()
{
    subscribed = false;
}

int main() {

    nh.initNode();
    nh.subscribe(servo_sub);
    
    cam_servo.period(PWM_PERIOD);   //us
    cam_servo.pulsewidth_us(CENTER_PWM);

    runtime.start();
    LEDtime.start();

    while (1) {
        init();
        nh.spinOnce();
        reset_mbed();
        LED_wait();
    }
}
#endif
