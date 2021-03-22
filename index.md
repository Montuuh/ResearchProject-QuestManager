# Presentation & Research Project Description
Hello, my name is David Montufo, [Montuuh](https://github.com/Montuuh) on github. I'm a student, now at the second year, at the Bachelor's degree in Video Game Design and Development in CITM (UPC - Barcelona). This is a personal research project about a basic Quest Manager, under the lecture of Ramón Santamaría, [raysan5](https://github.com/raysan5) on github.

Before starting to show you raw code, I firstly want to show you some basic concept ideas of quests, or maybe some questions or doubts that you may also have. From my point of view, I think this process is crucial, not only for implementing a Quest Manager, but any type of code. You should have the ideas clear in order to advance properly and fast.

# Concept Discoveries about quests, missions and code structure:
## Quests vs Missions
![Image of quests](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/quests2.png?raw=true)

First of all, I want to comment you my experience doing this research. At the firsts days I started coding this research, I nearly didn't look to any external info in the internet, so I got really lost, and got stuck at the beggining. It was due to this fact that I went back, and make a brainstorming of what are quests, why should they be present in an RPG game, etc. Nowadays, having quests is a key element in videogames, it's such so that for some games, quests are a game pillar for the narrative of it. Thus, wanting to have a well implemented Quest Manager, let's start brainstorming some ideas.

What are quest, and why is it not called as missions. Well, there is a tight difference between them, but generally a mission comes into action when a long process or storyline will be up to the player (GTA or Assassin's Creed as examples), whereas having multiple and rewardful quests that the player chooses which to do (World of Warcraft as example). Having known that, I preferred calling our System a Questie one, as I will be working on an RPG that requires quests rather than missions.

## Quests types
After knowing that we will be using quests as a base, I researched on the common quest types that are used in the majority of videogames. I could differentiate 6 of them:
 - **Gather Quest:** Known as collection or fetch quests, requires a character to collect X number of items. They can either be gathered from the environment, or they require to kill creatures in order to collect them. It can exist the possibility to gather different Y items in the same quest.
 - **Kill Quest:** These quests send the character out to kill X number of creatures, or just an specific one. Commonly, these quests require the character to bring back proof of their success, such as trophies, heads, fur, etc. (I thought that these quest may also be gathering ones if all of them must have a proof to take back to the NPC)
 - **Delivery Quest:** These quests involve the character being sent to deliver a package from one location to another. Maybe sometimes, the player will have to look for the item himself rather than being given by the demanding NPC. These quests challenge the player to go through dangerous or unexplored terrain, and maybe adding a time limit. (I also thought that these quests can also be summarised a gather quest).
 - **Dialogue Quest:** These quests are really simple and not commonly well rewarded. They are just done to make the player go through the different parts of the game. If you are given this quest, you know that where you complete it,  you will have a quest zone to do.
 - **Escort Quest:** These quests involve protecting an NPC while it walks completely randomly, but most commonly on a path / route. From my experience, the majority of time, that NPC will have a 0 iq AI and it will walk all through the enemies basement, and the player will have to handle multiple enemies, struggling while your loved NPC is just walking around, even pulling more and more enemies. As told, a not well-implement AI can make these quests sometimes impossible to complete for the player, as the NPC can behave unexpectedly or in unmanageable ways.
 - **Hybrid Quest:** Some of these quests can be combined to make more complex ones. For example, a quest will require a character to find several items in the world (gathering quest), assemble an specific gun and killing creatures with that weapon (killing quest). It can also involve puzzles and riddles.
![Quests Types](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/questsTypes2.png?raw=true)

## Questions & Brainstorming
![Exclamation Quest](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/exclamationQuests2.png?raw=true)

When I thought our quests would be I found some of the following questions:
 - Will there be a dialogue before and after each quest?
 - Will always a quest be given by an NPC, or entering a room, or just picking X item will trigger it?
 - Will I have to implement a template for each quest type?
 - Will I have pre-defined quests, or random ones?
 - Can a quest have multiple endings (player choice)?
 - How complew the logic we want to achieve will be? Or is it rather simple? How many different systems will involve (Dialogues, cutscenes, battles, NPCs...)?
 - There are multiple ways to complete a quest, or is it rather lineal?
 - Can we make complex quests by combining simple ones?


## Quest System structure
For structuring the Quest Manager I found two options, one simpler than another:

1. Having an object, in this case a Quest Manager, that contains the complete data and logic for all the quests. That's the simplest way to be implemented a Quest System, but the less recommended when your game is running lots of quests. This is the one I implemented and I will deliver to you.
2. Another idea I had was to have a parent quest object, and each quest will be a child of that class, that inherits generic logic from it. These childs will have the data for just 1 quest, and logic taken from the parent class plus additional unique logic.

![QuestManager Structure](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/conceptDataStructure2.png?raw=true)

# Complete implementation
## Data Structure 
For storing all the quest data, I used an .xml document. In there we will save some variables like:
![xml data structure](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/dataStructure.png?raw=true)

I do know, by now we may not need some of that unnecessary variables, but may in the future take more importance, so I decided to keep it like that. After that, we are loading it to the code, storing this data inside. It is easily done with pugi libraries and lists:
![Loading Xml Document](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/loadingXmlDocument.png?raw=true)

Now that we have the document loaded in the code, we just simply have to save each variable with its corresponding value, and after that, sorting out the lists activeQuests, inactiveQuests and finishedQuests, depending on the status number:
![Loading Xml Variables](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/loadingXmlDocument2.png?raw=true)

## Quest Manager Logic
For the QUest Manager logic, there's a function called at the QuestManager::Update(), named CheckQuestsLogic, which has a simple function by now. It's objective is to change correctly from one list to another if some requisites are completed. For example it has to change automatically a quest from the inactive to the active list if there exists a quest ID in the finished list, that matches a required ID in the inactive list. Another logic that this function controls is when completed a quest (boolean isCompleted == true), it manages to give the corresponding rewards, delete the quest from the active list and move it to the finished one:
![Quest Manager Logic 2](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/Quest%20Manager%20Logic%202.png?raw=true)
![ Ques Manager Logic 1](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/Quest%20Manager%20Logic.png?raw=true)

I found a problem in the chain quest logic. If a new quest needs as requisites, two different quests, as the system I provided just have one requiredID variable, it cannot be done automatically, it has to be a little bit of hardcoded. I will now show you how I did it:
![Quest Manager Logic 3](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/Quest%20Manager%20Logic%203.png?raw=true)

## Quest Drawing in the HUD
For a quest to be drawn in the HUD, it just has to be in the active list. The main problem is that I didn't know how to  print the list itself, independently from each quest or size. I hardcoded every position and size for each of the quests in order to put them in the correct position. I will show you how I did it, for the first quest only:
![Drawing Quests](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/Drawing%20Quests.png?raw=true)

And this is an image of the in-game HUD, showing the description of the quest (user pressing L key):
![Drawing Quests](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/Drawing%20Quests%20Example.png?raw=true)

## Completing quests
For completing quests, I had to make two different functions. One will put the quest boolean isCompleted to true, being given a quest Id (CompleteQuest). Whereas the other one will just check if the conditions of a quest completed are fulfilled, and if done, call the other function, passing that exact quest ID (CheckObjectivesCompletion).

![Complete Quest](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/Complete%20Quest%201.png?raw=true)
![Complete QUest 2](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/Complete%20Quest%202.png?raw=true)

# Quest System Testing by a small game environment
In order to test correctly the Quest Manager, I managed to make a really small game. 

The game consists of a player that has to complete some quests that are shown at the left of the screen. It has some other HUD features like Gold and XP, in order to also test the rewards that these quests give. The game has 3 different quest chains, with 2 quests each. 2 of these quest chains are available and shown at the start of the environment, whereas the third one will be active when both chainquests are completed.

All entities are managed through and entity system, with its corresponding colliders. Your interaction key is always the SPACE. Your movement keys are WASD. If you get stuck press R to return to the spawn. To show up quests descriptio press L. And for debugging, pressing F1 will complete quests id by id, starting for the first one.
![Game environment](https://github.com/Montuuh/ResearchProject-QuestManager/blob/master/docs/images/Game%20environment.png?raw=true)


# Handout for your own practice
I’m delivering to you a folder containing the game where you can play the .exe directly, a handout and a solution folder. It’s recommendable that you read and try the proposed ToDo’s in order to finish the understanding of the Quest manager. If you get stuck, don’t hesitate on asking me or look at your doubt in the solution. If you have any improvement proposal, I’m open to hear it, as it will expand my current knowledge. So let’s start explaining a bit the ToDo’s:
1. **ToDo 1:** Easy one, you will just have to fill the .xml file with the blanks I left. Go to Output folder in the Handout and open quests.xml. Remember that the id is unique for each quest, the type depends on the quest type, and the status depends whether it should start active or inactive if its a chain quest.
2. **ToDo 2:** You will have to load the xml document into the code. You will have to parse the result and LOG parse description if any problem occured. Finally you will have to point the xml_node to the document child called quests.
3. **ToDo 3:** You will have to fill each list with its corresponding quests. Remember, if status == 0 the quest is not active yet, if status == 1 the quest is active, and if status == 2 the quest is finished.
4. **Todo 4:** You will have to draw the description for each quest in screen when the user presses the L keybinding. I have given you the try and error position, size, spacing and tint of each code, so you won’t have to waste time hardcoding it.
5. **ToDo 5:** When a quest is completed the variable isCompleted is set to true. So make a piece of code that reads the active quests list and looks for a quest that has the boolean isCompleted to true. If done, delete that quest from the active list, add it to the finished list, and change its status to 2, which means finished. Remember to give the quest reward by calling two functions of the ModulePlayer.
6. **ToDo 6:** For the chain quest logic, you will have to look in the complete list of quests for that ones that have an id requirement (requiredId != 0). If it finds one, a new list of finished quests must be searched for and id that meets the requiredId. If done, delete the first quest from the inactive list, move it to the active one, and change the status to 1.
7. **ToDo 7:** For the success of the quest completion logic, it needs a function that given an id, look for that quest in the whole list of quests that matches ids, and then it sets the boolean isCompleted to true.
8. **ToDo 8:** Now remember or look back the objectives of each quest. On the function CheckObjectivesCompletion, you will have to call the function CompleteQuest for each one, having passed the condition. All conditions are managed and stored in the ModulePlayer, so you will have to take a look to the .h document in order to get the names.

For you to download the folders, go to [Release folder of the repository](https://github.com/Montuuh/ResearchProject-QuestManager/releases/tag/1.0).

# Possible Improvements and better solutions
 - **Quest Chains?**: I am pretty sure that quest chains requiring more quests than 1 can be implemented automatically, but I have not got enought time right now to implement it correctly, so if you want to improve it, feel free to give me your opinions and ideas.
 - **Quest HUD?**: By this possible improvement I am absolutely sure that I will find another way just to draw an entire list wihout hardcoding every position. But the same, if you get fresh ideas about how to do it correctly, don't hesitate on answering me.
 - **Save / Load?**: As I ran out of time, and I found Saving and Loading quests not a huge priority, I preferred not to implement it. But it is pretty easy to imlement it if you know a little bit of pugi and you handle correctly all the variables. On a close future, I will do it and upload it to a new release for you to have it.

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
