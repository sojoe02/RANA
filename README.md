
<p align="center">
  <img src=https://raw.githubusercontent.com/sojoe02/RANA/master/images/ranalogo.png />
</p>

# DISCLAIMER *Rana as a WebService*

**This is where Rana is headed, it is nowhere near ready**

* Hook up the Rana simulation core with a service layer, Ranas UI will be replaced with a socket based interface (udp/tcp).
* UI will be build using some easy to use web based client framework such as bootstrap with a WebGL based rendering interface.
* Unity will do the graphics (webGL).
* QT will be stripped completely from the project.
* QMake is replaced with CMake.
* With time Windows support will be re-added, not so sure about Mac, though it should be easy enough(now that QT is gone)
* Main dev platform moved from qtcreator to Clion.
* Getting rid of the AgentLua interface and replace that with a lua state pr. thread. the current situation is a mess!

# Description (to be updated)

Rana is a multi agent system simulator that is designed and implemented to support the high precision needed to perform real-time multi agent communication simulations. This entails support for real-time agent actions and event propagation.

It has been designed to offer an easy approach to agent modelling via a module and API approach. The modelling paradigm allows for flexible agent design and powerful data collection as well as flexible observation of emergence. Rana is currently developed as part of my Ph.D. program that seeks to offer a new way of modelling and simulating real-time critical systems using a multi-agent systems approach. E.g currently Rana is being used to offer a generalized approach to simulations of animal chorusing.

Furthermore Rana has been used as the simulation framework in a master thesis that enabled testing of traffic-light tactics in real city sections at varying traffic loads. 

Rana can also be used to perform 'action-based' simulations and is used as courseware in a multi agent computer science course at the University of Southern Denmark (2014-2018). During the course students are tasked to design and implement mining robot- and prey/predator simulations.


# Documentation

Rana functionality is currently documented via the wiki (https://github.com/sojoe02/RANA/wiki).

# Releases

Releases can be grabbed on github (https://github.com/sojoe02/RANA/releases).

# License

Rana is distributed as Open-source via.
* GNU GENERAL PUBLIC LICENSE Version 3 (http://www.gnu.org/licenses/gpl.html)

# Technologies (to be updated)

* Simulation core, event handler and agent interface is pure C++ (newest version requires support for C++14).
* Run-time agent design is enabled by Lua (https://www.lua.org/) with support for Just-in-time compilation via LuaJIT (http://luajit.org/).


# Devteam

 Søren V. Jørgensen (sojoe02@gmail.com) **Lead**.

 Mathias Neerup

 Theis Strøm-Hansen


# Special Thanks to

 * John Hallam
 * Yves Demazeau
 * Jakob Christensen-Dalsgaard
 * Leon Bonde Larsen
 * Thor Andreasen
 * Michael Lau Sørensen
 * Michael Greenfield
 * Rama Ratnam
 * Mike Bee

