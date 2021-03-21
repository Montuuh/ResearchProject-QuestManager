#include "ItemRubbish.h"

#include "App.h"
#include "ModuleCollisions.h"


ItemRubbish::ItemRubbish(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;
	this->entityType = EntityType::ITEM_RUBBISH;
	anim.PushBack({ 108,24,13,24 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 13 , 24 }, Collider::Type::ITEM, (Module*)app->entities, Collider::Items::RUBBISH);
}

void ItemRubbish::Update(float dt)
{
	Entity::Update(dt);
}