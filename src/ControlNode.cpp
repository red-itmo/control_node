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
    task_manager = n.advertiseService("task_manager", 
                   &ControlNode::choose_task,
                   this);
    waiting = true;
    
    ROS_INFO("Awaiting task to be named");
}

// Destructor shuts the node
ControlNode::~ControlNode()
{
    task_manager.shutdown();
}


bool ControlNode::choose_task(control_node::TaskManager::Request  &req,
                              control_node::TaskManager::Response &res)
{
   if (req.test == "BNT")
   {
       ts = TEST_BNT;
       waiting = false;
       return true;
   }
   else if (req.test == "BTT")
   {
       ts = TEST_BTT;
       waiting = false;
       ROS_INFO("You've chosen Basic Transportation Test");
       return true;
   }
   else
   {
       ROS_ERROR("WRONG! Your only options are \"BNT\" and \"BTT\"");
       return false;
   }
}

void ControlNode::spin()
{
    ros::Rate loop_rate(5.0);
    while(n.ok() && waiting)
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
}

void ControlNode::execute_test()
{
    spin();

    switch(ts)
    {
        case TEST_BNT:
            ROS_INFO("You've chosen Basic Navigation Test");
            break;
        case TEST_BTT:
            ROS_INFO("You've chosen Basic Transportation Test");
            break;
    }
}
