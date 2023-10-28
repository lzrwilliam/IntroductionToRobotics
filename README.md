**Welcome to the my "Introduction to Robotics (2023-2024)" repository.**

As a passionate 21-year-old student at the Faculty of Mathematics and Computer Science in Bucharest and currently in the third year, I have embarked on a journey into the fascinating world of robotics. This repository serves as a storage for various projects, documentation, code implementations, and technical specifications that I've developed during my coursework.

***About Me***

I am passionated about Computer Science, especially about networking and now day by day, about robotics.  While I may not be the most enthusiastic person , I've come to appreciate the intricacies and possibilities of this domain.
This repository is a collection of projects and coursework completed during my introduction to robotics. It is a way to share my learning experience and to manage it in a productive way.


***Expectation of things to be found here***

The current repository will contain all of my work during this semester during the "Introduction to Robotics" laboratory. It will contain detailed instructions such as images, explanations of the code implementations, video recordings and technical drawings.

**Homework 2**

This homework consists in having an RGB(Red-Green-Blue) Led controlled by 3 potentiometers, combining the intensity of each colour in order to obtain any possible colour. For controlling the intensity of each colour of the led, we use a separate potentiometer, so we will have 3 in total, along with 3 resistors for each channel. 
![WhatsApp Image 2023-10-23 at 00 57 36](https://github.com/lzrwilliam/IntroductionToRobotics/assets/92024459/8e7bcefb-2e53-41c1-a0d0-27277a6ed436)

[Vizionați implementarea](https://www.youtube.com/shorts/faK9UB8ilXU)





**Homework 3**


This code simulates an elevator system using an Arduino. The system can handle requests for three different floors,but floors can be added in the code very easily because we use arrays and the code is scalable. It uses LEDs to indicate the current floor of the elevator and another blinking LED to signify the elevator is moving. The elevator waits for a floor button to be pressed, closes its "doors" (signified by a tone), moves to the requested floor after a chosen delay by us, and then opens its "doors" (indicated by another tone). The system also utilizes a debounce function to ensure button presses are not falsely detected due to rapid pressing or electrical interference.
We use a State Machine in order to control the elevator that has 4 states:  Asteptare, InchidereUsi (closing the doors before leaving the current floor), InMiscare (moving), and DeschidereUsiLaDestinatie (opening doors when the desired floor is reached). If we want we can add another states like Emergency Stop, without a lot of work and keeps the code clean and very easy to be understood.

[Vizionati Implementarea Liftului]  https://www.youtube.com/watch?v=gr_ndrQBNs8
