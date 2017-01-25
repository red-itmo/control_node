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
    ros::spin();

    return 0;
}
