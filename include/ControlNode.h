/*                                                                
 *  ControlNode.h
 *
 *  Created on: 18.01.2017
 *      Author: nindz9@gmail.com
 */

#ifndef CONTROLNODE_H_
#define CONTROLNODE_H_

#include <queue>
#include <string>
#include <ros/ros.h>

#include <control_node/TaskManager.h>
#include <control_node/BasicNavigation.h>

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
    std::queue<std::string> str_q;

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

    /* 
     * parse() goes through each string
     * in a queue and forms a service request
     */
    void parse(control_node::BasicNavigation &srv);

    /*
     * Function implements all underlying
     * tasks of Basic Navigation Test
     */
    void BasicNavigationTest();
    
    /* 
     * The same function for Basic
     * Transportation Test
     */
    void BasicTransportationTest();

public:
    void execute_test();
};

#endif /* CONTROLNODE_H_ */
