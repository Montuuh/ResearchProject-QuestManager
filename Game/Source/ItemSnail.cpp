#include "ItemSnail.h"

#include "App.h"
#include "ModuleCollisions.h"


ItemSnail::ItemSnail(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;
	this->entityType = EntityType::ITEM_SNAIL;
	anim.PushBack({ 0,2,14,22 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 14 , 22 }, Collider::Type::ITEM, (Module*)app->entities, Collider::Items::SNAIL);
}

void ItemSnail::Update(float dt)
{
	Entity::Update(dt);
}