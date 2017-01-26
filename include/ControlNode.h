/*                                                                
 *  ControlNode.h
 *
 *  Created on: 18.01.2017
 *      Author: nindz9@gmail.com
 */

#ifndef CONTROLNODE_H_
#define CONTROLNODE_H_

#include <ros/ros.h>
#include <control_node/TaskManager.h>

class ControlNode
{
public:
    ControlNode();
    ~ControlNode();

private:
    // MEMBER VARIABLES
    ros::NodeHandle n;
    ros::ServiceServer task_manager;
    bool waiting;

    enum TestSpec 
    {
        TEST_BNT,
        TEST_BTT
    };
    TestSpec ts;

    // MEMBER FUNCTIONS (methods)
    /*
     * As soon as there is a request, 
     * compare it to a string literal
     * and print some output
     */
    bool choose_task(control_node::TaskManager::Request  &req,
                     control_node::TaskManager::Response &res);

    /*
     * This version of spin rolls
     * untill proper test name wasn't specified
     */
    void spin();

public:
    void execute_test();
};

#endif /* CONTROLNODE_H_ */
