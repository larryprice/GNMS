#! /bin/bash

# change the screen resolution
xrandr --output VGA1 --mode 1280x960 --rate 75

# change the camera from 30FPS to 60FPS
echo "touchgrp6" | sudo -S modprobe -r gspca-ov534
echo "touchgrp6" | sudo -S modprobe gspca-ov534 videomode=04

# start reacTIVision with sudo to get high priority
# run in background
echo "touchgrp6" | sudo -S reacTIVision &

# start GNMS OS
echo "touchgrp6" | sudo -S /home/grp6/477grp6/trunk/gnms_os/gnms_os &
