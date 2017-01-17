' ######################################################################
' Nicholas Moya
' SRNL - Boat Drone Prototype
' Dr. Tad Whiteside
' 7/15/2016
'
'
'
' Set_Waypoints.vbs
'
' This code opens a text file containing the number of waypoints and
' the waypoints, stores those values temporarily and then opens the
' arduino file and replaces the placeholders with the real values
' and closes it. Please see the README for details on how to use this
' code and write in the waypoints.txt file.
' ######################################################################



Const arduinoFilePath = "C:\Users\N8691\Documents\Arduino\arduino_demo\arduino_demo.ino"
Const waypointsFilePath = "C:\Users\N8691\Documents\waypoints.txt"
Const ForReading = 1
Const ForWriting = 2
Const numOfWaypoints_placeholder = "numOfWay"
Const waypoints_placeholder = "waypoints"
Dim numOfWaypoints_value
Dim waypoints_value



' #################################################################################
' OPEN TEXT FILE, READ IN NUMBER OF WAYPOINTS AND WAYPOINTS, STORE THEM, CLOSE FILE
' #################################################################################

' creates text file as object and opens it
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set waypointsFile = objFSO.OpenTextFile(waypointsFilePath,1)

' read in and store number of waypoints
numOfWaypoints_value = waypointsFile.ReadLine()

' read in and store waypoints
waypoints_value = waypointsFile.ReadLine()

' close file
waypointsFile.Close
Set waypointsFile = Nothing



' ############################################################################################################
' OPEN ARDUINO FILE, REPLACE PLACEHOLDER FOR NUMBER OF WAYPOINTS WITH THE REAL NUMBER OF WAYPOINTS, CLOSE FILE
' ############################################################################################################

' sets text file as object, opens it
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile(arduinoFilePath, ForReading)

' read all lines from old file, close it
strText = objFile.ReadAll
objFile.Close

' replace the numOfWaypoints_placeholder with the numOfWaypoints_value
numOfWaypoints_value = Replace(strText, numOfWaypoints_placeholder, numOfWaypoints_value)

' open text file, writes content to it, closes it
Set objFile = objFSO.OpenTextFile(arduinoFilePath, ForWriting)
objFile.WriteLine numOfWaypoints_value
objFile.Close



' ########################################################################################
' OPEN ARDUINO FILE, REPLACE PLACEHOLDER FOR WAYPOINTS WITH THE REAL WAYPOINTS, CLOSE FILE
' ########################################################################################

' sets text file as object, opens it
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile(arduinoFilePath, ForReading)

' read all lines from old file, close it
strText = objFile.ReadAll
objFile.Close

' replace the waypoints_placeholder with the waypoints_value
waypoints_value = Replace(strText, waypoints_placeholder, waypoints_value)

' open text file, writes content to it, closes it
Set objFile = objFSO.OpenTextFile(arduinoFilePath, ForWriting)
objFile.WriteLine waypoints_value
objFile.Close



Wscript.Echo "          Waypoints have been successfully reset.          "
