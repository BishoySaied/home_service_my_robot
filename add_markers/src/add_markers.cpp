#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <string>
#include <sstream>
#include "add_markers/CtrMarkerObj.h"

using namespace std;

ros::Publisher marker_pub;

// Initialize marker.
void initialize_marker(visualization_msgs::Marker& marker){
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();
    marker.ns = "cube";
    marker.id = 0;
  // cuber marker
    marker.type = visualization_msgs::Marker::CUBE;
  // marker size
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;
  //marker color
    marker.color.r = 1.0;
    marker.color.g = 0.0;
    marker.color.b = 0.0;
    marker.color.a = 1.0;
}

// Handle for sevice /add_markers/CtrMarkerObj.
bool handle_marker_ctr_obj(add_markers::CtrMarkerObj::Request& req, add_markers::CtrMarkerObj::Response& res){
    ros::Rate r(1);

    // Declare message for topic visualization_marker.
    visualization_msgs::Marker marker;

    // Initialize marker.
    initialize_marker(marker);

    // check if the marker is requsted to be added or deleted, infor from pick_up node
    if (req.to_add)
        marker.action = visualization_msgs::Marker::ADD;
    else
        marker.action = visualization_msgs::Marker::DELETE;

    // Set position for marker
    marker.pose.position.x = req.x;
    marker.pose.position.y = req.y;
    marker.pose.orientation.w = 1.0;

    // Print the message, which acknowledges request receiving.
    ROS_INFO("Receive request to %s marker at (%s, %s)", (req.to_add? "add" : "remove"), to_string(req.x).c_str(), to_string(req.y).c_str());

    marker.lifetime = ros::Duration();
    // Wait for visualization_marker topic subscribers
    while (marker_pub.getNumSubscribers() < 1)
    {
        if (!ros::ok())
            return -1;
        ROS_WARN_ONCE("Please create a subscriber to the marker");
        sleep(1);
    }

    // Publish topic visualization_marker to rviz: show or delete marker.
    marker_pub.publish(marker);
    res.msg_feedback = "Done";
    return true;
};



int main( int argc, char** argv )
{
    ros::init(argc, argv, "add_markers");
    ros::NodeHandle n;
    ros::ServiceServer service = n.advertiseService("/add_markers/CtrMarkerObj", handle_marker_ctr_obj);
    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    ROS_INFO("Ready to handle requests");

    if (argc == 1){
        ros::spin(); // Wait for requests.
    }
    else if (argc == 5){
        // Wait for visualization_marker topic subscribers
        while (marker_pub.getNumSubscribers() < 1)
        {
            if (!ros::ok())
                return -1;
            ROS_WARN_ONCE("Please create a subscriber to the marker");
            sleep(1);
        }

        // Declare message for topic visualization_marker.
        visualization_msgs::Marker marker;

        // Initialize marker.
        initialize_marker(marker);


    } else {
        ROS_ERROR("Wrong command line");
        return -1;
    }
    return 0;
}
