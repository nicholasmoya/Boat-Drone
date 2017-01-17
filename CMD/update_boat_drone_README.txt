INSTRUCTIONS TO RESET WAYPOINTS LAT/LON VALUES



Set up/Reset
-------------------------------------------------------------------------
1. In Set_Waypoints.vbs, change arduinoFilePath so it matches the path
	of the arduino file. Look in Set_Waypoints for an example.

2. Likewise, change waypointsFilePath so that it too matches the path
	of the text file that has the waypoints in it.

3. Make sure that the lat/lon values in the waypoints.txt file are
	written in the following way:



number_of_waypoints
{latValue1, lonValue1}, {latValue2, lonValue2}, ... {latValuen, lonValuen}



(see the waypoints.txt file for an example)

4. Replace the old actual lat and lon values in the arduino code with the
	placeholders. Ex:



const double waypoint[4][2] = {{33.34361936, -81.74101174},
                               {33.34399486, -81.7403841},
                               {33.34445751, -81.73954993},   
                               {33.3439711, -81.73915431}}; 



becomes:



const double waypoint[numOfWay][2] = {waypoints};



(see reset_placeholders.txt for an example)

5. Change the directory in update_boat_drone.cmd to match the directory
	of the Set_Waypoints.vbs file.

6. Change the path in update_boat_drone.cmd to match the path
	of the arduino sketch file. 



Action
-------------------------------------------------------------------------

Double click the update_boat_drone.cmd batch file and you should see two
	messages saying that the waypoints have been successfully reset
	and that the boat drone sketch has been uploaded.