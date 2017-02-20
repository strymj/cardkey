#include <ros/ros.h>/*{{{*/
#include <iostream>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_datatypes.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32MultiArray.h>
#include <time.h>/*}}}*/

#define PI 3.14159     // rad/*{{{*/
#define LOOPRATE 10    // Hz
#define W_TIME 5       // sec
#define PWM_CENTER 1500
#define PWM_90 950
#define TILT_RANGE 30  // deg
const double RAD2DEG = 180.0 / PI;/*}}}*/

using namespace std;/*{{{*/

double roll=0.0, pitch = 0.0, yaw = 0.0;
string inputcode = "nodata";
int object_num = 0;
float object_pose = 0.0;
double deg_z;/*}}}*/

void objectCallback(const std_msgs::Float32MultiArray &msg)/*{{{*/
{
	if (msg.data.size()) {
		object_num = (int)msg.data[0];
		object_pose = msg.data[3];
	}
}/*}}}*/

class Qrk/*{{{*/
{
	private:
		void init();
		void status_disp();
		void operate();
		void say_word_m();
		void say_word_f();
		void say_oc();
		void key_operate();
		time_t timer;
		bool mf;
		tm *now;
	public:
		void spin();
		ros::NodeHandle nh;
		ros::Subscriber object_sub;
		std_msgs::Int32 servoPWM;
		ros::Publisher servo_pub;
		std_msgs::String ring_msg;
		ros::Publisher ring_pub;
		Qrk()
		{
			time(&timer);
			nh.param("male_voice", mf, true);
			object_sub = nh.subscribe("/objects",1,objectCallback);
			servo_pub = nh.advertise<std_msgs::Int32>("/servoPWM",1);
			ring_pub = nh.advertise<std_msgs::String>("/file_name",1);
		}
};/*}}}*/

void Qrk::init()/*{{{*/
{
	object_num = 0;
	object_pose = 0.0;
}/*}}}*/

void Qrk::status_disp()/*{{{*/
{
	if (timer != time(NULL)) {
		time(&timer);
		now = localtime(&timer);
		cout<<endl;
		cout<<"========================================"<<endl<<endl;
		cout<<"       "<<ctime(&timer)<<endl;
		cout<<"          加藤研へようこそ！"<<endl<<endl;
		cout<<"         waiting ID card ..."<<endl<<endl;
		cout<<"========================================"<<endl;

		servoPWM.data = PWM_CENTER;
		servo_pub.publish(servoPWM);
	}
}/*}}}*/

void Qrk::say_word_m()/*{{{*/
{
	if (object_pose > 0){
		ring_msg.data = "irassyai";
	}
	else if (object_pose < 0) {
		ring_msg.data = "haisainara";
	}

	ring_pub.publish(ring_msg);
	sleep(2);
}/*}}}*/

void Qrk::say_word_f()/*{{{*/
{
	if (object_pose > 0){
		if (4 <=  now->tm_hour && now->tm_hour < 11) {
			ring_msg.data = "ohayougozaimasu";
		}
		else if (11 <=  now->tm_hour && now->tm_hour < 18) {
			ring_msg.data = "konnichiha";
		}
		else if (18 <=  now->tm_hour || now->tm_hour < 4) {
			ring_msg.data = "konbanha";
		}
	}
	else if (object_pose < 0) {
		ring_msg.data = "otsukaresamadeshita";
	}

	ring_pub.publish(ring_msg);
	sleep(2);
}/*}}}*/

void Qrk::say_oc()
{
	if (object_pose > 0) {
		ring_msg.data = "open";
	}
	else if (object_pose < 0) {
		ring_msg.data = "close";
	}
	ring_pub.publish(ring_msg);
}

void Qrk::key_operate()/*{{{*/
{
	if (object_pose > 0){
		cout<<endl;
		cout<<"========================================"<<endl<<endl<<endl;
		cout<<"           ############"<<endl;
		cout<<"           # key open #"<<endl;
		cout<<"           ############"<<endl<<endl<<endl;
		cout<<"========================================"<<endl;

		servoPWM.data = PWM_CENTER - PWM_90;

	}
	else if (object_pose < 0){
		cout<<endl;
		cout<<"========================================"<<endl<<endl<<endl;
		cout<<"           #############"<<endl;
		cout<<"           # key close #"<<endl;
		cout<<"           #############"<<endl<<endl<<endl;
		cout<<"========================================"<<endl;

		servoPWM.data = PWM_CENTER + PWM_90;
	}

	servo_pub.publish(servoPWM);
	sleep(1);
	servoPWM.data = PWM_CENTER;
	servo_pub.publish(servoPWM);
	sleep(3);
	ros::spinOnce();
}/*}}}*/

void Qrk::operate()/*{{{*/
{
	if (object_num) {
		if(mf) {
			say_word_m();
		} else {
			say_word_f();
		}
		say_oc();
		key_operate();
	}
}/*}}}*/

void Qrk::spin()/*{{{*/
{
	ros::Rate loop_rate(LOOPRATE);
	while (ros::ok())
	{
		init();
		ros::spinOnce();
		status_disp();
		operate();
		loop_rate.sleep();

	}
}/*}}}*/

int main (int argc, char** argv)/*{{{*/
{
	ros::init(argc, argv, "qrkey");
	Qrk qrk;
	qrk.spin();
	return 0;
}/*}}}*/
