SFML Setup 
Open your browser and go to the SFML official download page:
Scroll to the Download SFML section.
After downloading the .zip file, extract it anywhere on your computer.
Go to your game project folder.
Their is a folder named exclude.
Inside the exclude folder, create another folder named SFML.
Copy the entire contents of the extracted SFML folder (this includes folders like bin, include, lib, and so on) into the exclude/SFML folder.



Game Project Documentation
Title: Chicken Invaders – Space Shooter
Platform: Windows
Language: C++
Library Used: SFML (Simple and Fast Multimedia Library)
IDE: Visual Studio

1. Introduction
This project is a 2D space shooter game inspired by the classic Chicken Invaders series. The player controls a spaceship, navigates through waves of enemy invaders (chickens, monsters, etc.), and must survive by shooting them down. The game features sound effects, score tracking, lives, level progression, and animated enemy movement.

2. Objective
The player must eliminate as many enemies as possible while avoiding enemy fire and collisions. The game ends when the player loses all lives or completes all levels.

3. Features
Spaceship controlled by keyboard inputs

Multiple enemy types with unique movement patterns

Bullet firing in multiple directions

Level-based enemy progression

Score and lives tracking system

Collision detection and explosion effects

Background music and sound effects

Shake effect on hit

4. Game Controls
Action	Key
Move Left	Left Arrow
Move Right	Right Arrow
Move Up	Up Arrow
Move Down	Down Arrow

5. Project Structure
pgsql
Copy
Edit
Chicken_Invaders_Space_Shooter/
├── exclude/
│   └── SFML/          → Contains SFML library (include, lib, bin)
├── g1/
│   ├── main.cpp       → Game loop and initialization
│   ├── player.h       → Player ship logic
│   ├── enemy.h        → Enemy classes and logic
│   ├── level.h        → Level system
│   ├── game.h         → Core game logic
│   ├── menu.h         → Main menu and UI
│   ├── lives.h        → Life tracking system
├── Game.sln           → Visual Studio solution file
└── README.md          → Project overview and setup instructions
6. Assets
Images:
Spaceship, background, enemies (chicken, monster, boss), explosions

Sounds:
Shooting, explosion, background music, life lost

Fonts:
Used for displaying score, menus, and labels
