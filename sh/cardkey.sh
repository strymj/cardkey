#!/bin/sh

password="Strymj7290"

echo ${password} | sudo -S chmod a+rw /dev/ttyACM0 
roslaunch cardkey cardkey.launch

exit 0
