Protocol
========

This document describes the protocol to be used to communicate with the motor board.

Communication occurs over a serial connection, at a baud rate of 1,000,000. ASCII characters represent commands, except where backwards compatibility is required.

Commands
--------

Lower case characters affect output 0; upper case characters affect output 1.

`s` or `S`
:	**Set power:** set the power of the output to the next byte sent.

	Note that if the next byte sent is a null character (`\0`, 0x00), the command will be cancelled and the power will remain unchanged. To set an output's power to 0, disable the output with the Change mode command.

`m` or `M`
:	**Choose mode:** set the mode of the output will be set depending on the following character. There are the following modes:

	* `f`: Forward
	* `r`: Reverse
	* `h`: Halt (brake)
	* `d`: Disable

`\0` (0x00)
:	**Cancel:** cancel the previous command and wait for a new one. For example, sending `S\0` will leave the power of the output the same, as the `\0` cancels the set command.

`\1` (0x01)
:	**Print version:** send the firmware version string.

`\4` (0x04)
:	**Enter bootloader:** reset the motor board and enter bootloader mode, to allow the board to be reflashed.

Examples
--------

### Output 0 forwards, output 1 backwards ###

	sASAmfMf

* Set output 0 to 65
* Set output 1 to 65
* Set mode of output 0 to forward
* Set mode of output 1 to forward

### Output 0 freewheel, output 1 brake ###

	mdMh

* Set mode of output 0 to disable
* Set mode of output 1 to halt

### Output 0 brake lightly, output 1 brake gently ###

	sAScmhMh

* Set output 0 to 65
* Set output 1 to 100
* Set mode of output 0 to halt
* Set mode of output 1 to halt
