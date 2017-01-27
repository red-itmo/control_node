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
                   &ControlNode::choose_task, this);
    bnt_cl = n.serviceClient<control_node::BasicNavigation>
             ("bnt_points");
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

std::vector<std::string> ControlNode::parse()
{
    std::string s = str_q.front();
    std::string del = " ";
    size_t pos = 0;
    std::vector<std::string> names;

    while((pos = s.find(del)) != std::string::npos)
    {
        names.push_back(s.substr(0, pos));
        s.erase(0, pos + del.length());
    }

    names.push_back(s);

    return names;
}

void ControlNode::BasicNavigationTest()
{
    while(!str_q.empty())
    {
        // Composing message
        std::vector<std::string> elems = parse();
        bnt_srv.request.position    = elems.at(0);
        bnt_srv.request.orientation = 
            elems.at(1).at(0);
        bnt_srv.request.delay =
            atoi(elems.at(2).c_str());

        if (bnt_cl.call(bnt_srv))
        {
            if (bnt_srv.response.sent)
            {
                str_q.pop();
            }
        }
    }
}

void ControlNode::BasicTransportationTest()
{
    // TODO
}

void ControlNode::execute_test()
{
    spin();

    switch(ts)
    {
        case TEST_BNT:
            ROS_INFO("You've chosen Basic Navigation Test");
            // This is the part where TS requested from referee-box
            // TODO
            BasicNavigationTest();
            break;
        case TEST_BTT:
            ROS_INFO("You've chosen Basic Transportation Test");
            // This is the part where TS requested from referee-box
            // TODO
            BasicTransportationTest();
            break;
    }
}
