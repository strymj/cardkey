#!/bin/sh

LOOP=0

while [ ${LOOP} -ge 0 ]
do
	LOOP=`expr $LOOP + 1`
	echo "<Restarted ${LOOP}>"
	gnome-terminal --geometry 40x11+1200+400 -e 'bash -c "sh ~/catkin_ws/src/cardkey/sh/cardkey.sh"'
	sleep 3600
	echo "Restart ID-program in 10 seconds."
	sleep 10
	ps -e | grep roslaunch | grep -v grep | awk '{ print "kill -2", $1 }' | sh
	sleep 5
done

exit 0
