#include "Knight.h"

#include "App.h"
#include "ModuleCollisions.h"


Knight::Knight(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;
	this->entityType = EntityType::KNIGHT;
	anim.PushBack({ 0,55,32,32 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 32 , 32 }, Collider::Type::KNIGHT, (Module*)app->entities, Collider::Items::ITEM_NONE);
}

void Knight::Update(float dt)
{
	Entity::Update(dt);
}