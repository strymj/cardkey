#include <ros/ros.h>
#include <iostream>
#include <std_msgs/String.h>
#include <fstream>

#define LOOPRATE 10    // Hz

using namespace std;

char MAIL_FILE[] = "mail.txt";

bool enter = false;
bool finish = false; 

void keystatusCallback(const std_msgs::String &msg)
{
	if (msg.data == "open")
	{
		cout<<"<Enter>"<<endl;
		finish = true;
		enter = true;
	}
	else if (msg.data == "close")
	{
		cout<<"<Leave>"<<endl;
		finish = true;
		enter == false;
	}
}

void write_mail()
{
	time_t timer;
	time(&timer);

	remove(MAIL_FILE);
	ofstream ofs(MAIL_FILE);
	if (!ofs) {
		cerr<<"Can't open file"<<endl;
		exit(1);
	}

	ofs<<"To: yamaji.shoutarou@gmail.com"<<endl;
	ofs<<"From: suteregistermail@gmail.com"<<endl;
	ofs<<"Subject: MeCoLab"<<endl<<endl;
	
	ofs<<ctime(&timer);
	if (enter) {
		ofs<<"<Enter>";
	}
	else {
		ofs<<"<Leave>";
	}
}

int main (int argc, char ** argv)
{
	ros::init(argc, argv, "cout");
	ros::NodeHandle nh;
	ros::Subscriber keystatus_sub = nh.subscribe("/file_name", 1, keystatusCallback);
	ros::Rate loop_rate(LOOPRATE);

	while (ros::ok() && !finish) {
		ros::spinOnce();
		loop_rate.sleep();
	}

	write_mail();

	return 0;
}
