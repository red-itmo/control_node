/*                                                                
 *  ControlNode.h
 *
 *  Created on: 18.01.2017
 *      Author: nindz9@gmail.com
 */

#ifndef CONTROLNODE_H_
#define CONTROLNODE_H_

#include <ros/ros.h>
#include <string>

#include <atwork_ros_msgs/TaskInfo.h>
#include <control_node/TaskManager.h>

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

    enum Mode
    {
        NONE,
        BNT,
        BMT,
        BTT
    };
    Mode m;
    //===========================
    void task_spec_rcv(const atwork_ros_msgs::TaskInfoConstPtr&);
    bool choose_task(control_node::TaskManager::Request  &req,
                     control_node::TaskManager::Response &res);
public:
    void execute();
};

#endif /* CONTROLNODE_H_ */
