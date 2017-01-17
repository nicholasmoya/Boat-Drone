@echo off

rem - Nicholas Moya
rem - Boat Drone Prototype
rem - Dr. Tad Whiteside
rem - 7/15/2016
rem
rem
rem - update_boat_drone.cmd
rem
rem - This code allows the user to simply double click this .cmd file and the
rem - waypoint lat/lon values from the waypoints.txt file will be written
rem - into the arduino boat drone sketch AND the sketch will be uploaded.


rem - use lat/lon values from waypoints text file to set lat/lon values for
rem 		waypoints in arduino file
cd C:\Users\N8691\Documents\visual_basic
cscript Set_Waypoints.vbs

rem - upload arduino sketch with newly reset waypoints
cd C:\Program Files (x86)\Arduino
arduino.exe --upload C:\Users\N8691\Documents\Arduino\arduino_demo\arduino_demo.ino

echo.
echo           Boat Drone code have been successfully uploaded.          
echo.

PAUSE
