#include <ros/ros.h>/*{{{*/
#include <iostream>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_datatypes.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
#include <sound_play/SoundRequest.h>
#include <time.h>/*}}}*/

#define LOOPRATE 10      // Hz
#define W_TIME 5         // sec
#define PWM_CENTER 1500  // us
#define PWM_90_DEG 950   // us

using namespace std;

bool object_exist = false;
float object_pose = 0.0;

void objectCallback(const std_msgs::Float32MultiArray &msg)
{
	if (msg.data.size()) {
		object_exist = true;
		object_pose = msg.data[3];
	}
}

class Cardkey
{
	private:
		void status_disp();
		void sayword_male();
		void sayword_female();
		void openclose_sound();
		void key_operate();
		void publish_servoPWM(int);
		void publish_sound(string);
		time_t timer;
		bool malevoice_flag;
		tm *now;
		string folder;

		ros::NodeHandle nh;
		ros::Subscriber object_sub;
		std_msgs::Int32 servoPWM;
		ros::Publisher servo_pub;
		sound_play::SoundRequest sound_msg;
		ros::Publisher sound_pub;

	public:
		void spin();
		Cardkey()
		{
			time(&timer);
			nh.param("/cardkey/male_voice", malevoice_flag, true);
			nh.param("/cardkey/soundpath", folder, string(""));
			object_sub = nh.subscribe("/objects",1,objectCallback);
			servo_pub = nh.advertise<std_msgs::Int32>("/servoPWM",1);
			sound_pub = nh.advertise<sound_play::SoundRequest>("/robotsound", 1);
			sound_msg.sound = -2;	
			sound_msg.command = 1;
			sound_msg.volume = 1.0;
			sound_msg.arg = "";
			sound_msg.arg2 = "";
		}
};

void Cardkey::publish_sound(string filename)
{
	sound_msg.arg = folder + filename;
	sound_pub.publish(sound_msg);
}

void Cardkey::publish_servoPWM(int value)
{
	servoPWM.data = value;
	servo_pub.publish(servoPWM);
}

void Cardkey::status_disp()
{
	static int count = 0;
	static int send_interval = 5;
	if (timer != time(NULL)) {
		time(&timer);
		now = localtime(&timer);
		cout<<endl;
		cout<<"========================================"<<endl<<endl;
		cout<<"       "<<ctime(&timer)<<endl;
		cout<<"          加藤研へようこそ！"<<endl<<endl;
		cout<<"         waiting ID card ..."<<endl<<endl;
		cout<<"========================================"<<endl;

		count = (count+1)%send_interval;
		if(count == 0) {
			publish_servoPWM(PWM_CENTER);
		}
	}
}

void Cardkey::sayword_male()
{
	if (object_pose > 0){
		publish_sound("irassyai.wav");
	}
	else if (object_pose < 0) {
		publish_sound("haisainara.wav");
	}
	sleep(2);
}

void Cardkey::sayword_female()
{
	if (object_pose > 0){
		if (4 <=  now->tm_hour && now->tm_hour < 11) {
			publish_sound("ohayougozaimasu.wav");
		}
		else if (11 <=  now->tm_hour && now->tm_hour < 18) {
			publish_sound("konnichiha.wav");
		}
		else if (18 <=  now->tm_hour || now->tm_hour < 4) {
			publish_sound("konbanha.wav");
		}
	}
	else if (object_pose < 0) {
		publish_sound("otsukaresamadeshita.wav");
	}
	sleep(2);
}

void Cardkey::openclose_sound()
{
	if (object_pose > 0) {
		publish_sound("open.wav");
	}
	else if (object_pose < 0) {
		publish_sound("close.wav");
	}
}

void Cardkey::key_operate()
{
	if (0 <= object_pose){
		cout<<endl;
		cout<<"========================================"<<endl<<endl<<endl;
		cout<<"           ############"<<endl;
		cout<<"           # key open #"<<endl;
		cout<<"           ############"<<endl<<endl<<endl;
		cout<<"========================================"<<endl;

		publish_servoPWM(PWM_CENTER - PWM_90_DEG);

	}
	else if (object_pose < 0){
		cout<<endl;
		cout<<"========================================"<<endl<<endl<<endl;
		cout<<"           #############"<<endl;
		cout<<"           # key close #"<<endl;
		cout<<"           #############"<<endl<<endl<<endl;
		cout<<"========================================"<<endl;

		publish_servoPWM(PWM_CENTER + PWM_90_DEG);
	}

	sleep(1);
	publish_servoPWM(PWM_CENTER);
	sleep(3);
	ros::spinOnce();
}

void Cardkey::spin()
{
	ros::Rate loop_rate(LOOPRATE);
	while (ros::ok())
	{
		object_exist = false;
		ros::spinOnce();
		status_disp();
		if (object_exist) {
			if(malevoice_flag) {
				sayword_male();
			} else {
				sayword_female();
			}
			openclose_sound();
			key_operate();
		}
		loop_rate.sleep();

	}
}

int main (int argc, char** argv)
{
	ros::init(argc, argv, "qrkey");
	Cardkey cardkey;
	cardkey.spin();
	return 0;
}
