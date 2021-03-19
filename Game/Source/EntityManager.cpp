#include "EntityManager.h"

#include "App.h"

#include "Render.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "List.h"
#include "ModuleCollisions.h"
#include "Entity.h"

#include "ItemHealth.h"
//#include "ItemDiamond.h"


#define SPAWN_MARGIN 50
#define MAX_ENTITIES 20

Entities::Entities(bool startEnabled) : Module()
{
	name.Create("entity");

}

Entities::~Entities()
{

}

bool Entities::Start()
{

	spritesheetTexture = app->tex->Load("Assets/Textures/spritesheet.png");

	return true;
}


bool Entities::PreUpdate()
{
	bool ret = true;

	ListItem<Entity*>* list;
	for (list = entities.start; list != NULL && list->data != nullptr; list = list->next)
	{
		if (list != NULL && list->data->pendingToDelete)
		{
			entities.Del(list);
		}
	}
	return ret;
}

bool Entities::Update(float dt)
{
	bool ret = true;
	HandleEnemiesSpawn();
	ListItem<Entity*>* list;
	for (list = entities.start; list != NULL && list->data != nullptr; list = list->next)
	{
		list->data->Update(dt);
	}


	return ret;
}

bool Entities::PostUpdate()
{
	bool ret = true;

	ListItem<Entity*>* list;
	for (list = entities.start; list != NULL && list->data != nullptr; list = list->next)
	{
		list->data->Draw();
	}


	return ret;
}

bool Entities::CleanUp()
{
	bool ret = true;

	app->tex->UnLoad(spritesheetTexture);

	ListItem<Entity*>* list;
	list = entities.start;
	for (int i = 0; i < entities.Count(); ++i)
	{
		if (list != NULL)
		{
			entities.Del(list);
		}
		list = list->next;
	}
	entities.Clear();

	return ret;
}

bool Entities::AddEntity(EntityType type, int x, int y)
{
	bool ret = true;

	EntitySpawnpoint spawn;
	spawn.type = type;
	spawn.x = x;
	spawn.y = y;

	spawnQueue.Add(spawn);

	return ret;
}

void Entities::HandleEnemiesSpawn()
{

	ListItem<EntitySpawnpoint>* list;
	list = spawnQueue.start;
	int count = spawnQueue.Count();

	for (int i = 0; i < count; ++i)
	{
		if (list->data.type != EntityType::NO_TYPE)
		{
			SpawnEnemy(list->data);
			list->data.type = EntityType::NO_TYPE;
		}
		list = list->next;
	}
}

void Entities::HandleEnemiesDespawn()
{
	ListItem<Entity*>* list;
	list = entities.start;
	for (int i = 0; i < entities.Count(); ++i)
	{
		if (list != NULL)
		{
			if (list->data->position.x * app->win->GetScale() < (app->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", list->data->position.x * app->win->GetScale());

				list->data->SetToDelete();
			}
		}
		list = list->next;
	}
}

void Entities::SpawnEnemy(const EntitySpawnpoint& info)
{
	Entity* newEntity = NULL;
	switch (info.type)
	{
	case EntityType::ITEM_MUSHROOM:
	{
		newEntity = new ItemHealth(info.x, info.y);
		newEntity->texture = spritesheetTexture;

		break;
	}
	//case EntityType::ITEM_DIAMOND:
	//{
	//	newEntity = new ItemDiamond(info.x, info.y);
	//	newEntity->texture = spritesheetTexture;

	//	break;
	//}

	}
	entities.Add(newEntity);
}

bool Entities::OnCollision(Collider* c1, Collider* c2) // This is called through listener from the OnCollision(c2,c1) of player.cpp c2->entity rect c1->player rect
{
	ListItem<Entity*>* entitiesList;
	entitiesList = entities.start;
	for (int i = 0; i < entities.Count(); ++i)
	{
		entitiesList = entities.At(i);
		if (entitiesList->data->GetCollider() == c1)
			entitiesList->data->OnCollision(c2);
	}

	return true;
}
