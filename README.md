# switch-brainfuck
A brainfuck interpreter for the Nintendo Switch

## Usage
Put a .bf file with code (only code, no comments) in either the root of your emulator's SD card or in the same folder as your .nro. It will be booted on start.

When it asks for input move the left joystick to select a number.

## Compiling 
Just install devkitPro using [this](http://switchbrew.org/index.php?title=Setting_up_Development_Environment) guide, replace the source folder in your new project with the source folder here, and run **make build** in the same folder as the Makefile.
