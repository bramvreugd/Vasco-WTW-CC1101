# Vasco-WTW-CC1101
This code can control the Vasco T350 and T500 ventilation systems.
Other systems from vasco may also work but I only tested it on a T500.

You can send serial commands to the ESP which translates it into Vasco commands using a cc1101 chip.

send key 1 to set the ventilation in the lowest flow
key 2 for the middle and 
key 3 for the higest flow
key 4 is the timer
key a sends the auto mode which then uses a co2 sensor or the 0-10v input

key b the auto command is send every minute
key c disables sending the auto command
key d displays if the auto sending is on or off.

You need to install the cc1101 library.


