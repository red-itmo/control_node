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
    grasp_cl = nh.serviceClient<control_node::GraspObject>("grasp_obj");
    release_cl = nh.serviceClient<control_node::ReleaseObject>("release_obj");
    isSet = false;
    isUpAndRunning = false;
}

// Destructor shuts the node
ControlNode::~ControlNode()
{
    task_sub.shutdown();
    bench_state_sub.shutdown();
    grasp_cl.shutdown();
    release_cl.shutdown();
}

void ControlNode::run()
{
    ros::spin();
}

void ControlNode::task_spec_rcv(const atwork_ros_msgs::TaskInfoConstPtr spec)
{
    msg = *spec;
}

void ControlNode::bench_state_rcv(const atwork_ros_msgs::BenchmarkStateConstPtr state)
{
    bench = *state;

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

void ControlNode::nav_calibration()
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

void ControlNode::Navigation()
{
    switch(bench.phase.data)
    {
        case atwork_ros_msgs::BenchmarkState::PREPARATION:
            {
                if(!isSet && !msg.tasks.empty())
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
                        ROS_INFO("ANOTHER ONE!");
                    }
                    isSet = true;
                }
            }
            break;
        case atwork_ros_msgs::BenchmarkState::CALIBRATION:
            {
                nav_calibration();
            }
            break;
        case atwork_ros_msgs::BenchmarkState::EXECUTION:
            {
                do
                {
                    dest_ac.sendGoalAndWait(targets.front());
                    targets.pop();
                } while(!targets.empty());
            }
            break;
    }
}

void ControlNode::Manipulation()
{
    ROS_INFO("Starting Manipulation Test");
    switch(bench.phase.data)
    {
        case atwork_ros_msgs::BenchmarkState::PREPARATION:
            {
                if(!isSet && !msg.tasks.empty())
                {
                    for(int i = 0, size = msg.tasks.size(); i < size; i++)
                    {
                        if(msg.tasks[i].transportation_task.processing_team.data == "RED-ITMO")
                        {
                            obj obj_i;
                            obj_i.source = msg.tasks[i].transportation_task.source.description.data;
                            obj_i.object = msg.tasks[i].transportation_task.object;
                            obj_i.destination = msg.tasks[i].transportation_task.destination.description.data;

                            manipulation_targets.push(obj_i);
                        }
                    }
                    isSet = true;
                }
            }
            break;
        case atwork_ros_msgs::BenchmarkState::CALIBRATION:
            {
                nav_calibration();
            }
            break;
        case atwork_ros_msgs::BenchmarkState::EXECUTION:
            {
                do
                {
                    // MOVING TO SOURCE
                    navigation_step::DestGoal source_goal;
                    source_goal.dest_loc = manipulation_targets.front().source;
                    source_goal.duration = 0;
                    source_goal.orientation = "E"; // doesn't matter
                    dest_ac.sendGoalAndWait(source_goal);

                    // GRASPING
                    control_node::GraspObject grasp_srv;
                    grasp_srv.request.item = manipulation_targets.front().object;
                    if(grasp_cl.call(grasp_srv))
                    {
                        ROS_INFO("Processed Grasping Operation");
                        if(!grasp_srv.response.success) continue;
                    }

                    // MOVING TO DESTINATION
                    navigation_step::DestGoal destin_goal;
                    destin_goal.dest_loc = manipulation_targets.front().destination;
                    destin_goal.duration = 0;
                    source_goal.orientation = "E"; // doesn't matter
                    dest_ac.sendGoalAndWait(destin_goal);

                    // GRASPING
                    control_node::GraspObject release_srv;
                    release_srv.request.item = manipulation_targets.front().object;
                    if(release_cl.call(release_srv))
                    {
                        ROS_INFO("Processed Grasping Operation");
                        if(!release_srv.response.success) continue;
                    }

                    manipulation_targets.pop();
                } while(!manipulation_targets.empty());
            }
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
