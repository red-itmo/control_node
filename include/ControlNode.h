/*                                                                
 * JoystickTeleop.h
 *
 *  Created on: 18.01.2017
 *      Email: nindz9@gmail.com
 *          ITMO University
 *          Robotics Engineering Department
 */

#ifndef CONTROLNODE_H_
#define CONTROLNODE_H_

class ControleNode
{
public:
    ControlNode();
    ~ControlNode();

private:
    ros::NodeHandle n;
    unsigned int mode;
    

    void initialize(ros::NodeHandle &nh);

};

#endif /* CONTROLNODE_H_ */
