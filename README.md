# keylogger
Keylogger made in C for Windows. Also registers clicks (right, left, middle, auxiliary 1 and auxiliary 2)
Can be configured by changing some settings.

## Compilation

`tcc keylogger.c -o keylogger.exe -luser32`  
Needs `user32.dll` library to register the keypresses.

## Disclaimer

**DISCLAIMER: Only use this for educational purposes, this hasn't been created by no means to harm people nor to use maliciously. USE IT AT YOUR OWN RISK!**  
This program has been created ONLY with the purpose to learn WinAPI's keyboard and mouse functions.
