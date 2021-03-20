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
