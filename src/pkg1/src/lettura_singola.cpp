#include <ros/ros.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <actionlib/client/simple_action_client.h>
#include <sensor_msgs/JointState.h>
#include <iostream>

//flag globale utile per capire è stata realizzata la singola lettura dal topic
bool message_received = false;

//vairabile globale per la lettura della configurazione iniziale
sensor_msgs::JointState q0;


int main (int argc, char **argv)
{
    ros::init(argc, argv, "reader");

    ros::NodeHandle n;
    
    // Attende il singolo messaggio
    ros::Time start=ros::Time::now();
    auto msg=ros::topic::waitForMessage<sensor_msgs::JointState>("/joint_states",ros::Duration(1.0));
    
    
      for (size_t i = 0; i < msg->name.size(); ++i) 
      {
          ROS_INFO("Nome: %s, Posizione: %f", msg->name[i].c_str(), msg->position[i]);
      }

    q0=*msg;
      //faccio anche questa stampa per capire se q0 è stata riempita
       for (size_t i = 0; i < msg->name.size(); ++i) 
      {
          ROS_INFO("Posizione in q0 %f", q0.position[i]);
      }
    

    ros::spin();

    //exit
    return 0;
}

