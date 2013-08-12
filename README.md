Tekkodu = [Tekkotsu](http://www.tekkotsu.org/) + [Kodu](https://research.microsoft.com/en-us/projects/kodu/)
=======================================

## Introduction
Microsoft's [Kodu Game Lab](https://research.microsoft.com/en-us/projects/kodu/) is a programming environment designed for young children. Kodu programmers create computer games by writing behavioral rules for animated characters. This research develops strategies for mapping Kodu's idealized perception and manipulation primitives onto real robots, which have significant physical constraints. Out target platform is the [Calliope2SP](http://www.roprodesign.com/educational-robots/calliope/) robot.

## Problem with Implementing Kodu's Primitives on a Real Robot
In Kodu's virtual environment, perception and manipulation are instantaneous, omnidirectional, and never fail. On a real robot, vision is slow, expensive, and limited to where the camera is pointed. Also, a robot can only manipulate an object within the gripper's reach, and because gripping is unreliable, the gripper may lose the object while the robot is moving. For these reasons we need strategies to reliably implement Kodu's primitives on a real robot such as the Calliope2SP.

## How to Install on Ubuntu
### I.		Install Tekkotsu on a computer running Ubuntu Linux.
Follow all instructions in the links below:
[Ubuntu install instructions](http://wiki.tekkotsu.org/index.php/Ubuntu_install_instructions)
[Tekkotsu install instructions](http://wiki.tekkotsu.org/index.php/Tekkotsu_install_instructions)
[Mirage install instructions](http://wiki.tekkotsu.org/index.php/Mirage_install_instructions)

### II.		Download Tekkodu Project
Download the Tekkodu project code by [clicking me](https://github.com/troiwilliams/tekkodu/archive/master.zip).

### III.	Compile the Tekkodu source code with Tekkotsu
Move the project's zip file into the ~/project folder and execute the following commands in Terminal:

$ cd ~/project
$ unzip tekkodu-master.zip
$ mv tekkodu-master/*.cc.fsm .
$ mv tekkodu-master/my.kode .
$ mv tekkodu-master Kodu
$ make

### IV.		Run Tekkotsu
If you do have a physical Tekkotsu-compatible robot, or prefer to run Tekkotsu on in a virtual environment, do the following:
1) Open three Terminal tabs (two more if you already one Terminal window open from running the previous commands)
2) In tab #1, run:

$ ControllerGUI localhost

3) In tab #2, run:

$ Mirage tictactoe.mirage

4) Click and hold the box to the right of the "Full Screen" option, and select "No"
5) Click and hold the box to the right of the "Video Mode" option, and select "640 x 480"
6) Click the "Accept" button. NOTE: You should only have to do steps 4 and 5 once.

7) In tab #3, run:

$ ControllerGUI localhost

8) In tab #1, run:

$ ./tekkotsu-CALLIOPE2SP -c mirage.plist

Voila, you're now running Tekkotsu in Mirage!

### V.		Run the Kodu Game
In the Menu box on the left side of the Tekkotsu GUI, double-click on "User Behaviors" > "KoduGame"; the text "KoduGame" will turn red if it was activated. Now, watch the magic happen as your Kodu program runs on the virtual robot!
To stop the Kodu program, double-click on "KoduGame"; the text "KoduGame" will turn black if it was deactivated/stopped.

## Wiki and full tutorial coming soon!