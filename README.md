# DCF77 Uhr
Stand 26.12.2020

Ein Programm um mit einem Arduino-Nano-Clone eine Funkuhr über DCF77 zu betreiben. Es ist auch ein handgezeichneter Schaltplan beigefügt.

Folgende Bibliotheken werden benötigt:
DCF77.h
Time.h
LiquidCrystal.h
Sie sind beigefüht.

Es gilt folgende Anschlussbelegung.
Pin 2: DCF77-Signal
Pin 3; rs-Signal LCD-Display
Pin 4: en-Signal LCD-Display
Pin 5: d4-Signal LCD-Display
Pin 6: d5-Signal LCD-Display
Pin 7: d6-Signal LCD-Display
Pin 8: d7-Signal LCD-Display
Ein Schaltplan ist beigefügt.

Das Signal ist invertiert. Die Invertierung kann mit diesem Befehl verändert werden:
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT, false);
false: steht für ein invertiertes-Signal.
true: steht für ein normales Dignal.

Es gibt einen Sketch um das Signal zu testen (sketch_dec24a.ino). Hier sollten im SeriellenMonitr immer 800ms, 900ms, 100ms, und 200ms stehen. Ansonsten stimmt was mit dem Signal nicht. Danach kann das eigentliche Programm sketch_dec24b.ino eingespielt werden.


Folgende Einstellungen sind in der Rduino Entwicklungsumgebung notwendig für den Arduino NAno CLone:
Arduino Nano Clone (CH340)
Board: Arduino Nano
Prozessor: ATmega 328P (old Bootloader)
Programmer: AVRISP mkII
