/*
 * ControlNode.cpp
 * 
 *  Created on: 25.01.2017
 *      Author: nindz9@gmail.com
 */

#include <ControlNode.h>

// Constructor starts service server
ControlNode::ControlNode():
    dest_ac("navi", true)
{
    task_sub = nh.subscribe("/robot_example_ros/task_info", 1000, 
            &ControlNode::task_spec_rcv, this);
    while(!dest_ac.waitForServer()){
        ROS_INFO("Waiting for the action server to come up");
    }
    ROS_INFO("Action Server started!");
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
        switch(msg.tasks[i].type.data)
        {
            case atwork_ros_msgs::Task::NAVIGATION:
                Navigation(msg.tasks[i].navigation_task);
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

void ControlNode::Navigation(const atwork_ros_msgs::NavigationTask& nav_info)
{
    navigation_step::DestGoal dest_goal;
    dest_goal.dest_loc    = nav_info.location.description.data;
    dest_goal.orientation = nav_info.orientation.data;
    dest_goal.duration    = nav_info.wait_time.data.toSec();

    dest_ac.sendGoal(dest_goal);
    dest_ac.waitForResult();
    navigation_step::DestResult res_ptr = *(dest_ac.getResult());
    if(res_ptr.has_got)
    {
        return;
    }
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
