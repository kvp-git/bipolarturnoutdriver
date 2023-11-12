# Bipolar turnout driver

 Bipolar pulsed coil turnout driver for the Adafruit motor shield v1 with route selection buttons.
 Usable for Tomix and Kato bipolar pulsed coil turnouts.
 Written by Viktor Peter Kovacs (KVP) in 2023.

## Setup

 Use a 12V, 2A DC power source for the turnouts. Wire turnouts 1 to 4 to motor outputs M1 to M4.
 Connect route selection buttons between A0-A5 input pins and gnd. (May need to add pins to bottom right side A0-5 and Gnd connectors.)
 Always disconnect the power bridge jumper before the USB programming cable is connected (or program with the external power source disconnected)!

## Configuration

 Default route table is set up for a 5 track ladder with 4 turnouts on motor outputs M1 to M4 and 5 active buttons on A0-4.
 Route selection buttons are on the first 6 analog input pins, pull to gnd for selection.
 May connect multiple drivers and/or buttons together by connecting 12V, gnd and the button inputs in parallel.
