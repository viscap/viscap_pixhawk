#include <cstdlib>

#include <ros/ros.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/SetMode.h>

int main(int argc, char **argv)
{

    int rate = 10;

    ros::init(argc, argv, "mavros_takeoff");
    ros::NodeHandle n;

    ros::Rate r(rate);

    ////////////////////////////////////////////
    /////////////////GUIDED/////////////////////
    ////////////////////////////////////////////
    ros::ServiceClient cl = n.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
    mavros_msgs::SetMode srv_setMode;
    srv_setMode.request.base_mode = 0;
    srv_setMode.request.custom_mode = "GUIDED";
    if(cl.call(srv_setMode)){
        ROS_ERROR("setmode send ok %d value:", srv_setMode.response.success);
    }else{
        ROS_ERROR("Failed SetMode");
        return -1;
    }

    ////////////////////////////////////////////
    ///////////////////ARM//////////////////////
    ////////////////////////////////////////////
    ros::ServiceClient arming_cl = n.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
    mavros_msgs::CommandBool srv;
    srv.request.value = true;
    if(arming_cl.call(srv)){
        ROS_ERROR("ARM send ok %d", srv.response.success);
    }else{
        ROS_ERROR("Failed arming or disarming");
    }

    ////////////////////////////////////////////
    /////////////////TAKEOFF////////////////////
    ////////////////////////////////////////////
    ros::ServiceClient takeoff_cl = n.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/takeoff");
    mavros_msgs::CommandTOL srv_takeoff;
    srv_takeoff.request.altitude = 5;
    srv_takeoff.request.latitude = 0;
    srv_takeoff.request.longitude = 0;
    srv_takeoff.request.min_pitch = 0;
    srv_takeoff.request.yaw = 0;
    if(takeoff_cl.call(srv_takeoff)){
        ROS_ERROR("srv_takeoff send ok %d", srv_takeoff.response.success);
    }else{
        ROS_ERROR("Failed Takeoff");
    }

    ////////////////////////////////////////////
    /////////////////DO STUFF///////////////////
    ////////////////////////////////////////////
    sleep(10);

	ros::Publisher vel_pub = n.advertise<geometry_msgs::TwistStamped>("/mavros/setpoint_velocity/cmd_vel", 1000);
	geometry_msgs::TwistStamped msg;

	ros::Time beginTime;
	ros::Duration stepDuration = ros::Duration(2);

	for( int i = 0; i < 3; i++ ) {

		// Frente
		msg.twist.linear.x = 0.5;
		msg.twist.linear.y = 0.0;
		msg.twist.linear.z = 0.0;
		beginTime = ros::Time::now();
		while(ros::Time::now() < beginTime + stepDuration) {
			vel_pub.publish(msg);
			r.sleep();
		}

		// Direita
		msg.twist.linear.x = 0.0;
		msg.twist.linear.y = 0.5;
		msg.twist.linear.z = 0.0;
		beginTime = ros::Time::now();
		while(ros::Time::now() < beginTime + stepDuration) {
			vel_pub.publish(msg);
			r.sleep();
		}

		// Tras
		msg.twist.linear.x = -0.5;
		msg.twist.linear.y = 0.0;
		msg.twist.linear.z = 0.0;
		beginTime = ros::Time::now();
		while(ros::Time::now() < beginTime + stepDuration) {
			vel_pub.publish(msg);
			r.sleep();
		}

		// Esquerda
		msg.twist.linear.x = 0.0;
		msg.twist.linear.y = -0.5;
		msg.twist.linear.z = 0.0;
		beginTime = ros::Time::now();
		while(ros::Time::now() < beginTime + stepDuration) {
			vel_pub.publish(msg);
			r.sleep();
		}

	}

	sleep(10);

    ////////////////////////////////////////////
    ///////////////////LAND/////////////////////
    ////////////////////////////////////////////
    ros::ServiceClient land_cl = n.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/land");
    mavros_msgs::CommandTOL srv_land;
    srv_land.request.altitude = 5;
    srv_land.request.latitude = 0;
    srv_land.request.longitude = 0;
    srv_land.request.min_pitch = 0;
    srv_land.request.yaw = 0;
    if(land_cl.call(srv_land)){
        ROS_INFO("srv_land send ok %d", srv_land.response.success);
    }else{
        ROS_ERROR("Failed Land");
    }

    while (n.ok())
    {
      ros::spinOnce();
      r.sleep();
    }

    return 0;
}
