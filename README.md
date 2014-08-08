
#Description

RANA is a multi agent system simulator. It is designed and implemented to support the high precision needed to perform agent communication simulations. However it can also be used to perform simpler 'action-based' simulations.

A simplified system diagram can be seen in figure. The user interface and control layer is programmed using QT, while the simulation core and utility layer is programmed in pure C11 C++. The agent interface is written using the Lua C++ api. This ensures that RANA can be compiled on Linux, MacOS and MS based platforms.

I am currently hard at work on either implementing timewarp or live updates of event propagation so events arrive at the correct time even if the agents are moving.


#C++ API

The simulation core offers a number of functions that the agent can call to expand its functionality. The API make is possible for the agent to communicate with both the simulation core an utility classes, so it can manipulate the map, do physics calculations, etc.

##Api functions


|function		|Arguments	|Description		|
|:-----------------------|:-------------|:----------------------|
|**l_speedOfSound** 	|myX, myY, origX, origY, propspeed| Calculates the microstep, it takes for something that to propagate from origX,origY to myX,myY with the speed of propspeed *m/s* |
|**l_currentTime**	||				Returns the current microstep|
|**l_distance**		|myX, myY, origX, origY| Calculates the amount of units between myX,myY and origX,origY|
|**l_getMacroFactor**	||				Returns the macrofactor of the simulator|
|**l_getTimeResolution**	||Returns 1/microresolution |
|**l_getMersenneFloat**	|float1, float2	| Returns a 64bit float between [float1,float2[|
|**l_getMersenneInteger**|int1, int2	| Returns a number between int1 and int2, 64bit|
|**l_EnvironmentSize** 	||Returns width and height of the environment(starts at 0)|
|**l_modifyMap**	|x, y, R, G, B|	Changes the color of the map, at x,y |
|**l_checkMap**		|x, y|		Returns R,G,B value of position x,y on the map (256,256,256) if the map is out of bounds|
|**l_checkPosition**	|x, y| Returns a list of the ID's of the agents at position x,y|
|**l_checkCollision**	|x, y| Returns a boolean that is true if an agent is occupying x,y|
|**l_updatePosition**	|oldX, oldY, newX, newY, ID| updates the agents position from oldX,oldY to newX,newY, in order for **l_checkPosition** and **l_checkCollision** to work the agents have use this whenever they change position|
|**l_stopSimulation**	|| Tells the simulation core to stop the simulator when the next macroStep is done|

It is important to note that movement and map manipulation is not part of the simulation core itself, but rather the physics engine for very good reasons... email me if you want to know more.

#Compilation

RANA_QT has two depencies, Qt and Lua (version 5.2).

##Windows

For Windows one can use MinGW from http://www.mingw.org/. 


##MacOS

You need the Xcode framework for compilation and debugging.

###Lua

You can download Lua binarys from the Lua mainpage, http://www.lua.org. or compile it yourself.

####Linking Lua
Add static linking to Lua library, by right-clicking in the .pro file. Choose **"Add External Library"** and browse to the \*.a file.
Change the inclue directory to the files with the source files.

####Add dynamic library
In order to prevent RANA from chrashing on launch due to a missing liblua52.dylip file do the following:
Goto Projects, in the side panel and follaw:
 Run > Run Environment > Add, then add **DYLD\_LIBRARY\_PATH**, and set it to the path of the liblua52.dylib file. 

You need to do this event if Lua is statically linked.

##Linux

For linux you have to have at least gcc version 4.8 installed, to properly allow for C11 support.

If you use older Qt versions, it might not be able to find *#include \<QtWidgets\>* in the MainWindow.cpp. That line can then be replaced by *#include \<Qwidget\>*

###Arch

RANA was developed on using Arch linux, so the provided .pro file should be set up fine, provided you have lua and pkgconfig installed.

###Ubuntu

####Version 14.04 LTS

To compile RANA using Ubuntu do the following:

1. Install Ubuntu.
2. Install QtCreator, using the Software Center.
3. Install lua v5.2, via **"apt-get install lua5.2"**.
4. Install liblua5.2dev, via **"apt-get install lua5.2-dev"**.
5. Install git.
6. Execute **"git clone https://github.com/sojoe02/RANA_QT.git"** in a wanted parent directory.
7. Checkout whatever branch you want, e.g. **"git checkout MAS2014"**.
8. Change the line **"unix: PKGCONFIG += lua"** to **"unix: PKGCONFIG += lua5.2"**.

#License

GNU GENERAL PUBLIC LICENSE Version 3
http://www.gnu.org/licenses/gpl.html

#Devteam

 Development of the Simulation core, agent interface and UI: Søren V Jørgensen, sojoe02@gmail.com

 Example Agents are developed by ..... \<need to ...\>
