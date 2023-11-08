## Prosystem Emulator Version 1.3
The ProSystem Emulator is an Atari 7800 emulator for the PC and Windows OS.
The emulator was written in C++ using the Windows API and DirectX.
It emulates the Atari 7800 NTSC and PAL TV standards.
The emulator was originally written by Greg Stanton and then updated by others.

See the project web site for more details:
http://gstanton.github.io/ProSystem1_3/

## MODIFIED

By AK Sommerville, beginning 2023-06-14.
Planning to drop the Windows code, arrange to build with make, onboard my 'emuhost' platform layer.

## TODO

- [ ] Can we emulate the zapper? eg barnyard.a78
- [ ] Faulty ROMs? Or some feature we're not emulating?
- -     F14.bin: black screen
- -     F18.A78: freeze after mission briefing
- -     rescue.a78: Weird render problems, big orange box. no audio
- -     kung-fu-master.a78: Works, but colors all screwy
- -     mia.a78: Works, but sprites flicker. The first bad guy gets two different images, can't be right.
- -     Pitfight.a78: black screen
- -     double-dragon.a78: black screen
- -     rampage.a78: black screen

F14.bin: maybe just missing the 128-byte header?
00000000  01 41 54 41 52 49 37 38  30 30 20 20 20 20 20 20  |.ATARI7800      |
00000010  20 54 55 42 45 53 20 32  30 30 34 2d 31 30 2d 33  | TUBES 2004-10-3|
00000020  31 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |1...............|
00000030  00 00 00 40 00 00 00 01  01 00 ff ff ff ff ff ff  |...@............|
00000040  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  |................|
00000050  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  |................|
00000060  ff ff ff ff 41 43 54 55  41 4c 20 43 41 52 54 20  |....ACTUAL CART |
00000070  44 41 54 41 20 53 54 41  52 54 53 20 48 45 52 45  |DATA STARTS HERE|

