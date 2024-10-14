#include <ros/ros.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <actionlib/client/simple_action_client.h>
#include <sensor_msgs/JointState.h>


void messageCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
    // Stampa delle variabili di giunto
    for (size_t i = 0; i < msg->name.size(); ++i) 
    {
        ROS_INFO("Nome: %s, Posizione: %f", msg->name[i].c_str(), msg->position[i]);
    }
}



int main (int argc, char **argv)
{
      ros::init(argc, argv, "reader");

      ros::NodeHandle n;
      ros::Subscriber subscriber = n.subscribe("joint_states", 1000, messageCallback);

      ros::spin();

      //exit
      return 0;
}



