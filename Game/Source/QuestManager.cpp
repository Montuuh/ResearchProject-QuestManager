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
		case 2:
			// Title Drawing
			titleText = L->data->title.GetString();
			app->render->DrawText(font, titleText, 0, 60, 60, 0, { 255,255,255,255 });

			// Amount of mushrooms taken
			numToStr = to_string(app->player->mushroomCount);
			numToStr2 = numToStr.c_str();
			app->render->DrawText(font, numToStr2, 280, 63, 60, 0, { 255,255,255,200 });
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
	ListItem<Quest*>* L4 = questsActive.start;
	while (L4 != nullptr)
	{
		if (L4->data->isCompleted == true)
		{
			L4->data->status = 2;
			//L->append_attribute("status") = 2?
			//L->append_attribute("isCompleted") = true?
			questsActive.Del(L4);
			questsFinished.Add(L4->data);
			app->player->RewardXP(L4->data->rewardXP);
			app->player->RewardGold(L4->data->rewardGold);
		}

		L4 = L4->next;
	}

	// Quest chain logic (if required quest is completed)
	ListItem<Quest*>* L = questsInactive.start;
	while (L != NULL)
	{
		if (L->data->requiredId != 0)
		{
			ListItem<Quest*>* L2 = questsFinished.start;
			while (L2 != NULL)
			{
				if (L->data->requiredId == L2->data->id)
				{
					questsActive.Add(L->data);
					questsInactive.Del(L);
					//L->append_attribute("status") = 1?
					L->data->status = 1;
				}
				L2 = L2->next;
			}
		}

		L = L->next;
	}
	
	return true;
}

bool QuestManager::CheckObjectivesCompletion()
{
	/////////////////// Debug: Complete quest with id 2
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		int id = 1;
		CompleteQuestDebug(id);
	}
	//////////////////////////////////////////////////////////////////////

	if (app->player->mushroomCount == 8)
		CompleteQuest(1);
	
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