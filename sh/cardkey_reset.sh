#!/bin/sh

LOOP=0
#RESET_TIME="04:00"

while :
do
	gnome-terminal --geometry 40x11+1200+400 -e 'bash -c "sh ~/catkin_ws/src/cardkey/sh/cardkey.sh"'

	echo "Running ${LOOP} hours.\n"
	LOOP=$(( LOOP + 1 ))

	sleep 3580
	
	#echo "Running ${LOOP} days.\n"
	#LOOP=$(( LOOP + 1 ))

	#sleep 60

	#NOW_TIME=`date '+%H:%M'`
	#while [ ${RESET_TIME} != ${NOW_TIME} ]
	#do
	#	sleep 60
	#	NOW_TIME=`date '+%H:%M'`
	#done
	
	echo "Restart cardkey program after 10 seconds."
	sleep 10
	ps -e | grep roslaunch | grep -v grep | awk '{ print "kill -2", $1 }' | sh
	echo "Restarted.\n\n\n"
	sleep 10
done

exit 0
