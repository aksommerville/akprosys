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
