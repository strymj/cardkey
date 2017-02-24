cmake_minimum_required(VERSION 2.8.3)
project(cardkey)

find_package(catkin REQUIRED COMPONENTS
	cv_bridge
	roscpp
	rospy
	image_transport
	sound_play
	std_msgs
	)

catkin_package(
	#INCLUDE_DIRS include
	LIBRARIES cardkey
	CATKIN_DEPENDS roscpp rospy sound_play std_msgs
	DEPENDS system_lib
	)

include_directories(
	${catkin_INCLUDE_DIRS}
	)


add_executable(cardkey
	src/cardkey.cpp
	)

target_link_libraries(cardkey
	${catkin_LIBRARIES}
	)

