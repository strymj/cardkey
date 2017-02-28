#!/bin/sh

password="mec0"

echo ${password} | sudo -S chmod a+rw /dev/ttyACM0 
roslaunch cardkey cardkey.launch

exit 0
