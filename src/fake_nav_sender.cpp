#include <ros/ros.h>
#include <atwork_ros_msgs/TaskInfo.h>
#include <std_msgs/Time.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "fake_nav_sender");
    ros::NodeHandle n;
    ros::Publisher taskPub = 
        n.advertise<atwork_ros_msgs::TaskInfo>("/fake_nav_info", 10, true);
    
    atwork_ros_msgs::TaskInfo ti;
    std_msgs::Time time;
    time.data = ros::Time(3.0);

    
    atwork_ros_msgs::Task task1;
    task1.type.data = atwork_ros_msgs::Task::NAVIGATION;
    task1.navigation_task.location.description.data = "P1";
    task1.navigation_task.orientation.data = 
                    atwork_ros_msgs::NavigationTask::NORTH;
    task1.navigation_task.wait_time = time;
    ti.tasks.push_back(task1);

    atwork_ros_msgs::Task task2;
    task2.type.data = atwork_ros_msgs::Task::NAVIGATION;
    task2.navigation_task.location.description.data = "P2";
    task2.navigation_task.orientation.data = 
                    atwork_ros_msgs::NavigationTask::EAST;
    task2.navigation_task.wait_time = time;
    ti.tasks.push_back(task2);

    atwork_ros_msgs::Task task3;
    task3.type.data = atwork_ros_msgs::Task::NAVIGATION;
    task3.navigation_task.location.description.data = "P3";
    task3.navigation_task.orientation.data = 
                    atwork_ros_msgs::NavigationTask::WEST;
    task3.navigation_task.wait_time = time;
    ti.tasks.push_back(task3);

    while(n.ok())
    {
        taskPub.publish(ti);
    }

    return 0;
}
