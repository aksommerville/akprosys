## Prosystem Emulator Version 1.3
The ProSystem Emulator is an Atari 7800 emulator for the PC and Windows OS. The emulator was written in C++ using the Windows API and DirectX. It emulates the Atari 7800 NTSC and PAL TV standards. The emulator was originally written by Greg Stanton and then updated by others.

See the project web site for more details:
http://gstanton.github.io/ProSystem1_3/

## MODIFIED

By AK Sommerville, beginning 2023-06-14.
Planning to drop the Windows code, arrange to build with make, onboard my 'emuhost' platform layer.

## TODO

- [ ] Can we emulate the zapper? eg barnyard.a78
- [ ] Faulty ROMs? Or some feature we're not emulating?
- - bsktbrwl.a78: black screen
- - F14.bin: black screen
- - F18.A78: freeze after mission briefing
- - fatal_run: black screen after intro splash
- - fight_night.a78: render problem. some tiles missing from the boxers
- - ikariwar.a78: Similar to fight_night, sprites missing
- - rescue.a78: Weird render problems, big orange box. no audio
- - xenophobe.a78: Intro splash appears but unresponsive
