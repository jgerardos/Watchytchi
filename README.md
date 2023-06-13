# Watchytchi
This is a work-in-progress attempt at making a tamagotchi-like "watch face" for [Watchy](https://watchy.sqfmi.com/), an open-source open-source e-ink smart watch.

I love virtual pets and own many Tamagotchis, as well as several non-tamagotchi "oddpets." As a software developer I have always wanted to make my own but have never had the microelectronics skills and knowledge to create my own. Watchy was an appealing platform due to coming fully assembled with buttons, a vibration motor, and a beautiful e-ink screen. With that said, this is my first experience working with Arduino and ESP32 and I have struggled with the overall development flow. 

This repo contains 3 main items: a protoype made in Processing that I worked on before my Watchy came, the Watchy project, and some paint.net files in which I have been making assets. The Watchy project compiles and works on device, although it is still a very rough first draft. I am not updating this project frequently, but I hope to work more on it in the future. In the meantime, I hope it serves as useful reference for others.

## Instructions:
### Navigation:
 - **The upper-left button is "Cursor,"** which moves your menu selection. **The upper right button is "Confirm,"** which you use to select options. **The lower right button is "Cancel,"** which backs out of menus and resets your cursor position. **The lower left button is "Settings,"** and opens up the various watchy firmware options, such as time set, viration test, etc.
 - Pressing the "Cursor" button cycles your selection between the various care options on the top and bottom, similar to a real tamagotchi. The selected option is drawn in all black, while unselected options are grayed out (dithered)
 - You will feel the vibration motor buzz when you advance your cursor or select an option
### Inspect
Selecting the magnifying glass will open up the "inspect" UI, showing your pet's hunger (left) and age (right). 
 - For hunger, 0 is the hungriest possible, 9 is fully full. 
 - The age is the number of day's since your pet's "Birth." It goes up to a maximum of 9. There is currently no effect from increasing in age.
 - Press the "Cancel" button to close this display.
### Hunger and Feeding
 - Your pet becomes hungrier over time
 - Your pet's idle sprite will change depending on their hunger. If they are below 4 hunger, their trunk droops and they have a frazzled expression. If they are below 1 hunger, they will lie on the ground and look unhappy.
 - To notify the owner, your pet's vibration motor will buzz once when it reaches below 4 hunger, and vibrate repeatedly on an interval when it is below 1 hunger.
 - To feed your pet, move your cursor to the "refrigerator" icon and press "Confirm." A food item will spawn, and your pet will slowly chomp at it. Feeding makes your pet completely full, so there is no need to feed more than once in a row.
### Time of day
 - Your pet's world responds to the real-world time of day
 - During the morning and afternoon, a sun moves across the screen indicating the hour. During the evening and night, the sun is replaced by a moon. 
 - Clouds move in the sky over time
 - In the early evening, a dithered vignette will appear at the edges of the screen to show nightfall
 - At night a dark vignette will cover much of the screen. A lightbulb will hang down and act as a "light source." Your pet will lay down and look sleepy. 
 - To put your pet to bed, select the "lightbulb" option from the care icons. This will turn out the lights, causing screen colors to invert. Your creature will sleep peacefully.
 - The lights will automatically turn back on in the daytime.
 - Thresholds for day and night are hardcoded to certain hour thresholds, and do not respond to local sunrise / sunset times (i.e. longitude and time of year)
 - While this software is not meant to be a watch, there is a small time display in the lower right for debugging purposes
### Idle animations
 - During the day, if your pet is not hungry, they may display idle animations
 - Every 3 minutes, they will perform a short animation 
 - Depending on the time of day, their default idle pose may be replaced by a "skygazing" (flipped onto their back) pose. 

# Limitations
This is very much a work in progress project and has several large issues. Due to my inexperience with the platform, some of these may take a long time to fix.
- All data is lost when the battery runs out. I don't know quite how to make data fully persistent.
 - All player input is ignored during animations. For longer animations, or subtle animations the owner doesn't realize are playing, this can be frustrating
 - Battery life is very poor, perhaps due to updating the screen too frequently or to overusing the vibration motor
 - The menu is not very responsive and it takes a long time to scroll through options
 - Most of the menu buttons don't do anything due to many features not being implemented. They appear as question marks. A "Book" icon appears in the lower right, but has no function.
 
# Planned features
There's a lot I would love to do with this project, although I don't know if all of it is feasible:
 - Persistent save data between power cycles
 - Support for multiple creature species and "evolutions," similar to a traditional tamagotchi
 - Consequences for poor care and neglect (perhaps lowering lifespan or affecting evolution)
 - Poop and poop cleanup functions
 - Making the pet get sick occasionally, and medicine
 - A finite lifespan(?)
 - The ability to read to your creature, and/or perform an enriching activity, which would increase a happiness meter 

# Non-planned features
There are a few features which are common to many watch faces, but which I don't plan to implement:
 - I plan for this to be an unconnected device with no wi-fi or bluetooth functionality. This is both for project scope issues, lack of experience with those technologies, and a general desire for this to be a self-contained piece of software that requires little setup and won't lose functionality if APIs change or certain services go down.
 - I don't care very much for the "games" included in most tamagotchis, which are usually so simple as to feel like chores you try to get through as quickly as possible to raise your happy meter. I also have no intention of doing digimon-style battling
 - The "weight" stat on tamagotchis never appealed to me much

# Acknowledgements
Thank you to Devign999 for providing your excellent (dktime)[https://github.com/dezign999/dkTime] watch face as a free online resource, which served as a fantastic example of animations. In addition, thank you to everyone on the Watchy discord server who has answered my questions!
