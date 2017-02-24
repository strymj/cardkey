#include "mbed.h"
#include <ros.h>
#include <std_msgs/Int32.h>

#define PWM_PERIOD 0.02
#define CENTER_PWM 1500
#define SUBLED_N 2
#define LOOPRATE 10          // Hz

Timer runtime;
Timer looptime;

ros::NodeHandle nh;

PwmOut cam_servo(p26);
DigitalOut loopled(LED1);
DigitalOut subled(LED4);

int t_sum = 0;
bool subscribed = false;

void servoCallback(const std_msgs::Int32 &PWM_msg)
{
	cam_servo.pulsewidth_us(PWM_msg.data);
	subscribed = true;
}

ros::Subscriber<std_msgs::Int32> servo_sub("/servoPWM", servoCallback);

void subled_flash()
{
	static int count = 0;
	if(subscribed || count) {
		subled = !subled;
		count = (count+1) % (SUBLED_N*2);
	}
	else {
		subled = 0;
	}
}

void looprate_sleep_ms(int looprate)
{
	int waittime_ms = 1000.0/looprate - looptime.read_ms();
	if(waittime_ms > 0) {
		wait_ms(waittime_ms);
	}
	looptime.reset();

	static int count = 0;
	if(count==0) {
		loopled = !loopled;
	}
	count = (count+1) % looprate;
}

int main()
{
	nh.initNode();
	nh.subscribe(servo_sub);

	cam_servo.period(PWM_PERIOD);
	cam_servo.pulsewidth_us(CENTER_PWM);

	runtime.start();
	looptime.start();

	while (1) {
		subscribed = false;
		nh.spinOnce();
		subled_flash();
		looprate_sleep_ms(LOOPRATE);
	}
}
