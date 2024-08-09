# Vasco-WTW-CC1101
This code can control the Vasco T350 and T500 ventilation systems (heat exchanger or dutch: WTW).
Other systems from Vasco may also work but I only tested it on a T500.

You can send serial commands to the ESP which translates it into Vasco commands using a cc1101 chip.

You can log in a webpage and send the following keys:<br>
key 1 to set the ventilation in the lowest flow<br>
key 2 for the middle<br>
key 3 for the higest flow<br>
key 4 is the timer<br>
key a sends the auto mode <br>
   In this mode you can use a co2 sensor or the 0-10v input<br>
key b the auto command is send every minute<br>
   I use this because my WTW forgets the normal auto after a couple of minutes.<br>
key c disables sending the auto command <br>
key d displays if the auto sending is on or off. <br>

You need to install the cc1101 library.

<br>
Maybe next version will include a MQTT connection.


