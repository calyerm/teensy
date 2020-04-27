# Teensy Audio Tutorials Virtual Panel
# 04/27/2020
Why : Teensy Audio Tutorials require having the HW . 2 pots and 3 buttons and breadboarding. A virtual HW
requires no harware or wiring but does require a PC or RPi , USB connection and software. 
The software simulates 2 pots and 3 buttons nneded for tutorials. There is an addition pot implemented to control
sound volume. (If you do have a pot on your Teensy audio adapter boad)
How ?  Virtual HW implemented using Python tk application and modified Teensy audio tutorial examples.
File : t4_serial.py      - Basic serial interface module , requires pyserial.
File : t4_ctrl_panel.py  - Basic virtual panel application with 3 pots and 3 buttons
File : Part_2_08_Oscillators.ino - tutorial example modfied to use virtual panel
See : https://www.pjrc.com/store/audio_tutorial_kit.html 
See : https://www.pjrc.com/teensy/td_libs_Audio.html
