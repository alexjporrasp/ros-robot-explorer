#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>


#include <vector>
#include <stdlib.h>
#include <cmath>

const double PI = 3.141592653;
const double WALL_DISTANCE = 0.5;

float g_angle;
float g_velocity;


void FarthestInRange(const sensor_msgs::LaserScan::ConstPtr &msg)
{

	int min_index = ((-65*PI/180) - msg->angle_min) / msg->angle_increment;
	int max_index = ((65*PI/180) - msg->angle_min) / msg->angle_increment;

	int min_collision_index = ((-PI/3) - msg->angle_min) / msg->angle_increment;
	int max_collision_index = ((PI/3) - msg->angle_min) / msg->angle_increment;

	float farthest_in_range = 0;
	int farthest_index = min_index;

	for (int i = min_index; i < max_index; i++) 
	{
		if (msg->ranges.at(i) > farthest_in_range)
		{
			farthest_in_range = msg->ranges.at(i);
			farthest_index = i;
		}
	}

	float nearest_in_range = std::numeric_limits<float>::max();
	float nearest_index = 0;

	for (int i = min_collision_index; i < max_collision_index; i++)
	{
		if (msg->ranges.at(i) < nearest_in_range)
		{
			nearest_in_range = msg->ranges.at(i);
			nearest_index = i;
		}
	}

	float angle_to_farthest = msg->angle_increment * farthest_index + msg->angle_min;

	g_angle = angle_to_farthest;

	float angle_to_nearest = msg->angle_increment * nearest_index + msg->angle_min;

	
	if (farthest_in_range < 1)
	{
		g_velocity = 0;
		g_angle -= 3;
	} else if (nearest_in_range < WALL_DISTANCE && angle_to_nearest < 0)
	{
		g_velocity = 0;
		g_angle += 2;
	} else if (nearest_in_range < WALL_DISTANCE && angle_to_nearest >= 0)
	{
		g_velocity = 0;
		g_angle -= 2;
	} 
	else
	{
		g_velocity = 1;
	}
	
}


int main(int argc, char **argv)
{

	// Initialize the ROS system and become a node.

	ros::init( argc , argv , "exploring") ;

    ROS_INFO("[robot_explorer] Robotic explorer node running and initialized! Let's have some fun!");

	ros::NodeHandle nh;

	// Create a publisher object
	ros::Publisher pub = nh.advertise <geometry_msgs::Twist>("/cmd_vel" , 1000);
	// Create a subscriber object
	// ros::Subscriber sub = nh.subscribe("/laser_scan", 1000, processScanCallback);

	ros::Subscriber sub = nh.subscribe("/laser_scan", 1000, FarthestInRange);

	// Seed the random number generator.
	srand ( time (0) ) ;

	// Loop at 2Hz until the node is shut down.
  	ros::Rate rate (2) ;
  	ros::Time begin = ros::Time::now();

  	while ( begin.toSec() == 0 )
    	begin = ros::Time::now();

  	double ellapsed_time = 0;

  	while ( ( ros::ok () ) && ( ellapsed_time < 60*5 ) )
	{

	// Create and fill in the message. The other four
	// fields , which are ignored by stage, default to 0.

	// TO CHANGE (START)
	
	geometry_msgs::Twist msg;
	msg.linear.x = g_velocity;
	// msg.angular.z = ( 2*double(rand()) / double(RAND_MAX) ) - 1;
	msg.angular.z = g_angle;
	// Publish the message.
	pub.publish(msg);

	// TO CHANGE (FINISH)

	// Send a message to rosout with the details .
	// ROS_INFO("[robot_explorer] Sending random velocity command: linear= %.2f angular=%.2f", msg.linear.x, msg.angular.z );

	// Wait until it 's time for another iteration .
	rate.sleep () ;
	ros::spinOnce(); // give time to receive /base_scan messages

	ros::Time current = ros::Time::now();
	ellapsed_time = (current - begin).toSec();
	ROS_INFO("[robot_explorer] Ellpased time: %.2f", ellapsed_time );

	}
}
