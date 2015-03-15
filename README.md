
#Description

RANA is a multi agent system simulator. It is designed and implemented to support the high precision needed to perform agent communication simulations. However it can also be used to perform simpler 'action-based' simulations.

A simplified system diagram can be seen in figure. The user interface and control layer is programmed using QT, while the simulation core and utility layer is programmed in pure C11 C++. The agent interface is written using the Lua C++ api. This ensures that RANA can be compiled on Linux, MacOS and MS based platforms.

I am currently hard at work on either implementing timewarp or live updates of event propagation so events arrive at the correct time even if the agents are moving.

##Precision Levels

RANA handles agent actions at two different precision levels, 'macro' and 'micro'.

The macro level is a user defined multiple of the micro level. The micro-level is the precision with which the agent can perceive and react to events emitted by other agents. As the simulator is designed to perform 'real-time' simulations. A microlevel of 1x10<sup>6</sup> enables the agent to perceive events with one millionth second precision. 

The macro-level is where all major actions are usually performed. Actions such as movement and initiation of events.

##Events

Any external action that the agent can perform, is called an event. Events are basically containers that hold all the relevant information required for the simulation core and the agents to interpret them correctly.  Event attributes and their description is listed below.

|Attribute	|Type		|Description |
|:-----------|:------------|:------------|
|id| 	Unsigned Integer |Event identifier, a runtime unique integer ID is provided via the simulation core.|
|propagationSpeed| Unsigned Integer|The speed the event propagates at in meters pr. second(m/s). Other agents will not perceive/receive the event until it has propagated to their position. PropagationSpeed of 0 is equal to instant propagation|
|posX | Unsigned Float|The X coordinate the event originates from.|:
|posY | Unsigned Float|The Y coordinate the event originates from.|
|table | String |A serialized Lua table that can hold event specific data. |
|desc | String | A descriptor string that can help agents identify event types.|
|targetID | Unsigned Integer | Events can either be broadcast or targeted. If the targetID is 0 the event will be broadcast to all agents in the simulation.|

#C++ API

The simulation core offers a number of functions that the agent can call to expand its functionality. The API make is possible for the agent to communicate with both the simulation core an utility classes, so it can manipulate the map, do physics calculations, etc.

##Api functions

It is important to note that movement and map manipulation is not part of the simulation core itself, but rather the physics engine for very good reasons... email me if you want to know more.

If an agent takes up more than one x,y space, you can add more than one position via l_updatePosition by providing the extra coordinates as both oldX,oldY and newX,newY, just remember to update all the positions correctly upon movement.



###Output:

|Function		|Arguments	|Description		|
|:-----------------------|:-------------|:----------------------|
|l_print		|string |Prints an html formatted string to simulation output.|
|l_debug 	|string| Same as l_print, though the output of this can be disabled in the menu|

###Map:
|:-----------------------|:-------------|:----------------------|
|l_modifyMap	|x, y, R, G, B|	Changes the color of the map, at x,y |
|l_checkMap		|x, y|		Returns R,G,B value of position x,y on the map (256,256,256) if the map is out of bounds|

###Shared Values:
|:-----------------------|:-------------|:----------------------|
|l_addSharedNumber	|key, number| adds any type of 'number' to a shared hash-map, indexed by 'key' of type string|
|l_getSharedNumber	|key| returns the value associated with key, if the key does not exist it returns "no_value"|
|l_addSharedString	|key, string| adds a string to a shared hashmap, with 'key' of type string. E.g. this can be used to store serialized tables|
|l_getSharedNumber	|key| returns the string associated with the 'key', if the key does not exist it returns "no_value"|

###Utility:
|:-----------------------|:-------------|:----------------------|
|l_speedOfSound	|myX, myY, origX, origY, propspeed| Calculates the arrival microstep, for something that propagates from origX,origY to myX,myY with the speed of propspeed *m/s*. |
|l_distance	|myX, myY, origX, origY| Calculates the amount of units between myX,myY and origX,origY|
|l_getMersenneFloat	|float1, float2	| Returns a 64bit float between [float1,float2[|
|l_getMersenneInteger|uint1, uint2	| Returns a 64 signed integer between [uint1, uint2]|
|l_getEnvironmentSize	||Returns width and height of the environment(starts at 0)|


###Simulation Variables:
|:-----------------------|:-------------|:----------------------|
|l_currentTime	||				Returns the current microstep.|
|l_getMacroFactor	||				Returns the macrofactor of the simulator|
|l_getTimeResolution	||Returns the microresolution |
|l_getEnvironmentSize	||Returns width and height of the environment(starts at 0)|
|l_updatePosition	|oldX, oldY, newX, newY, ID| Updates the agents position from oldX,oldY to newX,newY, in order for l_checkPosition and l_checkCollision to work the agents have use this whenever they change position|

###Agent Manipulation:
|l_checkPosition	|x, y| Returns a list of the ID's of the agents at position x,y|
|l_checkCollision	|x, y| Returns a boolean that is true if an agent is occupying x,y|
|l_stopSimulation	|| Tells the simulation core to stop the simulator when the next macroStep is done|
|l_getAgentPath			|| returns two strings, the path of the agent(no filename) and the filename of the main lua agent file|
|l_addAgent |x,y,z,path,filename | adds a new auton at a given x,y and z position using the path and filename given, the simulation will stop with a warning if the agent source cannot be found. Returns the ID of the new agent|
|l_removeAgent | id | removes an agent with the given ID. Returns true or false whether it's successful|
 
#Event Processing

RANA has an event processing module. 

Events can upon the completion of a simulation be saved. They can later be loaded into the event post-processing module and be processed and drawn in 2 dimensional color map.

To enable event processing the agent must implement the event processing function, see the ICTAI agents in the lua\_agents library for examples.

The user defines the following parameters.

* Map Resolution. Defines the granularity of the event colormap in relation to the simulated map.
* Z thresshold. Events are processed via a recursive function, that starts from origin where it calculates an 'intensity' or z value. It then expands to all neighbouring cell either until it reaches the end of the map, or the calculated z value goes below the defined thresshold. This value is a percentage of the original.
* Time resolution. Defines how many seconds each color map will cover.
* To and From. Defines the timeperiod the event processing should cover.
* Event description. This is a regular expression, the user can use this to only process event whose description matches the givent regular expression.

The event table can contain a duration index, this will tell the event processing the duration of the event in seconds. If duration is not defined the duration will be 0, which is 1 time frame regardless of time resolution.

The map frames can either be browsed manually or played back with a user defined speed for a 'live' view.

#Compilation

RANA_QT has two depencies, Qt and Lua (version 5.2).

Pre-compiled Lua libraries for Windows and MacOS can be downloaded from here http://www.lua.org. For Linux it is recommended to utilize the target distributions integrates Lua development packages.

It is highly recommended to compile as 64bit as RANA does operate with 64bit numbers. Also event heavy simulations can use more memory than is available in 32bit userspace (usually less than 2Gb).

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

 Input and Support an various subjects regarding RANA: \<needs to be expanded\>

 * John Hallam
 * Yves Demazeau
 * Jakob Christensen-Dalsgaard

 Example Agents are developed by ..... \<need get more examples ...\>
