/*
 * ControlNode.cpp
 * 
 *  Created on: 25.01.2017
 *      Author: nindz9@gmail.com
 */

#include <ControlNode.h>

ControlNode::ControlNode():
    dest_ac("navi", true) 
{       
    //just to test, subscribe to fake_sender's publisher
    task_sub = nh.subscribe("/fake_nav_info", 1000, 
            &ControlNode::task_spec_rcv, this);
    bench_state_sub = 
        nh.subscribe("/robot_example_ros/benchmark_state", 1000, 
            &ControlNode::bench_state_rcv, this);
    isSet = false;
    isUpAndRunning = false;
}

// Destructor shuts the node
ControlNode::~ControlNode()
{
    task_sub.shutdown();
    bench_state_sub.shutdown();
}

void ControlNode::run()
{
    ros::Rate loop_rate(10);
    do
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    while((bench.connected_teams.empty() || msg.tasks.empty()) && nh.ok());

    switch(bench.scenario.type.data)
    {
            case atwork_ros_msgs::BenchmarkScenario::BNT:
                Navigation();
                break;
            case atwork_ros_msgs::BenchmarkScenario::BMT:
                Manipulation();
                break;
            case atwork_ros_msgs::BenchmarkScenario::BTT:
                Transportation();
                break;
    }
}

void ControlNode::task_spec_rcv(const atwork_ros_msgs::TaskInfoConstPtr spec)
{
    msg = *spec;
}

void ControlNode::bench_state_rcv(const atwork_ros_msgs::BenchmarkStateConstPtr state)
{
    bench = *state;
}

void ControlNode::Navigation()
{
    switch(bench.phase.data)
    {
        case atwork_ros_msgs::BenchmarkState::PREPARATION:
            {
                if(!isSet)
                {
                    for(int i = 0, size = msg.tasks.size(); i < size; i++)
                    {
                        navigation_step::DestGoal dest_goal;
                        dest_goal.dest_loc = msg.tasks[i].navigation_task.location.description.data;
                        dest_goal.duration = msg.tasks[i].navigation_task.wait_time.data.toSec();
                        switch(msg.tasks[i].navigation_task.orientation.data)
                        {
                            case atwork_ros_msgs::NavigationTask::NORTH:
                                dest_goal.orientation = "N";
                                break;
                            case atwork_ros_msgs::NavigationTask::EAST:
                                dest_goal.orientation = "E";
                                break;
                            case atwork_ros_msgs::NavigationTask::SOUTH:
                                dest_goal.orientation = "S";
                                break;
                            case atwork_ros_msgs::NavigationTask::WEST:
                                dest_goal.orientation = "W";
                                break;
                        }
                        targets.push(dest_goal);
                    }
                    isSet = true;
                }
            }
            break;
        case atwork_ros_msgs::BenchmarkState::CALIBRATION:
            {
                if(!isUpAndRunning)
                {
                    while(!dest_ac.waitForServer()){
                        ROS_INFO("Waiting for the action server to come up");
                    }
                    ROS_INFO("Action Server started!");

                    isUpAndRunning = true;
                }
            }
            break;
        case atwork_ros_msgs::BenchmarkState::EXECUTION:
            {
                do
                {
                    dest_ac.sendGoal(targets.front());
                    dest_ac.waitForResult();
                    if(dest_ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                    {
                        targets.pop();
                    }
                } while(!targets.empty());
            }
            break;
    }
}

void ControlNode::Manipulation()
{
    switch(bench.phase.data)
    {
        case atwork_ros_msgs::BenchmarkState::PREPARATION:
            // TODO
            break;
        case atwork_ros_msgs::BenchmarkState::CALIBRATION:
            // TODO
            break;
        case atwork_ros_msgs::BenchmarkState::EXECUTION:
            // TODO
            break;
    }
}

void ControlNode::Transportation()
{
    switch(bench.phase.data)
    {
        case atwork_ros_msgs::BenchmarkState::PREPARATION:
            // TODO
            break;
        case atwork_ros_msgs::BenchmarkState::CALIBRATION:
            // TODO
            break;
        case atwork_ros_msgs::BenchmarkState::EXECUTION:
            // TODO
            break;
    }
}