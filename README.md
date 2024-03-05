# Garage-Door-Controller-Arduino
A simple Arduino project where you can use a generic ESP8266 board and up to 4 relays where you can control your garage port via a web GUI.

The schematic for the project is provided in the schimatic.jpg above, but Ill write down the important parts below:

Relay 1-3 uses the digital port 0-2, and the relay nr. 4 uses digital port 5. This is because there are some special ports in between port 2 and 5, like 'flash' port, ground and 3.3v ports.

