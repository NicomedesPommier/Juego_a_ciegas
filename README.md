# Blind Scape
The following repository showcases a game console designed for blind individuals. This project was created in collaboration with Benjamin Loubies. It originated during a class called Interface Design. The concept emerged after successfully utilizing a Wii controller in Rhino 7 to control the movement and construction of a small procedural building. If interested in this project, please contact me.

<p align="center">
  <img width="600" height="300" src=images\rhino_wii.jpg >
</p>
Subsequently, we decided to develop a game console and design a game based on Rhino Grasshopper. Eventually, we selected a blind user to provide us with restrictions on how to construct the interface. Due to the limited time available for development and testing of the console, we were unable to have blind individuals evaluate the ergonomics of our design.

The final console included an escape room game, a Bluetooth controller, and the main console. Initially, we aimed to create a handheld console, but our professors encouraged us to develop separate objects to enhance the shared experience.
 unforunatley we lost a lot of images and videos.
## Table of Contents
- [Introduction](#Introduction)
- [Renders](#Renders)
- [Build](#Build)
- [Schematics](#Schematics)
- [comments](#comments)

## Introduction
Blind Escape represents the inaugural game on our custom console. It plunges players into an immersive escape room adventure where darkness shrouds every corner. With sight rendered useless, players must rely on both auditory and tactile senses to navigate their surroundings. Amidst the darkness, distinct noises and tactile cues serve as guiding beacons, leading players towards their means of escape. To break free from their confines, they must unlock a safe and decipher a Morse code to obtain the keys that unlock their liberty.





## Renders
### First poster
<p align="center">
  <img width="500" height="600" src=images\Lamin1.JPG >
</p>
<p align="center">
  <img width="500" height="500" src=images\render1.JPG >
</p>
<p align="center">
  <img width=500" height="500" src=images\render2.JPG >
</p>

## Build

| Material List|
|:-----:|
|Arduino nano|
|esp 32|
|6 buttons| 
|1 joystick|
|1 potenciometer|
|DFPlayer |
|vibrating motor|
|Speaker|
|SD card|

###  First screen test

<p align="center">
  <img width="300" height="500" src=images\first_screen.jpg >
</p>


### first prototype
<p align="center">
  <img width="400" height="500" src=images\buil_11.jpg>
</p>

<p align="center">
  <img width="400" height="500" src=images\prototipe1_1.jpg>
</p>

## coments
This project proved to be as challenging as it was enjoyable. Throughout its development, we gained valuable insights into ergonomic design, Bluetooth connectivity, and audio management. However, during the coding phase, we encountered a significant obstacle: a bug in the audio component. Despite our efforts to debug it, we ultimately had to remove the audio part entirely before the demo, as it would only play the first audio track. Fortunately, the game remained functional, as audible signals were provided by a buzzer. The project was especially challenging due to the tight deadline of one month, which coincided with a heavy workload of classes that semester. Balancing our academic responsibilities with the demands of the project added to the difficulty. Despite this, we persevered. While we both had prior experience with Arduino coding, tackling a project of this magnitude was uncharted territory for us.