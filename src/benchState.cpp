#include <ros/ros.h>
#include <std_srvs/Empty.h>
#include <atwork_ros_msgs/BenchmarkState.h>

atwork_ros_msgs::BenchmarkState bState;

bool changeStateCb(std_srvs::Empty::Request& req,
				   std_srvs::Empty::Response& res)
{
	bState.phase.data--;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "benchState");
	ros::NodeHandle nh;
	ros::Publisher benchPub = 
		nh.advertise<atwork_ros_msgs::BenchmarkState>("/robot_example_ros/benchmark_state", 10, true);
	ros::ServiceServer service = nh.advertiseService("switch_state", &changeStateCb);

	bState.benchmark_time.data = ros::Time::now();
	bState.phase.data = 2;
	bState.state.data = 1;
	bState.scenario.type.data = 1;
	bState.scenario.type_id.data = 1;
	bState.scenario.description.data = "BNT 1: Basic Navigation Test instance #1";

	ROS_INFO("Ready to recieve from client!");
	while(nh.ok())
	{
		ros::spinOnce();
		benchPub.publish(bState);
	}
}