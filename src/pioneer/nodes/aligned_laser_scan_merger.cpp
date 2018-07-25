/* author: JerkoNikolic

Warning:
The node currentl assumes the first scan is the one with the wider FOV and lower point density.
This may or may not be fixed in a later version.
It also assumes the centers of the laser scan fields are aligned, just because it's what I need
and it's a lot easier.

Use at your own peril */

#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"

class LaserScanMerger {
    private:
        ros::NodeHandle node;
        std::string pubTopic, firstScanTopic, secondScanTopic;
        sensor_msgs::LaserScan firstScan, secondScan;
        int flags[2];

    public:
        LaserScanMerger();
        void firstScanReceiveCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
        void secondScanReceiveCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
        void spin();
};

LaserScanMerger::LaserScanMerger(){
    node.param<std::string>("first_scan_topic", firstScanTopic, "/first_scan");
    node.param<std::string>("second_scan_topic", secondScanTopic, "/second_scan");
    node.param<std::string>("publish_scan_topic", pubTopic, "merged_scan" );
}

void firstScanReceiveCallback(const sensor_msgs::LaserScan::ConstPtr& scan){
    ROS_INFO("nano timestamp: %s", scan->header.frame_id);
}

void secondScanReceiveCallback(const sensor_msgs::LaserScan::ConstPtr& scan){

}

void LaserScanMerger::spin(){
    node.subscribe<sensor_msgs::LaserScan>(firstScanTopic, 1, boost::bind(&LaserScanMerger::firstScanReceiveCallback, this, _1));
    node.subscribe<sensor_msgs::LaserScan>(secondScanTopic, 1, boost::bind(&LaserScanMerger::firstScanReceiveCallback, this, _1));
    node.advertise<sensor_msgs::LaserScan>(pubTopic, 2);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "aligned_laser_scan_merger");
    LaserScanMerger laserScanMerger;
    laserScanMerger.spin();
    return 0;
}