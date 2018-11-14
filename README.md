
<p align="center">
  <img src=https://raw.githubusercontent.com/sojoe02/RANA/master/images/ranalogo.png />
</p>

# DISCLAIMER

**Rana development has been in a bit of a hiatus, and its development branch (and branches in general) is a mess. Development is starting up again now. Rana is facing some big changes fx. Qt is going to be removed and the client part of its implementation will change codebase to something else (C# mono) and is Lua agent implementation is going to be updated
I have made so many dumb architectual descisions in my scramble to get Rana ready for my PhD that I don't really believe that Rana is anywhere near ready for prime-time**

**You can follow the development of 'new Rana' in branch future.**

**I will do bugfixes only in branch master.**


# Description

Rana is a multi agent system simulator that is designed and implemented to support the high precision needed to perform real-time multi agent communication simulations. This entails support for real-time agent actions and event propagation.

It has been designed to offer an easy approach to agent modelling via a module and API approach. The modelling paradigm allows for flexible agent design and powerful data collection as well as flexible observation of emergence. Rana is currently developed as part of my Ph.D. program that seeks to offer a new way of modelling and simulating real-time critical systems using a multi-agent systems approach. E.g currently Rana is being used to offer a generalized approach to simulations of animal chorusing.

Furthermore Rana has been used as the simulation framework in a master thesis that enabled testing of traffic-light tactics in real city sections at varying traffic loads. 

Rana can also be used to perform 'action-based' simulations and is used as courseware in a multi agent computer science course at the University of Southern Denmark (2014-2016). During the course students are tasked to design and implement mining robot- and prey/predator simulations.

An event processing tool is also featured. It allows for post processed visualization of event propagation and intensity, this allows users to observe event based emergence and determine how events has affected agent behaviour.


# Documentation

Rana functionality is currently documented via the wiki (https://github.com/sojoe02/RANA/wiki).

# Releases

Releases can be grabbed on github (https://github.com/sojoe02/RANA/releases).

# License

Rana is distributed as Open-source via.
* GNU GENERAL PUBLIC LICENSE Version 3 (http://www.gnu.org/licenses/gpl.html)

# Technologies

* Simulation core, event handler and agent interface is pure C++ (newest version requires support for C++14).
* User interface and graphic representation is written in Qt (https://www.qt.io/)
* Run-time agent design is enabled by Lua (https://www.lua.org/) with support for Just-in-time compilation via LuaJIT (http://luajit.org/).


# Devteam

 Development of the Simulation core, agent interface, UI and logo: 
 
 * Søren V. Jørgensen (sojoe02@gmail.com). Center for Bio-robotics, University of Southern Denmark.

# Input and Support

 * John Hallam
 * Yves Demazeau
 * Jakob Christensen-Dalsgaard
 * Leon Bonde Larsen
 * Thor Andreasen
 * Michael Lau Sørensen

