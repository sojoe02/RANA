# Mac compilation notes

##Lua

###Linking Lua
Add static linking to Lua library, by right-clicking in the .pro file. Choose **"Add External Library"** and browse to the \*.a file.
Change the inclue directory to the files with the source files.

###Add dynamic library
In order to prevent RANA from chrashing on launch due to a missing liblua52.dylip file do the following:
Goto Projects, in the side panel and follaw:
 Run > Run Environment > Add, then add **DYLD\_LIBRARY\_PATH**, and set it to the path of the liblua52.dylib file. 

You need to do this event if Lua is statically linked.

# Linux Compilation notes

For linux you have to have at least gcc version 4.8 installed, to properly allow for C11 support.

If you use older Qt versions, it might not be able to find *#include<QtWidgets>* in the MainWindow.cpp. That line can then be replaced by *#include<Qwidget>*

##Ubuntu


###Version 14.04 LTS

To compile RANA using Ubuntu do the following:

1. Install Ubuntu.
2. Install QtCreator, using the Software Center.
3. Install lua v5.2, via **"apt-get install lua5.2"**.
4. Install liblua5.2dev, via **"apt-get install lua5.2-dev"**.
5. Install git.
6. Execute **"git clone https://github.com/sojoe02/RANA\_QT.git"** in a wanted parent directory.
7. Checkout whatever branch you want, e.g. **"git checkout MAS2014"**.
8. Change the line **"unix: PKGCONFIG += lua"** to **"unix: PKGCONFIG += lua5.2"**.
