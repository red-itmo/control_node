/*                                                                
 *  ControlNode.h
 *
 *  Created on: 18.01.2017
 *      Author: nindz9@gmail.com
 */

#ifndef CONTROLNODE_H_
#define CONTROLNODE_H_

#include <string>
#include <queue>
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <navigation_step/DestAction.h>

#include <atwork_ros_msgs/TaskInfo.h>
#include <atwork_ros_msgs/BenchmarkState.h>

class ControlNode
{
public:
    ControlNode();
    ~ControlNode();
private:
    bool isSet; // flag is related to composed scenario msg
    bool isUpAndRunning;
    ros::NodeHandle nh;
    ros::Subscriber bench_state_sub;
    ros::Subscriber task_sub; 
    atwork_ros_msgs::TaskInfo msg;
    atwork_ros_msgs::BenchmarkState bench;
    std::queue<navigation_step::DestGoal> targets;
    actionlib::SimpleActionClient<navigation_step::DestAction> dest_ac;
    void task_spec_rcv(const atwork_ros_msgs::TaskInfoConstPtr);
    void bench_state_rcv(const atwork_ros_msgs::BenchmarkStateConstPtr);
    void Navigation(); // PARAMETER TYPE???
    void Manipulation();
    void Transportation();
public:
    void run();
};

#endif /* CONTROLNODE_H_ */