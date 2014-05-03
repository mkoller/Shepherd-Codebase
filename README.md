Shepherd-Codebase
=================

The shepherd band codebase

To setup your machine to work on the firmware follow this link exactly:
https://github.com/spark/core-firmware Except for cloning the git repositories.

Then navigate into C:\Spark\ with git bash
Then enter this command:
git init
Then sync up with our github directories:
git remote add origin https://github.com/mkoller/Shepherd-Codebase
git pull origin master

Now you have all the code for the firmware and the python TCP server

If you want to change firmware, make your changes
then open cmd, navigate to C:\Spark\core-firmware\build\ and enter
this command into the shell:
make

Then connect the device to your PC via USB, put the device in DFU mode
mode, and enter this command to flash the device:
dfu-util -d 1d50:607f -a 0 -s 0x08005000:leave -D core-firmware.bin

To commit your changes to our code-base:
git push origin master
