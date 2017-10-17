#include <ros/ros.h>
#include <motion_decoder/image_converter.hpp>
#include <apriltags_ros/AprilTagDetectionArray.h>
#include <apriltags_ros/AprilTagDetection.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>
#include <string>
#include <sstream>

using namespace std;

ImageConverter* ic;

void apriltag_detection_callback(const apriltags_ros::AprilTagDetectionArray msg)
{
    ROS_INFO("In subscribe\n");

    static tf::TransformBroadcaster br; // for later
    tf::Transform transform;

    for (int i = 0; i < msg.detections.size(); i ++)
    {
    	geometry_msgs::PoseStamped current_pose = msg.detections[i].pose;
    	double xLoc = current_pose.pose.position.x;
    	double yLoc = current_pose.pose.position.y;
    	double zLoc = current_pose.pose.position.z;

    	double quatX = current_pose.pose.orientation.x;
    	double quatY = current_pose.pose.orientation.y;
    	double quatZ = current_pose.pose.orientation.z;
    	double quatW = current_pose.pose.orientation.w;

    	// quaternion
    	transform.setRotation(tf::Quaternion(quatX, quatY, quatZ, quatW));

    	// origin
    	transform.setOrigin(tf::Vector3(xLoc, yLoc, zLoc));

    	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "camera", "april_tf"));

    }



}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_converter");
  
    ros::NodeHandle n;
  //TODO: Add a subscriber to get the AprilTag detections The callback function skeleton is given.
  
    // subscribing to AprilTag-ros
	ros::Subscriber motion_sub = n.subscribe("tag_detections", 1000, apriltag_detection_callback);


  
    ImageConverter converter;
    ros::Rate loop_rate(50);
    ROS_INFO("In main\n");

    while(ros::ok()) 
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
