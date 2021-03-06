#include "Entity.h"

#include "App.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "Render.h"
#include "EntityManager.h"
#include "Input.h"
#include "ModulePlayer.h"



Entity::Entity(int x, int y) : position(x, y)
{
	spawnPos = position;
}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Update(float dt)
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x + tempx, position.y);
}

void Entity::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
}

void Entity::OnCollision(Collider* collider)
{
	if (this->collider->type == Collider::ITEM && collider->type == Collider::Type::PLAYER)
	{
		switch (this->collider->item)
		{
		case Collider::Items::MUSHROOM:
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				this->SetToDelete();
			}
			break;
		case Collider::Items::TREE:
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && app->player->mushroomCount >= 8)
			{
				this->SetToDelete();
			}
			break;
		case Collider::Items::RUBBISH:
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				this->SetToDelete();
			}
			break;
	
		case Collider::Items::SNAIL:
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				this->SetToDelete();
			}
			break;

		default:
			break;
		}
	}
	if (this->collider->type == Collider::TURTLE && collider->type == Collider::Type::PLAYER)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			this->SetToDelete();
	}	
	if (this->collider->type == Collider::MONSTER && collider->type == Collider::Type::PLAYER)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			this->SetToDelete();
	}
}

void Entity::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr)
		collider->pendingToDelete = true;
}