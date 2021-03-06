    #include <ros/ros.h>
    #include <tf/transform_broadcaster.h>
    #include "std_msgs/String.h"
    #include <nav_msgs/Odometry.h>
    #include <gazebo_msgs/ModelStates.h>
    #include<gazebo_msgs/SetModelState.h>
    #include <iostream>
    
    class odomClass{
    public:
    odomClass();
    ~odomClass(){}    
    void odomTransformFunction(const gazebo_msgs::ModelStates & Model_states);
     
    private:
     ros::NodeHandle nh_;
     ros::Subscriber sub;
     ros::Publisher odom_pub;
    
    };
    
    void odomClass::odomTransformFunction(const gazebo_msgs::ModelStates& Model_states)
    {
       
       double posex = Model_states.pose[1].position.x;
       double posey = Model_states.pose[1].position.y;
       double posez = Model_states.pose[1].position.z;
       
       double orienx = Model_states.pose[1].orientation.x;
       double orieny = Model_states.pose[1].orientation.y;
       double orienz = Model_states.pose[1].orientation.z;
       double orienw = Model_states.pose[1].orientation.w;
       
       double vx = Model_states.twist[1].linear.x;
       double vy = Model_states.twist[1].linear.y;
       double angz = Model_states.twist[1].angular.z;
       
       tf::TransformBroadcaster odom_broadcaster;
       ros::Time current_time, last_time;
       current_time = ros::Time::now();
       last_time = ros::Time::now();
     
       geometry_msgs::TransformStamped odom_trans; 
       odom_trans.header.stamp = ros::Time::now();
       odom_trans.header.frame_id = "odom";
       odom_trans.child_frame_id = "base_link";
       odom_trans.transform.translation.x = posex;
       odom_trans.transform.translation.y = posey;
       //odom_trans.transform.translation.z = posez;
       odom_trans.transform.rotation.x = orienx;
       odom_trans.transform.rotation.y = orieny;
       odom_trans.transform.rotation.z = orienz;
       odom_trans.transform.rotation.w = orienw;
       
       odom_broadcaster.sendTransform(odom_trans);
       
        //next, we'll publish the odometry message over ROS
       nav_msgs::Odometry odom;
       odom.header.stamp = ros::Time::now();
       odom.header.frame_id = "odom";
       odom.child_frame_id = "base_link";
   
       //set the position
       odom.pose.pose.position.x = posex;
       odom.pose.pose.position.y = posey;
      //odom.pose.pose.position.z = posez;
       odom.pose.pose.orientation.x = orienx;
       odom.pose.pose.orientation.y = orieny;
       odom.pose.pose.orientation.z = orienz;
       odom.pose.pose.orientation.w = orienw;
       
       //set the velocity
       odom.twist.twist.linear.x = vx;
       odom.twist.twist.linear.y = vy;
       odom.twist.twist.angular.z = angz;
       
       odom_pub.publish(odom);

       last_time = current_time;
       
//        std::cout<<"pose.x="<<Model_states.pose[1].position.x<<std::endl; 
//        std::cout<<"pose.y="<<Model_states.pose[1].position.y<<std::endl; 
//        std::cout<<"pose.z="<<Model_states.pose[1].position.z<<std::endl; 
       //just test test
  
    }
    
    odomClass::odomClass(){   
    sub = nh_.subscribe("gazebo/model_states", 1000, &odomClass::odomTransformFunction, this );
    odom_pub = nh_.advertise<nav_msgs::Odometry>("odom", 10);
    
    }



    int main(int argc, char** argv)  {
        
        ros::init(argc, argv,"subscribe_to_model_state"); 
        odomClass odom_class;
        ROS_INFO("Transform to odom.");
	ros::Rate loop_rate(1000);
	while(ros::ok())
	{
	  ros::spinOnce();
	  loop_rate.sleep();
	}
      
    }
      
      
      /*
        
      tf::TransformBroadcaster odom_broadcaster;
      
      gazebo_msgs::ModelState ModelState;
      
     geometry_msgs::Pose hexapod_pose;
     
     hexapod_pose.position.x =ModelState.pose.position.x;
     
     /*
     hexapod_pose.position.x =  gazebo_msgs::ModelState::pose.position.x;  
     hexapod_pose.position.y =  gazebo_msgs::ModelState::pose.position.y;
     hexapod_pose.position.z =  gazebo_msgs::ModelState::pose.position.z;
     
     hexapod_pose.orientation.x = gazebo_msgs::ModelState::pose.orientation.x;
     hexapod_pose.orientation.y = gazebo_msgs::ModelState::pose.orientation.y;
     hexapod_pose.orientation.z = gazebo_msgs::ModelState::pose.orientation.z;
     hexapod_pose.orientation.w = gazebo_msgs::ModelState::pose.orientation.w;
*/
  
 /*     
     geometry_msgs::Twist hexapod_twist;;
     hexapod_twist.linear.x = gazebo_msgs::ModelState::twist.linear.x;
     hexapod_twist.linear.y = gazebo_msgs::ModelState::twist.linear.y;
     hexapod_twist.linear.z = gazebo_msgs::ModelState::twist.linear.z;
     hexapod_twist.angular.x = gazebo_msgs::ModelState::twist.angular.x;
     hexapod_twist.angular.y = gazebo_msgs::ModelState::twist.angular.y;
     hexapod_twist.angular.z = gazebo_msgs::ModelState::twist.angular.z;

     
     ros::Time current_time, last_time;
     current_time = ros::Time::now();
     last_time = ros::Time::now();
     ros::Rate r(1.0);
     while(n.ok()){
   
       ros::spinOnce();               // check for incoming messages
       current_time = ros::Time::now();
   
       //compute odometry in a typical way given the velocities of the robot
       double dt = (current_time - last_time).toSec();
       double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
       double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
       double delta_th = vth * dt;
       
     /*
       //since all odometry is 6DOF we'll need a quaternion created from yaw
       geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);
   
       //first, we'll publish the transform over tf
       geometry_msgs::TransformStamped odom_trans;
       odom_trans.header.stamp = current_time;
       odom_trans.header.frame_id = "odom";
       odom_trans.child_frame_id = "base_link";
   
       odom_trans.transform.translation.x = hexapod_pose.position.x;
       odom_trans.transform.translation.y = hexapod_pose.position.y;
       odom_trans.transform.translation.z = hexapod_pose.position.z;
       odom_trans.transform.rotation = odom_quat;
   
       //send the transform
       odom_broadcaster.sendTransform(odom_trans);
   
//        //next, we'll publish the odometry message over ROS
       nav_msgs::Odometry odom;
       odom.header.stamp = current_time;
       odom.header.frame_id = "odom";
   
       //set the position
       odom.pose.pose.position.x = gazebo_msgs::ModelState::pose.position.x;
       odom.pose.pose.position.y = gazebo_msgs::ModelState::pose.position.y;
       odom.pose.pose.position.z = gazebo_msgs::ModelState::pose.position.z;
       odom.pose.pose.orientation = odom_quat;
   
       //set the velocity
       odom.child_frame_id = "base_link";
       odom.twist.twist.linear.x =  gazebo_msgs::ModelState::twist.linear.x; 
       odom.twist.twist.linear.y =  gazebo_msgs::ModelState::twist.linear.y;
       odom.twist.twist.angular.z =  gazebo_msgs::ModelState::twist.angular.z;
   
       //publish the message
       odom_pub.publish(odom);
   
       last_time = current_time;
       r.sleep();
     }
   }
*/
