#include "ModuleCollisions.h"

#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Log.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_Scancode.h"

Collider::Collider(SDL_Rect rectangle, Type type, Module* listener, Items item) : rect(rectangle), type(type), listener(listener), item(item)
{

}

void Collider::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	if ((rect.x > r.x + r.w) || (rect.x + rect.w < r.x) || (rect.y > r.y + r.h) || (rect.y + rect.h < r.y))
	{
		return false;
	}

	return true;
}

ModuleCollisions::ModuleCollisions(bool b) : Module()
{
	ListItem<Collider*>* colliders2;
	colliders2 = colliders.start;

	matrix[Collider::Type::BLOCK][Collider::Type::BLOCK] = false;
	matrix[Collider::Type::BLOCK][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::BLOCK][Collider::Type::TURTLE] = false;
	matrix[Collider::Type::BLOCK][Collider::Type::KNIGHT] = false;
	matrix[Collider::Type::BLOCK][Collider::Type::MONSTER] = false;
	matrix[Collider::Type::BLOCK][Collider::Type::ITEM] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::BLOCK] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::TURTLE] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::KNIGHT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::MONSTER] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::ITEM] = true;

	matrix[Collider::Type::TURTLE][Collider::Type::BLOCK] = false;
	matrix[Collider::Type::TURTLE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::TURTLE][Collider::Type::TURTLE] = false;
	matrix[Collider::Type::TURTLE][Collider::Type::KNIGHT] = false;
	matrix[Collider::Type::TURTLE][Collider::Type::MONSTER] = false;
	matrix[Collider::Type::TURTLE][Collider::Type::ITEM] = false;

	matrix[Collider::Type::KNIGHT][Collider::Type::BLOCK] = false;
	matrix[Collider::Type::KNIGHT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::KNIGHT][Collider::Type::TURTLE] = false;
	matrix[Collider::Type::KNIGHT][Collider::Type::KNIGHT] = false;
	matrix[Collider::Type::KNIGHT][Collider::Type::MONSTER] = false;
	matrix[Collider::Type::KNIGHT][Collider::Type::ITEM] = false;

	matrix[Collider::Type::MONSTER][Collider::Type::BLOCK] = false;
	matrix[Collider::Type::MONSTER][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::MONSTER][Collider::Type::TURTLE] = false;
	matrix[Collider::Type::MONSTER][Collider::Type::KNIGHT] = false;
	matrix[Collider::Type::MONSTER][Collider::Type::MONSTER] = false;
	matrix[Collider::Type::MONSTER][Collider::Type::ITEM] = false;

	matrix[Collider::Type::ITEM][Collider::Type::BLOCK] = false;
	matrix[Collider::Type::ITEM][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ITEM][Collider::Type::TURTLE] = false;
	matrix[Collider::Type::ITEM][Collider::Type::KNIGHT] = false;
	matrix[Collider::Type::ITEM][Collider::Type::MONSTER] = false;
	matrix[Collider::Type::ITEM][Collider::Type::ITEM] = false;
}
// Destructor
ModuleCollisions::~ModuleCollisions()
{

}

bool ModuleCollisions::PreUpdate()
{
	bool ret = true;

	// Remove all colliders scheduled for deletion
	ListItem<Collider*>* L;
	L = colliders.start;
	for (int i = 0; i < colliders.Count(); ++i)
	{
		if (L != NULL && L->data->pendingToDelete == true)
		{
			colliders.Del(L);
		}
		L = L->next;
	}

	Collider* c1;
	Collider* c2;

	// app->player->collisionExist = false;

	ListItem<Collider*>* list1;
	list1 = colliders.start;
	for (int i = 0; i < colliders.Count(); ++i)
	{
		// skip empty colliders
		if (list1 == NULL)
		{
			list1 = list1->next;
			continue;
		}

		c1 = list1->data;

		ListItem<Collider*>* list2;
		list2 = colliders.start;
		for (int k = i + 1; k < colliders.Count(); ++k)
		{
			if (list2 == NULL)
			{
				if (list2->next == NULL)
					break;
				list2 = list2->next;
				continue;
			}

			c2 = list2->data;

			if (c1->Intersects(c2->rect))
			{
				if (matrix[c1->type][c2->type] && c1->listener)
				{
					if (c1->type == Collider::Type::PLAYER)
					{
						app->player->collisionExist = c1->listener->OnCollision(c1, c2);
					}
					else
						c1->listener->OnCollision(c1, c2);
				}


				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);
			}

			list2 = list2->next;
		}
		list1 = list1->next;
	}

	return ret;
}

bool ModuleCollisions::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	return ret;
}

bool ModuleCollisions::PostUpdate()
{
	bool ret = true;
	if (debug)
		DebugDraw();

	return ret;
}

void ModuleCollisions::DebugDraw()
{
	ListItem<Collider*>* list1;
	list1 = colliders.start;
	Uint8 alpha = 160;
	for (int i = 0; i < colliders.Count(); ++i)
	{
		if (list1 == NULL)
		{
			list1 = list1->next;
			continue;
		}

		switch (list1->data->type)
		{
		case Collider::Type::BLOCK: // Blue
			app->render->DrawRectangle(list1->data->rect, 0, 165, 255, 200);
			break;
		case Collider::Type::PLAYER: // Green
			app->render->DrawRectangle(list1->data->rect, 0, 255, 0, alpha);
			break;
		case Collider::Type::TURTLE: // red
			app->render->DrawRectangle(list1->data->rect, 255, 0, 0, alpha);
			break;
		case Collider::Type::KNIGHT: // orange
			app->render->DrawRectangle(list1->data->rect, 255, 165, 0, alpha);
			break;
		case Collider::Type::MONSTER: // red	
			app->render->DrawRectangle(list1->data->rect, 255, 0, 0, alpha);
			break;
		case Collider::Type::ITEM: // Dark blue	
			app->render->DrawRectangle(list1->data->rect, 238, 130, 238, alpha);
			break;
		}
		list1 = list1->next;
	}
}

// Called before quitting
bool ModuleCollisions::CleanUp()
{
	LOG("Freeing all colliders");

	ListItem<Collider*>* listColl;
	listColl = colliders.start;
	for (int i = 0; i < colliders.Count(); ++i)
	{
		if (listColl != NULL)
		{
			colliders.Del(listColl);
		}
		listColl = listColl->next;
	}
	colliders.Clear();
	return true;
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener, Collider::Items Item)
{
	Collider* ret = new Collider(rect, type, listener, Item);
	colliders.Add(ret);
	return ret;
}

void ModuleCollisions::RemoveCollider(Collider* collider)
{
	ListItem<Collider*>* listColl;
	listColl = colliders.start;
	for (int i = 0; i < colliders.Count(); ++i)
	{
		if (listColl != NULL && listColl->data == collider)
		{
			colliders.Del(listColl);
		}
		listColl = listColl->next;
	}
}