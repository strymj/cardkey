#include <ros/ros.h>/*{{{*/
#include <iostream>
#include <unistd.h>
#include <sound_play/SoundRequest.h>
#include <std_msgs/String.h>
#include "kbhit.h"

#define LOOPRATE 10/*}}}*/

using namespace std;

bool flag_ring = false;/*{{{*/
string folder = "/home/meco/catkin_ws/src/cardkey/sounds/";
string file = "";
string kakutyoushi = ".wav";

string keyopen = "open.wav";
string keyclose  = "close.wav";
/*}}}*/

void ringCallback(const std_msgs::String &msg)/*{{{*/
{
	flag_ring = true;
	file = folder + msg.data + kakutyoushi;
}/*}}}*/

int main(int argc, char** argv)/*{{{*/
{
	ros::init(argc, argv, "qrkey_sound");
	ros::NodeHandle nh;

	ros::Publisher say_pub = nh.advertise<sound_play::SoundRequest>("/robotsound", 1);
	ros::Subscriber ring_sub = nh.subscribe("/file_name", 1, ringCallback);

	sound_play::SoundRequest say_msg;
	say_msg.sound = -2;	
	say_msg.command = 1;
	say_msg.volume = 1.0;
	say_msg.arg = "";
	say_msg.arg2 = "";

	ros::Rate loop_rate(LOOPRATE);
	while (ros::ok()) {
		if (flag_ring) {
			say_msg.arg = file;
			say_pub.publish(say_msg);
			flag_ring = false;
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}/*}}}*/
