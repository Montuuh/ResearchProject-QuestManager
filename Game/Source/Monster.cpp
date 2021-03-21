#include "Monster.h"

#include "App.h"
#include "ModuleCollisions.h"


Monster::Monster(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;
	this->entityType = EntityType::MONSTER;
	anim.PushBack({ 58,0,48,36 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 48 , 36 }, Collider::Type::MONSTER, (Module*)app->entities, Collider::Items::ITEM_NONE);
}

void Monster::Update(float dt)
{
	Entity::Update(dt);
}