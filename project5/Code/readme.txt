Below are the functions that play the biggest part in making the program run, the full
description for all functions can be found with comments in the respective file

main.cpp
--------
Lines 51-158: Implemented an overarching try-catch block to run everything in,
             rather than using multiple smaller blocks
             
functions.hpp
-------------
NONE

functions.cpp
-------------

Line 36: calculateCycleTime

	      calculate the metadata metrics by "mapping" metadata descriptors to their
	      corresponding components

Line 82: calculateSleepTime

         calculate the value for sleep / wait function

Line 180: getComponentCycleTimes

          gets the various component cycle times from the configuration file
          
Line 215: calculateTime

          calculate the metadata metrics by "mapping" metadata descriptors to their
          corresponding components
          
Line 356: handleErrors

          handles all errors given the error code by displaying a corresponding
          message and terminates the program after logging
          
Line 372: printTime

	       output duration between two points for timestamp

Line 390: priorityScheduling

	       sets up vector compliant with the priority scheduling algorithm

Line 508: processAndLog

          logs all data to the appropriate location in the prescribed format,
          changes PCB process state, and deals with multithreading and semaphore locks

Line 892: readConfigurationFile

          uses modular functions to read the entire configuration file

Line 995: readMetadataFile

          uses a modular function to read the entire metadata file
          
Line 1012: roundRobin

           sets up vector compliant with the round robin algorithm

Line 1021: shortestJobFirst

           sets up vector compliant with the shortest job first scheduling algorithm
           
Line 1191: shortestTimeRemaining

           sets up vector compliant with the shortest time remining scheduling algorithm
