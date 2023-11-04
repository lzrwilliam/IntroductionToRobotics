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



<br> <br> <br>

![WhatsApp Image 2023-11-03 at 00 30 57](https://github.com/lzrwilliam/IntroductionToRobotics/assets/92024459/3704d839-7fcb-4cce-ac7b-0e0bb6922176)



[Vizionati Implementarea](https://www.youtube.com/watch?v=yqiRp4QMCM)
</details>


<details>

  <summary> Homework 4 - 7 Digit Display with a Joystick</summary>
<br> <br>
  This homework purpose is to understand the functionalities of the 7 Digit Display and  of the Joystick. We are required to move with the joystick on the 7 Digit Display, from neighbour to neighour, having provided a list of possible neighbours for each segment of the 7 Digit Display. 
  
  The start position is the DP segment and the only possible neighbour segment is on the left side and other movements like Right, Left, Up / Down, will change nothing on the display.
  
  The current position must always blink, no matter if the segment state is ON or OFF, and if we press the joystick button for a short period of time, the current segment will change its state from HIGH to LOW or from LOW to HIGH. Also, if the button is pressed longer (3-4 seconds), when released, the 7 Digit Display will reset to the default state where the DP is the current position and all the other segments are OFF. To determine the presstime, it is forbiddent to use of delay(), instead we use the millis() function.

<br> <br>
 ![WhatsApp Image 2023-11-02 at 14 31 23](https://github.com/lzrwilliam/IntroductionToRobotics/assets/92024459/7f1c2cd5-8e9e-4ee5-b2a3-94a3da1c7e49) 

 [Vizionati functionalitatea](https://youtube.com/watch/nMcQfggGcKI) 


  
</details>
