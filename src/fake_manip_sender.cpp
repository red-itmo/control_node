#include <ros/ros.h>
#include <atwork_ros_msgs/TaskInfo.h>

int main(int argc, char** argv)
{
	ros::init(argc, argv, "fake_manip_sender");
	ros::NodeHandle n;
	ros::Publisher taskPub = 
        n.advertise<atwork_ros_msgs::TaskInfo>("/fake_manip_info", 10, true);

    atwork_ros_msgs::TaskInfo ti;
    //TODO

    while(n.ok())
    {
    	taskPub.publish(ti);
    }

    return 0;
}