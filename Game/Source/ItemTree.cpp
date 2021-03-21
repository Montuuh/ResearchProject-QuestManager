#include "ItemTree.h"

#include "App.h"
#include "ModuleCollisions.h"


ItemTree::ItemTree(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;
	this->entityType = EntityType::ITEM_TREE;
	anim.PushBack({ 114,51,24,30 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 24 , 30 }, Collider::Type::ITEM, (Module*)app->entities, Collider::Items::TREE);
}

void ItemTree::Update(float dt)
{
	Entity::Update(dt);
}