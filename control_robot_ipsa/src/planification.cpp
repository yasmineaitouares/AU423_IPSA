#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Pose2D.h>

using namespace std;

class Planning{
private:
  ros::NodeHandle _nh;
  ros::Publisher _cmdArm1Pub, _cmdArm2Pub, _cmdArm3Pub;
  ros::Subscriber _poseSub;
  std_msgs::Float64 _cmdArm1, _cmdArm2, _cmdArm3;
  float _theta1, _theta2, _alpha, _x_des, _z_des;
  float _l0, _l1, _l2, _l3, _l4;
  bool _receivedCommand;

public:
  //Constructor
  Planning(): _receivedCommand(false)
  {
    //Initialization of the publishers and the subscribers
    _cmdArm1Pub = _nh.advertise<std_msgs::Float64>("/arm1_position_controller/command", 1);
    _cmdArm2Pub = _nh.advertise<std_msgs::Float64>("/arm2_position_controller/command", 1);
    _cmdArm3Pub = _nh.advertise<std_msgs::Float64>("/arm3_position_controller/command", 1);
    _poseSub = _nh.subscribe("/desired_2Dpose_effector", 1, &Planning::poseCb, this);
    getDim();
    execute();
  }

  //Destructor
  virtual ~Planning(){
  }

  //Retrieves the configuration of the robot
  void getDim(){
    _nh.getParam("/robot_ipsa/l0", _l0);
    _nh.getParam("/robot_ipsa/l1", _l1);
    _nh.getParam("/robot_ipsa/l2", _l2);
    _nh.getParam("/robot_ipsa/l3", _l3);
    _nh.getParam("/robot_ipsa/l4", _l4);
    _nh.getParam("/robot_ipsa/alpha", _alpha);
  }

  void poseCb(const geometry_msgs::Pose2D& msg){
    _x_des = msg.x;
    _z_des = msg.y - _l0;
    _receivedCommand = true;
  }

  //Computes the orientation of each arm in order to respect the constraints of the Lab
  void computeCommands(){
    computeTheta2_1();

    _cmdArm1.data = _theta1;
    _cmdArm2.data = _theta2;
    _cmdArm3.data = _alpha - _theta2 - _theta1;

    cout << "theta1: " << _theta1 << ",\t theta2: " << _theta2 << ",\t theta3: " << _alpha - _theta1 - _theta2 << endl;

    //Publish the commands on the topics
    // _cmdArm1Pub.publish(_cmdArm1);
    // _cmdArm2Pub.publish(_cmdArm2);
    // _cmdArm3Pub.publish(_cmdArm3);
  }

  //Loop function
  void execute(){
    ros::Rate r(1);
    while(_nh.ok()){
      if(_receivedCommand)
        computeCommands();
      r.sleep();
      ros::spinOnce();
    }
  }

  //Computes the angles
  void computeTheta2_1(){
    //TODO
    //Write the inverse kinematics here
  }
};


//Main
int main(int argc, char *argv[]) {
  ros::init(argc, argv, "Planning"); //initialization of ROS
  Planning planif; //Creation of the commands planner
  ros::spin();  //To keep the main running
  return 0;
}
