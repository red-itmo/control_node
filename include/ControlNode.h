/*                                                                
 *  ControlNode.h
 *
 *  Created on: 18.01.2017
 *      Author: nindz9@gmail.com
 */

#ifndef CONTROLNODE_H_
#define CONTROLNODE_H_

#include <string>
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <navigation_step/DestAction.h>

#include <atwork_ros_msgs/TaskInfo.h>
//#include <control_node/TaskManager.h>

class ControlNode
{
public:
    ControlNode();
    ~ControlNode();
private:
    //===========================
    ros::NodeHandle nh;
    ros::Subscriber task_sub; 
    ros::ServiceServer task_manager;
    atwork_ros_msgs::TaskInfo msg;
    actionlib::SimpleActionClient<navigation_step::DestAction> dest_ac;
    //===========================
    void task_spec_rcv(const atwork_ros_msgs::TaskInfoConstPtr);
    //bool choose_task(control_node::TaskManager::Request  &req,
    //                 control_node::TaskManager::Response &res);
    void Navigation(const atwork_ros_msgs::NavigationTask& nav_info);
    void Manipulation();
    void Transportation();
public:
    void run();
};

#endif /* CONTROLNODE_H_ */
