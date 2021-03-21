# Presentation & Research Project Description
Hello, my name is David Montufo, [Montuuh](https://github.com/Montuuh) on github. I'm a student, now at the second year, at the Bachelor's degree in Video Game Design and Development in CITM (UPC - Barcelona). This is a personal research project about a basic Quest Manager, under the lecture of Ramón Santamaría, [raysan5](https://github.com/raysan5) on github.

Before starting to show you raw code, I firstly want to show you some basic concept ideas of quests, or maybe some questions or doubts that you may also have. From my point of view, I think this process is crucial, not only for implementing a Quest Manager, but any type of code. You should have the ideas clear in order to advance properly and fast.

# Concept Discoveries about quests, missions and code structure:
First of all, I want to comment you my experience doing this research. At the firsts days I started coding this research, I nearly didn't look to any external info in the internet, so I got really lost, and got stuck at the beggining. It was due to this fact that I went back, and make a brainstorming of what are quests, why should they be present in an RPG game, etc. Nowadays, having quests is a key element in videogames, it's such so that for some games, quests are a game pillar for the narrative of it. Thus, wanting to have a well implemented Quest Manager, let's start brainstorming some ideas.

What are quest, and why is it not called as missions. Well, there is a tight difference between them, but generally a mission comes into action when a long process or storyline will be up to the player (GTA or Assassin's Creed as examples), whereas having multiple and rewardful quests that the player chooses which to do (World of Warcraft as example). Having known that, I preferred calling our System a Questie one, as I will be working on an RPG that requires quests rather than missions.

After knowing that we will be using quests as a base, I researched on the common quest types that are used in the majority of videogames. I could differentiate 6 of them:
 - Gather Quest: Known as collection or fetch quests, requires a character to collect X number of items. They can either be gathered from the environment, or they require to kill creatures in order to collect them. It can exist the possibility to gather different Y items in the same quest.
 - Kill Quest: These quests send the character out to kill X number of creatures, or just an specific one. Commonly, these quests require the character to bring back proof of their success, such as trophies, heads, fur, etc. (I thought that these quest may also be gathering ones if all of them must have a proof to take back to the NPC)
 - Delivery Quest: These quests involve the character being sent to deliver a package from one location to another. Maybe sometimes, the player will have to look for the item himself rather than being given by the demanding NPC. These quests challenge the player to go through dangerous or unexplored terrain, and maybe adding a time limit. (I also thought that these quests can also be summarised a gather quest).
 - Dialogue Quest: These quests are really simple and not commonly well rewarded. They are just done to make the player go through the different parts of the game. If you are given this quest, you know that where you complete it,  you will have a quest zone to do.
 - Escort Quest: These quests involve protecting an NPC while it walks completely randomly, but most commonly on a path / route. From my experience, the majority of time, that NPC will have a 0 iq AI and it will walk all through the enemies basement, and the player will have to handle multiple enemies, struggling while your loved NPC is just walking around, even pulling more and more enemies. As told, a not well-implement AI can make these quests sometimes impossible to complete for the player, as the NPC can behave unexpectedly or in unmanageable ways.
 - Hybrid Quest: Some of these quests can be combined to make more complex ones. For example, a quest will require a character to find several items in the world (gathering quest), assemble an specific gun and killing creatures with that weapon (killing quest). It can also involve puzzles and riddles.


# Complete implementation


# Quest System Testing by a small game environment


# Handout for your own practice


# Possible Improvements and better solutions


# Code Style and Conventions
Here it is a list with some of the conventions used in this research:

Code element | Convention | Example
--- | :---: | ---
Defines | ALL_CAPS | `#define PLATFORM_DESKTOP`
Macros | ALL_CAPS | `#define MIN(a,b) (((a)<(b))?(a):(b))`
Variables | lowerCase | `int screenWidth = 0;`
Local variables | lowerCase | `Vector2 playerPosition;`
Global variables | lowerCase | `bool fullscreen = false;`
Constants | lowerCase | `const int maxValue = 8`
Pointers | MyType* pointer | `Texture2D* array;`
float values | always x.xf | `float value = 10.0f`
Operators | value1 * value2 | `int product = value * 6;`
Operators | value1 / value2 | `int division = value / 4;`
Operators | value1 + value2 | `int sum = value + 10;`
Operators | value1 - value2 | `int res = value - 5;`
Enum | TitleCase | `enum TextureFormat`
Enum members | ALL_CAPS | `UNCOMPRESSED_R8G8B8`
Struct | TitleCase | `struct Texture2D`
Struct members |lowerCase | `texture.id`
Functions | TitleCase | `InitWindow()`
Class | TitleCase | `class Player`
Class Fields | lowerCase | `Vector2 position`
Class Methods | TitleCase | `GetPosition()`
Ternary Operator | (condition)? result1 : result2 | `printf("Value is 0: %s", (value == 0)? "yes" : "no");`

 - Research uses aligned braces or curly brackets:
```c
void SomeFunction()
{
   // TODO: Do something here!
}
```

 - Research uses **TABS** instead of 4 spaces.

 - Trailing spaces MUST be avoided!

## Files and Directories Naming Conventions
 
 - Directories will be named using `TitleCase` : `Assets/Audio/Music`
  
 - Files will be named using `snake_case`: `main_title.png`
  
_NOTE: Avoid any space or special character in the files/dir naming!_

### Directories Organization:
  
Files should be organized by context and usage in the game, think about the loading requirements and put all the assets that need to be loaded at the same time together.

Here an example, note that some assets require to be loaded all at once while other assets require to be loaded only at initialization (gui, font).
  
```
Assets/Audio/Fx/long_jump.wav
Assets/Audio/Music/main_theme.ogg
Assets/Screens/Logo/logo.png
Assets/Screens/Title/title.png
Assets/Screens/Gameplay/background.png
Assets/Characters/player.png
Assets/Characters/enemy_slime.png
Assets/Common/font_arial.ttf
Assets/GUI/gui.png
```

# Webgraphy
## Interesting web-pages about quests:
[Wikipedia: Quests](https://en.wikipedia.org/wiki/Quest_(video_games))

[GameDev Forum: Quests vs. missions](https://www.gamedev.net/forums/topic/692182-missions-vs-quests/5356654/)

## Interesting videos about Quest System:
[Video 1](https://youtu.be/c9IPvBdJoiE)

[Video 2](https://youtu.be/k2zC9pg6Eno)


# Help & Support (delete when delivery, only for me as Markdown)

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```
***
 - **TODO:** Use this to personal ToDo's
***

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

