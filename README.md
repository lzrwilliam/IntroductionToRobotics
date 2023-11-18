**Welcome to the my "Introduction to Robotics (2023-2024)" repository.**

As a passionate 21-year-old student at the Faculty of Mathematics and Computer Science in Bucharest and currently in the third year, I have embarked on a journey into the fascinating world of robotics. This repository serves as a storage for various projects, documentation, code implementations, and technical specifications that I've developed during my coursework.

***About Me***

I am passionated about Computer Science, especially about networking and now day by day, about robotics.  While I may not be the most enthusiastic person , I've come to appreciate the intricacies and possibilities of this domain.
This repository is a collection of projects and coursework completed during my introduction to robotics. It is a way to share my learning experience and to manage it in a productive way.


***Expectation of things to be found here***

The current repository will contain all of my work during this semester during the "Introduction to Robotics" laboratory. It will contain detailed instructions such as images, explanations of the code implementations, video recordings and technical drawings.

<details> <summary>  Homework 2 </summary>

This homework consists in having an RGB(Red-Green-Blue) Led controlled by 3 potentiometers, combining the intensity of each colour in order to obtain any possible colour. For controlling the intensity of each colour of the led, we use a separate potentiometer, so we will have 3 in total, along with 3 resistors for each channel. 
![WhatsApp Image 2023-10-23 at 00 57 36](https://github.com/lzrwilliam/IntroductionToRobotics/assets/92024459/8e7bcefb-2e53-41c1-a0d0-27277a6ed436)

[Vizionați implementarea](https://www.youtube.com/watch/faK9UB8ilXU)

</details>


<details>
  <summary>
Homework 3 - Simulating an Elevator
  </summary>
  <br> <br>

This code simulates an elevator system using an Arduino. The system can handle requests for three different floors,but floors can be added in the code very easily because we use arrays and the code is scalable. It uses LEDs to indicate the current floor of the elevator and another blinking green LED to signify the elevator is moving. The elevator waits for a floor button to be pressed, closes its "doors" (signified by a tone), moves to the requested floor after a chosen delay by us, and then opens its "doors" (indicated by another tone). When moving between floors, the led of each floor lights up for a few seconds when the elevator reaches it, no matter if it is not the destination floor.

The system also utilizes a debounce function to ensure button presses are not falsely detected due to rapid pressing or electrical interference. The debounce is implemented using the millis function.
We use a State Machine in order to control the elevator that has 4 states:  Asteptare, InchidereUsi (closing the doors before leaving the current floor), InMiscare (moving), and DeschidereUsiLaDestinatie (opening doors when the desired floor is reached). If we want we can add another states like Emergency Stop, without a lot of work and keeps the code clean and very easy to be understood.
To be mentioned, to realize this homework succesfully, the help of ChatGPT has been used, to understand better some functionalities and particularties of coding and hardware.



<br> <br> <br>

![WhatsApp Image 2023-11-03 at 00 30 57](https://github.com/lzrwilliam/IntroductionToRobotics/assets/92024459/3704d839-7fcb-4cce-ac7b-0e0bb6922176)



[Vizionati Implementarea](https://www.youtube.com/watch?v=yq_iRp4QMCM)
</details>


<details>

  <summary> Homework 4 - 7 Digit Display with a Joystick</summary>
<br> <br>
  This homework purpose is to understand the functionalities of the 7 Digit Display and  of the Joystick. We are required to move with the joystick on the 7 Digit Display, from neighbour to neighour, having provided a list of possible neighbours for each segment of the 7 Digit Display. 
  
  The start position is the DP segment and the only possible neighbour segment is on the left side and other movements like Right, Left, Up / Down, will change nothing on the display.
  
  The current position must always blink, no matter if the segment state is ON or OFF, and if we press the joystick button for a short period of time, the current segment will change its state from HIGH to LOW or from LOW to HIGH. Also, if the button is pressed longer (3-4 seconds), when released, the 7 Digit Display will reset to the default state where the DP is the current position and all the other segments are OFF. To determine the presstime, it is forbiddent to use of delay(), instead we use the millis() function.
  To be mentioned, to realize this homework succesfully, the help of ChatGPT has been used, to understand better some functionalities and particularties of coding and hardware.

<br> <br>
 ![WhatsApp Image 2023-11-02 at 14 31 23](https://github.com/lzrwilliam/IntroductionToRobotics/assets/92024459/7f1c2cd5-8e9e-4ee5-b2a3-94a3da1c7e49) 

 [Vizionati functionalitatea](https://youtube.com/watch/nMcQfggGcKI) 


  
</details>



<details>
<summary> Homework 5 - StopWatch Timer </summary>
<br> <br>

The purpose of this homework is to get familiar with working with a 4-Digit 7-Segment LED display and a Shift Register (74HC595) to have more outputs (8) in exchange of using 3 input pins. 

For this homework, we will be using 3 buttons along side with a 4-Digit 7-Segment LED display, in order to implement a stopwatch timer that counts in 10ths of a second and can save up to 4 laps, just like our phones.
The functionalities of the buttons are the following: <details> <summary> Button 1 </summary> <br>
Button 1 is for starting or pausing the counting of the time, if the reset button has not been pressed. If the reset was pressed, the Start/Pause buttons is used for descending cycling through the saved laps, and this usage of the button is available until the next press of Reset button. </details> <details><summary> Button 2 </summary> <br>
Button 2 serves as a Reset Button, but only if the timer is on pause mode and it resets the displayed time to 000.0 and the saved laps if the viewing lap mode is active (the reset button has been already pressed for the first time). </details>  <details><summary> Button 3 </summary> <br>
Button 3 purpose is to save a lap when pressed if the timer is in counting mode, or ascendent cycling through the laps if the reset button has been pressed once, having a buzzer that indicates the Save Lap button has been pressed. </details>

When we press the Start Button, the timer will start and the time will be displayed in the format 000.0, for example if it has been passing 50.4 seconds since the timer has started, the displayed value will be 050.4 seconds. When we press the Save Lap button, we save up to 4 laps, and if there are already 4 saved states, we will overwrite the existing ones, beginning from the first saved state. Also, pressing the Reset Button while the timer is counting will have no effect.

In the Pause mode, the Save Button Lap will not work, and the first pressing of Reset Button resets the timer back to 000.0 seconds, but keeps the saved laps. After that press, if we press the Save Lap Button, we will move between the saved lap cycles in ascending order, and in the descending order if we press the start/pause button, and the reset button will also now reset the lap cycles and the timer back to 000.0 seconds, restoring the initial functionality of the start/pause button. To be mentioned, to realize this homework succesfully, the help of ChatGPT has been used, to understand better some functionalities and particularties of coding and hardware.

<br> <br>
  ![WhatsApp Image 2023-11-09 at 23 46 01](https://github.com/lzrwilliam/IntroductionToRobotics/assets/92024459/317a6b70-fd71-4024-bac4-815d83574d27)

  [Vizionati functionalitatea](https://www.youtube.com/watch?v=_rE9K5HRT-I)

</details>


<details>
  <summary> Homework 6 - Smart Environment Monitor </summary>
  <br> <br>
  The purpose of this homework is to get familiar with sensors such as LDR and Ultrasonic,creating a responsive and efficiently menu and most importantly, using the EEPROM memory.
  For this homework we will be using the following components:  <details> <summary> RGB Led </summary> <br>
    This RGB Led display only Red or Green colour. The Red colour is displayed if one of our sensors are outside the set value by us from keyboard, or Green otherwise, if the RGB Led is on AUTO mode. 
    Also, if the AUTO mode is off, in the manual mode, we can enter the 'r' or 'g' to change the led color to wheather Red or Green color.
    The led colour is saved everytime, so if we restart the circuit and the auto mode is off; the RGB Led will have the last colour saved.
  </details>
  <details><summary> Light-Dependent Resistor (LDR) </summary> <br>
    Used to determine if the light in the room is lower or higher.
  </details>
   <details><summary> Ultrasonic Sensor </summary> <br>
    Used to measure the distance from an object.
  </details>

  <br> 
  We are required to implement a menu that facilitates sensor settings, data logging, system status checks, and RGB LED control. 
The Menu Features are the following:
1. Sensor Settings:
- Sensors Sampling Interval: Set a sampling rate between 1 and 10 seconds for sensors.
- Ultrasonic Alert Threshold: Define a threshold for the ultrasonic sensor to trigger alerts.
- LDR Alert Threshold: Set a threshold for the LDR sensor to signal alerts, like the onset of night.
- Back: Returns to the main menu.
  <br>
2. Reset Logger Data:
Options to confirm or deny the deletion of all sensor data, with the ability to reset data either for all sensors at once or individually.
<br>
3. System Status
- Current Sensor Readings: Continuously display readings at the set sampling rate, with an exit option.
- Current Sensor Settings: Show current sampling rate and threshold values.
- Display Logged Data: Exhibit the last 10 readings from all sensors.
- Back: Return to the main menu.
  <br>
4. RGB LED Control:
- Manual Color Control: Allows manual setting of RGB colors.
-LED: Toggle Automatic ON/OFF: In automatic mode, the LED color changes based on sensor alerts. In manual mode, it retains the last set RGB values.
- Back: Return to the main menu.
  <br>
This interactive menu system is designed to be user-friendly, providing real-time sensor data and control over the system's operations. 

<br><br>

![WhatsApp Image 2023-11-18 at 14 07 25](https://github.com/lzrwilliam/IntroductionToRobotics/assets/92024459/ab9939f7-de17-4c77-8801-a41150afecd6)

  [Vizionati functionalitatea](https://youtu.be/5CoDA14knkE)
</details>
