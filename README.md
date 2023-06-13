# Watchytchi
This is a work-in-progress attempt at making a tamagotchi-like virtual pet for [Watchy](https://watchy.sqfmi.com/), an open-source open-source e-ink smart watch.

I love virtual pets, and as a game developer I have always wanted to make my own. I don't have any experience with microelectronics or custom hardware, so I chose Watchy as my platform because it has buttons, a screen, and comes more or less fully assembled. With that said, this is my first experience working with Arduino and ESP32 and it is not the cleanest code at all. 

This repo contains 3 main items: an all-software prototype made in (Processing)[https://processing.org/], the Watchy project, and some paint.net asset files. The Watchy project compiles and works on device but it is a rough first draft, with some messy code and missing freatures. I have not been updating this project frequently, but I hope to get back to it! In the meantime, I hope it serves as useful reference for others.

 <div>
 <img src="./Github%20Assets/Watchytchi_Idle.jpg" alt="The pet in idle pose" width="200"/> 
 <img src="./Github%20Assets/Watchytchi_Double.jpg" alt="Two Watchys with the pet" width="400"/> 
 <img src="./Github%20Assets/Watchytchi_TiredNight.jpg" alt="A tired pet at night, lit by a lightbulb" width="200"/> 
 </div>

## Instructions:
### Navigation:
 - **The upper-left button is "Cursor,"** which moves your menu selection. **The upper right button is "Confirm,"** which you use to select options. **The lower right button is "Cancel,"** which backs out of menus and resets your cursor position. **The lower left button is "Settings,"** and opens up the various Watchy firmware options, such as time set, viration test, etc.
 - Pressing the "Cursor" button cycles your selection between the care icons on the top and bottom. A selected option is drawn in all black, while unselected options are grayed out (dithered)
 - The device vibrates when you advance your cursor or select an option
### Inspect
Selecting the magnifying glass will open up the "inspect" UI, showing your pet's hunger (left) and age (right). 
 - For hunger, 0 is the hungriest possible, and 9 is full. 
 - The age is the number of day's since your pet's "Birth." It goes up to a maximum of 9. Age currently has no effect
 - Press the "Cancel" button to close this display.
### Hunger and Feeding
 - Your pet becomes hungrier over time
 - Your pet's idle sprite will change depending on their hunger. If they are below 4 hunger, their trunk droops and they have a frazzled expression. If they are below 1 hunger, they will lie on the ground and look unhappy.
 - To notify the owner, your pet's vibration motor will buzz once when it reaches below 4 hunger, and vibrate repeatedly on an interval when it is below 1 hunger.
 - To feed your pet, move your cursor to the "refrigerator" icon and press "Confirm." A food item will spawn, and your pet will slowly chomp at it. Input is blocked during this animation. Feeding makes your pet completely full, so there is no need to feed more than once in a row.
### Time of day
 - Your pet's world responds to the real-world time of day
 - During the morning and afternoon, a sun slowly moves across the screen. This is replaced by a moon during the evening and night. Clouds drift through the sky over time
 - In the early evening, a dithered vignette will appear at the edges of the screen
 - At night, a dark vignette will cover much of the screen. A lightbulb will hang down and act as a "light source." Your pet will lay down and look sleepy. 
 - To put your pet to bed, select the "lightbulb" care icon. This turns out the lights and inverts the screen colors. Your pet will sleep peacefully.
 - The lights automatically turn back on in the daytime.
 - Thresholds for day and night are hardcoded to certain hours, and do not respond to local sunrise / sunset times (i.e. longitude and time of year)
 - While this software is not a true watch face, there is a small time display in the lower right for debugging purposes
### Idle animations
 - During the day, if your pet is not hungry, they may display idle animations
 - Every 3 minutes they perform a short animation 
 - Depending on the time of day, their default idle pose may be replaced by a "skygazing" (flipped onto their back) pose.
# Limitations
This is very much a work in progress project and has several large issues. Due to my inexperience with microelectronics and C++, some of these may take a long time to fix
 - All data is lost when the battery runs out. I don't know quite how to make data fully persistent.
 - The menu is not very responsive and it takes a long time to scroll through options
 - All player input is ignored during animations. For longer animations, or subtle animations the owner doesn't realize are playing, this can be frustrating
 - Battery life is very poor, perhaps due to updating the screen too frequently or to overusing the vibration motor
 - Most of the menu buttons don't do anything due to many features not being implemented. They appear as question marks. A "Book" icon appears in the lower right, but has no function.
 - "Watchytchi" is a mouthful, I know. And "Tamawatchy" was right there. I do not apologize.
 
# Planned features
There's a lot I would love to do with this project, although I don't know if all of it is feasible:
 - Persistent save data between power cycles
 - Optimization to increase battery life
 - Support for multiple creature species and "evolutions," similar to a traditional tamagotchi
 - Consequences for poor care and neglect (perhaps lowering lifespan, affecting evolution, and death in extreme cases)
 - The ability to read a book to your creature, and/or perform an enriching activity, which would increase a happiness meter 
 - Poop and poop cleanup functions
 - Making the pet get sick occasionally, and medicine
 - More idle animations for variety
 - A finite lifespan(?)

# Non-planned features
There are a few features which are common to many watch faces, but which I don't plan to implement:
 - I plan for this to be an unconnected device with no wi-fi or bluetooth functionality. This is both for project scope issues, lack of experience with those technologies, and a general desire for this to be a self-contained piece of software that requires little setup and won't lose functionality if APIs change or certain services go down. The only thing I might do is optionally enable time synchronization over wi-fi to help with unreliable RTC hardware.
 - I don't care very much for the "games" included in most tamagotchis, which are usually so simple as to feel like chores you try to get through as quickly as possible to raise your happy meter. I also have no intention of doing digimon-style battling
 - The "weight" stat on tamagotchis never appealed to me much

# Acknowledgements
Thank you to Devign999 for providing your excellent (dktime)[https://github.com/dezign999/dkTime] watch face as a free online resource, which served as a fantastic example of animations. In addition, thank you to everyone on the Watchy discord server who has answered my questions!

![The Watchy Creature](./Github%20Assets/WatchytchiEatingGif.gif?raw=true "Title")
