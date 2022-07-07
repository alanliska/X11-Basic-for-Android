# X11-Basic-for-Android

This is fork of X11-Basic project by Markus Hoffmann (https://github.com/kollokollo/X11Basic) which aims at X11-Basic executables 
working in non-terminal environment on generic (unrooted) Android within the single shell command inside of a custom app development, e.g.:
exec(getApplicationInfo().nativeLibraryDir+"/libxbbc.so -o "+getFilesDir()+"/somefile.b "+getFilesDir()+"/somefile.bas");
exec(getApplicationInfo().nativeLibraryDir+"/libxbvm.so "+getFilesDir()+"/somefile.b");
which prints the output of the interpreted code.

VARIANTS: 
(A) armeabi-v7a, arm64-v8a, i686, x86_64 ... compiled inside of Pydroid3 app (https://play.google.com/store/apps/details?id=ru.iiec.pydroid3),
working on Android 6+
(B) armeabi-v7a_legacy ... compiled inside of Terminal IDE app (https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/terminal-ide/terminalide-2.02.apk)
with the C/C++ compiler installed manually from CCTools (https://github.com/mooring/cctools-gcc-for-terminal-IDE-on-Android),
working on older arm devices with Android 4.3

DETAILED COMPILATION PROCEDURE: please see the attached file compilation-notes-[arch].txt

OTHER ATTEMPTS (unsuccessful at all or not optimal):
* Terminal IDE with the built-in C/C++ compiler
* CCTools with the built-in terminal emulator - troubles with configure
* Termux - (dynamic) executables tend to depend on other external libraries which are not available out of the Termux terminal environment
* cross compilation by Android NDK
- official release - error in configuration dialogue
- Crystax - successful with toolchain containing gcc/g++ version 5 (not 4.8 or 4.9) but all the binaries depend on libcrystax.so (even if they are forced 
manually to be linked with libcrystax.a) and some of them do not work properly in a single Android shell command

WARNING: Here published compiled binaries (without graphics support) are provided without any warranty. 

ACKNOWLEDGEMENTS: 	I thank Markus Hoffmann for numerous helpful hints and encouragement. 

LICENSES:
X11-Basic - GNU GPLv2
GMP library - GNU GPLv2 and GNU LGPLv3
For details: see the included files. 
