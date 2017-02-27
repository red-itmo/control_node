/*
 * main.cpp
 * 
 *  Created on: 25.01.2017
 *      Author: nindz9@gmail.com
 */

#include <ControlNode.h>

#include <ros/ros.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "control_node");
    ControlNode cn;
    ROS_INFO("PLEASE, call a \"task_manager\" service with:");
    ROS_INFO("1 = BNT, 2 = BMT, 3 = BTT");
    cn.run();

    return 0;
}
