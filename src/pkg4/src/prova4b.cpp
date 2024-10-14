#include <ros/ros.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <actionlib/client/simple_action_client.h>
#include <sensor_msgs/JointState.h>
#include <iostream>


typedef actionlib::SimpleActionClient< control_msgs::FollowJointTrajectoryAction> TrajClient;

class RobotArm
{
private:
  // Action client for the joint trajectory action 
  // used to trigger the arm movement action
  TrajClient* traj_client_;
  std::vector<std::string> joint_names_ = {"panda_joint1",
                                             "panda_joint2",
                                             "panda_joint3",
                                             "panda_joint4",
                                             "panda_joint5",
                                             "panda_joint6",
                                             "panda_joint7"};   
    sensor_msgs::JointState q0;


public:
  //! Initialize the action client and wait for action server to come up
  RobotArm() 
  {
    // tell the action client that we want to spin a thread by default
    traj_client_ = new TrajClient("/position_joint_trajectory_controller/follow_joint_trajectory", true);

    // wait for action server to come up
    while(!traj_client_->waitForServer(ros::Duration(5.0))){
      ROS_INFO("Waiting for the joint_trajectory_action server");
    }
  }

  //! Clean up the action client
  ~RobotArm()
  {
    delete traj_client_;
  }

  //! Sends the command to start a given trajectory
  void startTrajectory(control_msgs::FollowJointTrajectoryGoal goal)
  {
    // When to start the trajectory: 1s from now
    goal.trajectory.header.stamp = ros::Time::now() + ros::Duration(1.0);
    traj_client_->sendGoal(goal);
  }

  //! Generates a simple trajectory with two waypoints, used as an example
  /*! Note that this trajectory contains two waypoints, joined together
      as a single trajectory. Alternatively, each of these waypoints could
      be in its own trajectory - a trajectory can have one or more waypoints
      depending on the desired application.
  */
  control_msgs::FollowJointTrajectoryGoal armExtensionTrajectory()
  {
    //our goal variable
    control_msgs::FollowJointTrajectoryGoal goal;

    // First, the joint names, which apply to all waypoints
    goal.trajectory.joint_names=joint_names_;

    // We will have two waypoints in this goal trajectory
    goal.trajectory.points.resize(2);

    ros::NodeHandle n;
    
    // Attendi il singolo messaggio e salvalo nella variabile globale
    ros::Time start=ros::Time::now();
    auto msg=ros::topic::waitForMessage<sensor_msgs::JointState>("/joint_states",ros::Duration(1.0));
      for (size_t i = 0; i < msg->name.size(); ++i) 
      {
          ROS_INFO("Nome: %s, Posizione: %f", msg->name[i].c_str(), msg->position[i]);
      }

    q0=*msg;
    // First trajectory point
    // Positions
    goal.trajectory.points[0].positions.resize(7);
    goal.trajectory.points[0].positions=q0.position;

    // Velocities
    goal.trajectory.points[0].velocities.resize(7);
    for (size_t j = 0; j < 7; ++j)
    {
      goal.trajectory.points[0].velocities[j] = 0.0;
    }
    // To be reached 1 second after starting along the trajectory
    goal.trajectory.points[0].time_from_start = ros::Duration(1.0);

    // Second trajectory point
    // Positions
    goal.trajectory.points[1].positions.resize(7);
    goal.trajectory.points[1].positions=goal.trajectory.points[0].positions;
    goal.trajectory.points[1].positions[6]+=M_PI/4;
    // Velocities
    goal.trajectory.points[1].velocities.resize(7);
    for (size_t j = 0; j < 7; ++j)
    {
      goal.trajectory.points[1].velocities[j] = 0.0;
    }
    // To be reached 2 seconds after starting along the trajectory
    goal.trajectory.points[1].time_from_start = ros::Duration(8.0);

    //we are done; return the goal
    return goal;
  }

  //! Returns the current state of the action
  actionlib::SimpleClientGoalState getState()
  {
    return traj_client_->getState();
  }
 
};

int main(int argc, char** argv)
{
  // Init the ROS node
  ros::init(argc, argv, "robot_driver");

  RobotArm arm;
  // Start the trajectory
  arm.startTrajectory(arm.armExtensionTrajectory());
  // Wait for trajectory completion
  while(!arm.getState().isDone() && ros::ok())
  {
    usleep(50000);
  }
  return 0;
}