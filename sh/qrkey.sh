#!/bin/sh

echo "mec0" | sudo -S chmod a+rw /dev/ttyACM0 
roslaunch cardkey cardkey.launch

exit 0
