#include "Turtle.h"

#include "App.h"
#include "ModuleCollisions.h"


Turtle::Turtle(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;
	this->entityType = EntityType::TURTLE;
	anim.PushBack({ 82,72,33,39 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 33 , 39 }, Collider::Type::TURTLE, (Module*)app->entities, Collider::Items::ITEM_NONE);
}

void Turtle::Update(float dt)
{
	Entity::Update(dt);
}