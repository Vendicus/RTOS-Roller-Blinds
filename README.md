# RTOS-Roller-Blinds
The purpose of this project is to present practical experience in designing and implementing automatic control systems, with a particular focus on using a Real-Time Operating System (RTOS). An important objective of this work was to ensure that the solution could be applied in real-world scenarios, not only as a theoretical demonstration.

For detailed information about the project, please refer to the DOC section. Below is a brief description of the system from a functional perspective. If you would like to explore the source code, please navigate to the "Automatic Roller Blinds" folder. All implemented tasks and objects are located in the lib folder.

## Why Roller blinds? 
My goal was to create something practical and genuinely useful in everyday life (you can watch a demonstration video of the roller blinds below). This project can be especially helpful for people who prefer to automate small daily routines — such as closing the blinds every evening and opening them every morning (myself included).

https://github.com/user-attachments/assets/c069ea1e-f934-4705-8a69-ac7b0845caf2

## Functionalities
There are two modes used in project, manual move mode and automatic mode. Both are essentially important. In short terms, users should be able to move roller blinds with two switches, first switch move motor up and second down. There is another switch which is an emergency type switch (to see electrical connections, see electronic diagram in DOC folder). If something wrong happens during a move or program is not responding, the user should be able to stop the machine at any time, and it should be certain to stop it. Device itself download data from servers about sunshine and sunset in the user region daily and open or close roller blinds during that time automatically. Project should save as much electrical energy as possible, so if nothing is going on, after certain time esp32 goes in light sleep mode to save power. Additionally, after cutting power off suddenly the controller enters backup mode to save encoders data in flash to remember position of motors after next power on. That solution allows return to work immediately without needing to set base position of motors after every turn on manually (to see build of backup power comparator used in project refer to electronic diagram).  

<img width="2385" height="1678" alt="Architecture of the system" src="https://github.com/user-attachments/assets/5dccf4d4-3be7-426f-aa9b-dd0b3e1f6720" />
The RTOS system Architecture in the Project.

### Manual mode
Manual mode is used when user wants to move roller blind manually by switches or to program maximal and minimal height in which motor stops during automatic phase. If the controller is in light sleep mode, pressing any button will wake up esp32 automatically. For manual mode, mostly button task is used, awakened by ISR, which sends case to main by task notification. The state machine cases of main task in numbers are described as below:
* -1- up switch pressed – move motor up
* -2- down switch pressed – move motor down
* -3- emergency button pressed – stop motor, hard stop. WARNING: After pressing that button, the red LED starts flashing. MOTORS cannot be moved anymore; user needs to restart device by turn off and on.
* -4- both up and down button pressed – save maximal or minimal position of roller blind. When the user presses 2 buttons, the system will signal it by flashing red LED diode. During certain time (5 seconds) users choose if the position of motor is max or min by pressing up or down button. If they don’t select anything, the red diode will go off, and mode will be changed to neutral again. If the user selects a maximum pos which will be below minimum pos, min pos will be reset and program will signal it with red light. The same situation will be with min pos above max position. Users should change max pos or min if that occurs.
* -5- the button unpressed – stop motors with soft stop

If manual mode was activated, the program cannot change to automatic for 5 seconds because of safety reasons. After 5 seconds of waiting for the next press, the controller will go into light sleep mode. 

### Automatic mode
Automatic mode uses mostly SSR task. That task launches the Wi-Fi module, shuts down that module in device and sends requests to the server for sunrise and sunset data. If data cannot be downloaded, then the system will try to reconnect after the next 5 seconds. If the time for sunrise or sunset has gone, then it sets timer for next day to try downloading the next data. Otherwise, the automatic mode will set a wakeup timer for the next sunset or sunrise. During Sunset or sunrise, the SSR after wakeup will send a QUEUE message for main task with number (1 - sunrise, 2 - sunset) and main will move motors, using state machine and monitoring encoders state. If the position of motors is in minimal or maximal position set by user, motor will stop and main will notify SSR task to calculate and set timer for next event. After that Esp32 will go to light sleep mode. 
