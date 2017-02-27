/*
 * ControlNode.cpp
 * 
 *  Created on: 25.01.2017
 *      Author: nindz9@gmail.com
 */

#include <ControlNode.h>

// Constructor starts service server
ControlNode::ControlNode()
{
    task_sub = nh.subscribe("/robot_example_ros/task_info", 1000, 
            &ControlNode::task_spec_rcv, this);
    //task_manager = nh.advertiseService("task_manager",
    //        &ControlNode::choose_task, this);
    //m = NONE;
}

// Destructor shuts the node
ControlNode::~ControlNode()
{
    task_sub.shutdown();
}

void ControlNode::run()
{
    ros::Rate loop_rate(10);
    do
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    while(msg.tasks.empty());

    for(unsigned short i = 0, size = msg.tasks.size(); i < size; i++)
    {
        atwork_ros_msgs::Task task = msg.tasks[i];
        switch(task.type.data)
        {
            case atwork_ros_msgs::Task::NAVIGATION:
                //TODO
                break;
            case atwork_ros_msgs::Task::TRANSPORTATION:
                //TODO
                break;
        }
    }
}

void ControlNode::task_spec_rcv(const atwork_ros_msgs::TaskInfoConstPtr spec)
{
    msg = *spec;
}

void ControlNode::Navigation()
{
    //TODO
}

void ControlNode::Manipulation()
{
    //TODO
}

void ControlNode::Transportation()
{
    //TODO
}

/**bool ControlNode::choose_task(control_node::TaskManager::Request  &req,
                              control_node::TaskManager::Response &res)
{
    switch(req.test)
    {
        case 1:
            m = BNT;
            res.ans = "Execute Basic Navigation Test!";
            return true;
        case 2:
            m = BMT;
            res.ans = "Execute Basic Manipulation Test!";
            return true;
        case 3:
            m = BTT;
            res.ans = "Execute Basic Transportation Test!";
            return true;
        default:
            ROS_INFO("Enter either 1, 2 or 3");
            return false;
    }
}**/
