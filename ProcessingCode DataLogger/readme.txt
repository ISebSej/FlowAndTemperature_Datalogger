Small "Processing" script to log serial data from the arduino to the computer.

In order to use:

	Connect an arduino which prints sensor data to a file

	Make sure the arduino is streaming to any COM (program is build for windows) and preferably has no other COMs connected

	run either the .pde file (and press play) if you have PROCESSING installed, or use the .exe in the application.windows64 folder

	Upon running a small windows opens, which should be displaying serial data

	This data is saved inside a data[timestap].txt file. If you want it to be a comma seperated values file (CSV), make sure your arduino adds commas between every serial print

	All the program does is write the raw console text to a data.txt file

	Ones you're done, press any key on the keyboard to stop to program and save the data

	NOTICE: IF YOU CLOSE THE PROGRAM WITH THE X ON THE TOP RIGHT, NO DATA WILL BE SAVED


By Besmir Sejdijaj
Powered by processing 3