#include "QuestManager.h"
#include "Log.h"
#include "App.h"
#include "Module.h"
#include "External/PugiXml/src/pugixml.hpp"
#include "ModulePlayer.h"
#include "List.h"
#include "Input.h"
#include "Render.h"
#include "Font.h"
#include "Textures.h"
#include "SDL/include/SDL_scancode.h"

#include <string>
#include <iostream>
using namespace std;

QuestManager::QuestManager() : Module()
{
	name.Create("quests");
}

QuestManager::~QuestManager() 
{
}

bool QuestManager::Start()
{
	font = new Font("Assets/Fonts/dungeon_font3.xml", app->tex);

	pugi::xml_node questNode;
	pugi::xml_document questData;
	pugi::xml_parse_result parseResult = questData.load_file("quests.xml");
	if (parseResult == NULL)
		LOG("Could not load xml file <quests.xml> pugi error: %s", parseResult.description());
	else
		questNode = questData.child("quests");
	if (questNode == NULL)
		LOG("Could not load <questsData> xml_document");

	questNode = questNode.child("quest");
	while (questNode != NULL)
	{
		Quest* quest = new Quest();

		quest->id = questNode.attribute("id").as_int();
		quest->type = questNode.attribute("type").as_int();
		quest->title = questNode.attribute("title").as_string();
		quest->description = questNode.attribute("description").as_string();
		quest->objective = questNode.attribute("objective").as_string();
		quest->quantity = questNode.attribute("quantity").as_int();
		quest->demandingNPC = questNode.attribute("demandingNPC").as_string();
		quest->rewardingNPC = questNode.attribute("rewardingNPC").as_string();
		quest->rewardXP = questNode.attribute("rewardXP").as_int();
		quest->rewardGold = questNode.attribute("rewardGold").as_int();
		quest->requiredId = questNode.attribute("requiredId").as_int();
		quest->isCompleted = questNode.attribute("isCompleted").as_bool();
		quest->status = questNode.attribute("status").as_int();

		if (quest->status == 0) // 0 inactive but loaded
		{
			questsInactive.Add(quest);
		}
		else if (quest->status == 1) // 1 active and loaded
		{
			questsActive.Add(quest);
		}
		else if (quest->status == 2) // 2 completed
		{
			questsFinished.Add(quest);
		}

		questsList.Add(quest);

		questNode = questNode.next_sibling("quest");
	}
	return true;
}

bool QuestManager::Update(float dt)
{
	CheckQuestsLogic();
	CheckObjectivesCompletion();

	return true;
}

bool QuestManager::PostUpdate()
{
	DrawActiveQuests();
	return true;
}


bool QuestManager::CleanUp()
{
	questsList.Clear();
	questsInactive.Clear();
	questsActive.Clear();
	questsFinished.Clear();

	return true;
}



pugi::xml_node QuestManager::LoadQuestData(pugi::xml_document questData)
{
	pugi::xml_node ret;

	pugi::xml_parse_result parseResult = questData.load_file("quests.xml");
	if (parseResult == NULL)
		LOG("Could not load xml file <quests.xml> pugi error: %s", parseResult.description());
	else
		ret = questData.child("quests");

	return ret;
}

bool QuestManager::DrawActiveQuests()
{
	ListItem<Quest*>* L = questsActive.start;
	const char* titleText;
	const char* descriptionText;
	string numToStr;
	const char* numToStr2;
	while (L != NULL)
	{
		switch (L->data->id)
		{
		case 1: // Quest number 1
			// Title Drawing
			titleText = L->data->title.GetString();
			app->render->DrawText(font, titleText, 0, 60, 60, 0, { 255,255,255,255 });

			// Amount of mushrooms taken
			numToStr = to_string(app->player->mushroomCount);
			numToStr2 = numToStr.c_str();
			app->render->DrawText(font, numToStr2, 280, 63, 60, 0, { 255,255,255,200 });
			break;
		case 2: // quest chain from quest 1
			// Title Drawing
			titleText = L->data->title.GetString();
			app->render->DrawText(font, titleText, 0, 60, 60, 0, { 255,255,255,255 });

			// Amount of mushrooms taken
			numToStr = to_string(app->player->chopTreeCount);
			numToStr2 = numToStr.c_str();
			app->render->DrawText(font, numToStr2, 180, 63, 60, 0, { 255,255,255,200 });
			break;

		case 3: // new quest chain
			// Title Drawing
			titleText = L->data->title.GetString();
			app->render->DrawText(font, titleText, 0, 100, 60, 0, { 255,255,255,255 });

			// Amount of mushrooms taken
			numToStr = to_string(app->player->beachRubbish);
			numToStr2 = numToStr.c_str();
			app->render->DrawText(font, numToStr2, 225, 103, 60, 0, { 255,255,255,200 });
			break;
		case 4: // new quest chain
			// Title Drawing
			titleText = L->data->title.GetString();
			app->render->DrawText(font, titleText, 0, 100, 60, 0, { 255,255,255,255 });
			break;
		default:
			break;
		}
		
		L = L->next;
	}
	return true;
}

bool QuestManager::CheckQuestsLogic()
{
	// For changing from one list to another (has finished quest)
	ListItem<Quest*>* activeQuestsList = questsActive.start;
	while (activeQuestsList != nullptr)
	{
		if (activeQuestsList->data->isCompleted == true)
		{
			activeQuestsList->data->status = 2;
			questsActive.Del(activeQuestsList);
			questsFinished.Add(activeQuestsList->data);
			app->player->RewardXP(activeQuestsList->data->rewardXP);
			app->player->RewardGold(activeQuestsList->data->rewardGold);
		}

		activeQuestsList = activeQuestsList->next;
	}

	// Quest chain logic (if required quest is completed)
	ListItem<Quest*>* inactiveQuestsList = questsInactive.start;
	while (inactiveQuestsList != NULL)
	{
		if (inactiveQuestsList->data->requiredId != 0)
		{
			ListItem<Quest*>* L2 = questsFinished.start;
			while (L2 != NULL)
			{
				if (inactiveQuestsList->data->requiredId == L2->data->id)
				{
					questsActive.Add(inactiveQuestsList->data);
					questsInactive.Del(inactiveQuestsList);
					inactiveQuestsList->data->status = 1;
				}
				L2 = L2->next;
			}
		}

		inactiveQuestsList = inactiveQuestsList->next;
	}
	
	return true;
}

bool QuestManager::CheckObjectivesCompletion()
{
	/////////////////// Debug: Complete quest with id selected
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		int id = 1;
		CompleteQuestDebug(id);
	}
	//////////////////////////////////////////////////////////////////////

	if (app->player->mushroomCount == 8)
		CompleteQuest(1);

	if (app->player->chopTreeCount == 10)
		CompleteQuest(2);
	
	if (app->player->beachRubbish == 6)
		CompleteQuest(3);

	if (app->player->turtleKilled == true)
		CompleteQuest(4);

	return true;
}

bool QuestManager::CompleteQuest(int id)
{
	ListItem<Quest*>* L = questsList.start;
	while (L != nullptr)
	{
		if (id == L->data->id)
		{
			L->data->isCompleted = true;
		}
		L = L->next;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool QuestManager::CompleteQuestDebug(int id)
{
	ListItem<Quest*>* L = questsList.start;
	while (L != nullptr)
	{
		if (id == L->data->id)
		{
			L->data->isCompleted = true;
		}
		L = L->next;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////