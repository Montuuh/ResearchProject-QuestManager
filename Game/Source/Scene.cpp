#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "EntityManager.h"
#include "Animation.h"
#include "Font.h"
#include "QuestManager.h"
#include "ModulePlayer.h"

#include "Defs.h"
#include "Log.h"

#include <string>
#include <iostream>
using namespace std;

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->map->Load("map.tmx");
	app->map->LoadColliders();

	coinTex = app->tex->Load("Assets/Textures/spritesheet.png");
	coinRect = { 134,21,30,30 };

	font = new Font("Assets/Fonts/dungeon_font3.xml", app->tex);

	app->entities->AddEntity(EntityType::ITEM_MUSHROOM, 26 * 32 + 9, 11 * 32 + 10);
	app->entities->AddEntity(EntityType::ITEM_MUSHROOM, 25 * 32 + 9, 13 * 32 + 10);
	app->entities->AddEntity(EntityType::ITEM_MUSHROOM, 28 * 32 + 9, 13 * 32 + 10);
	app->entities->AddEntity(EntityType::ITEM_MUSHROOM, 24 * 32 + 9, 15 * 32 + 10);
	app->entities->AddEntity(EntityType::ITEM_MUSHROOM, 26 * 32 + 9, 15 * 32 + 10);
	app->entities->AddEntity(EntityType::ITEM_MUSHROOM, 25 * 32 + 9, 17 * 32 + 10);
	app->entities->AddEntity(EntityType::ITEM_MUSHROOM, 28 * 32 + 9, 17 * 32 + 10);
	app->entities->AddEntity(EntityType::ITEM_MUSHROOM, 26 * 32 + 9, 19 * 32 + 10);

	app->entities->AddEntity(EntityType::ITEM_RUBBISH, 18 * 32 + 9.5, 16 * 32 + 4);
	app->entities->AddEntity(EntityType::ITEM_RUBBISH, 13 * 32 + 9.5, 17 * 32 + 4);
	app->entities->AddEntity(EntityType::ITEM_RUBBISH, 18 * 32 + 9.5, 18 * 32 + 4);
	app->entities->AddEntity(EntityType::ITEM_RUBBISH, 15 * 32 + 9.5, 19 * 32 + 4);
	app->entities->AddEntity(EntityType::ITEM_RUBBISH, 21 * 32 + 9.5, 19 * 32 + 4);
	app->entities->AddEntity(EntityType::ITEM_RUBBISH, 18 * 32 + 9.5, 21 * 32 + 4);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->player->beachRubbish == 6 && app->player->turtleKilled == false)
	{
		app->player->beachRubbish = 0;
		app->entities->AddEntity(EntityType::TURTLE, 20 * 32, 20 * 32);
	}
	if (app->player->mushroomCount == 8 && !treeSpawn)
	{
		app->entities->AddEntity(EntityType::ITEM_TREE, 24 * 32 + 4, 8 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 25 * 32 + 4, 10 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 23 * 32 + 4, 11 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 27 * 32 + 4, 12 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 24 * 32 + 4, 13 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 27 * 32 + 4, 14 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 25 * 32 + 4, 16 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 27 * 32 + 4, 17 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 24 * 32 + 4, 18 * 32 + 1);
		app->entities->AddEntity(EntityType::ITEM_TREE, 27 * 32 + 4, 19 * 32 + 1);
		treeSpawn = true;
	}
	if (app->player->turtleKilled && app->player->chopTreeCount == 10 && !knightSnailSpawn)
	{
		app->entities->AddEntity(EntityType::ITEM_SNAIL, 8 * 32, 2 * 32);
		app->entities->AddEntity(EntityType::KNIGHT, 33 * 32, 2 * 32);
		knightSnailSpawn = true;
	}
	if (app->player->snailDelivered && !monsterSpawned)
	{
		app->entities->AddEntity(EntityType::MONSTER, 16 * 32, 28 * 32);
		monsterSpawned = true;
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	app->map->Draw();
	app->render->DrawTexture(coinTex, 3, 3, &coinRect);

	// Gold HUD
	string s = to_string(app->player->gold);
	const char* s2 = s.c_str();
	app->render->DrawText(font, s2, 54, -8, 60, 0, { 255,255,255,200 });
	
	// XP HUD
	app->render->DrawText(font, "XP", 0, 20, 60, 0, { 249,215,28,255 });
	s = to_string(app->player->xp);
	s2 = s.c_str();
	app->render->DrawText(font, s2, 54, 20, 60, 0, { 255,255,255,200 });

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->map->CleanUp();

	return true;
}
