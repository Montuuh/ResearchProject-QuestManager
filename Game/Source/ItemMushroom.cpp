#include "ItemMushroom.h"

#include "App.h"
#include "ModuleCollisions.h"


ItemHealth::ItemHealth(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;
	this->entityType = EntityType::ITEM_MUSHROOM;
	anim.PushBack({ 16,3,14,12 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 14 , 12 }, Collider::Type::ITEM, (Module*)app->entities, Collider::Items::MUSHROOM);
}

void ItemHealth::Update(float dt)
{
	Entity::Update(dt);
}
