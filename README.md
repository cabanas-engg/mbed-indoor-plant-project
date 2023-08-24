# Plant Care Device

Built with Mbed microcontroller and software to monitor and automate plant care.

This is a project I put together in a short amount of time, therefore it's far from polished, but I plan to update it to PCB components and possibly a different microcontroller for better performance and fitting.
I also plan to update this project to use a web server interface to display plant information instead of an LCD

# Parts List:
-	mbed LPC14768
-	16x2 LCD
-	12V Power Supply
-	4 Terminal Relay
-	DHT11 Temperature and Humidity Sensor
-	12V Solenoid Valve
-	12V Fan
-	Grow Lights
-	Button
-	Photo-resistor
-	9V Battery
-	Soil Moisture Sensor 

# Description
The MBED microcontroller maintains a healthy environment for the plant, using sensors for light, temperature, and moisture. Normally running is the LCD displaying important info on the plant’s vitals, then, at a specific rate of time set by the user, the microcontroller’s software interrupts this to check on the intensity of light (Photo-resistor), moisture level (Moisture Sensor), and temperature (DHT11 Sensor). When one of these is unstable then other operations follow to keep the plant alive. For water: a solenoid valve connected to a container filled with water opens if the soil is dry; For light: a grow light LED strip turns on by either a user input (button) or by the photo-resistor value itself; For temp: a heating element turns on below a normal temperature and a fan turns on both below and over the stable temperature.  
