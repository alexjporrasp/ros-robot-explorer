# ros-robot-explorer
(Robotics class project) Implementation of the behavior a robot that has to explore an area as large as possible within an unknown space and in a given time. (performs SLAM as the robot is not aware of its own pose either)

The developed algorithm is part of a ROS C++ node. It was testesd employing the stage simulator. Stage is a robot simulator, it provides a virtual world populated by mobile robots and sensors, along with various objects for the robots to sense and manipulate. ROS provides a node, called 'stage_ros', which wraps the robotic bases yielding odometry information.

If you want to install ROS in your computer, you can follow the detailed guide in http://wiki.ros.org/kinetic/Installation and install de desktop-full version of ROS.

To launch the code you just need to type the following

roslaunch missions_pkg demo_3_robotic_explorer.launch

The logic of the robot is within

src/robotic_explorer/src/robotic_explorer_node.cpp

What I did was make the robot follow the fursthest point wihtin a limited angle range of laser scans. Although it is very simple it  performed reasonably good. 
