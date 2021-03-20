#include "ModulePlayer.h"

#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "Window.h"
#include "Map.h"
#include "QuestManager.h"

#include "Log.h"

#include <stdio.h>
#include "SDL/include/SDL_scancode.h"


//Now temporally is this
#define VELOCITY 200.0f
#define MAXVELOCITY_X 300.0f
#define MAXVELOCITY_Y 300.0f // Before it was 1000.0f

ModulePlayer::ModulePlayer(bool b) : Module()
{
	name.Create("player");

	// Left Idle Animation
	idleLeftAnim.PushBack({ 56, 11, 28, 32 });

	// Right Idle Animation
	idleRightAnim.PushBack({ 155, 11, 28, 32 });

	// Down Idle Animation
	idleDownAnim.PushBack({ 10, 11, 29, 31 });

	// Up Idle Animation
	idleUpAnim.PushBack({ 106, 11, 29, 31 });

	// Left Run Animation
	runLeftAnim.PushBack({ 56, 11, 28, 32 });
	runLeftAnim.PushBack({ 56, 59, 28, 32 });
	runLeftAnim.PushBack({ 56, 107, 28, 32 });
	runLeftAnim.PushBack({ 56, 155, 28, 32 });
	runLeftAnim.loop = true;
	runLeftAnim.speed = 0.01f;

	// Right Run Animation
	runRightAnim.PushBack({ 155, 11, 28, 32 });
	runRightAnim.PushBack({ 155, 59, 28, 32 });
	runRightAnim.PushBack({ 155, 107, 28, 32 });
	runRightAnim.PushBack({ 155, 155, 28, 32 });
	runRightAnim.loop = true;
	runRightAnim.speed = 0.01f;

	// Down Run Animation
	runDownAnim.PushBack({ 10, 11, 29, 31 });
	runDownAnim.PushBack({ 10, 59, 29, 32 });
	runDownAnim.PushBack({ 10, 107, 29, 31 });
	runDownAnim.PushBack({ 10, 155, 29, 32 });
	runDownAnim.loop = true;
	runDownAnim.speed = 0.01f;

	// Up Run Animation
	runUpAnim.PushBack({ 106, 11, 29, 31 });
	runUpAnim.PushBack({ 106, 59, 29, 32 });
	runUpAnim.PushBack({ 106, 107, 29, 31 });
	runUpAnim.PushBack({ 106, 155, 29, 32 });
	runUpAnim.loop = true;
	runUpAnim.speed = 0.01f;
}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	characterTexture = app->tex->Load("Assets/Textures/george.png");

	// Starting position of the player
	playerPos = { 320.0f,32.0f };

	// Setting starting position of the player's collider
	playerCollider = app->collisions->AddCollider({ (int)playerPos.x + 3,(int)playerPos.y + 3,26,26 }, Collider::Type::PLAYER, (Module*)app->player);

	// Loading starting animation && texture
	currentAnimation = &idleDownAnim;
	currentTexture = &characterTexture;

	godMode = false;

	return ret;
}

bool ModulePlayer::Update(float dt)
{
	bool ret = true;

	if (dt > 1.0f / 30.0f)
	{
		dt = 1.0f / 30.0f;
	}

	Input(dt);

	Logic(dt);
	CheckCollisions(dt);

	if (currentAnimation != NULL)
	{
		currentAnimation->Update();
	}

	return ret;
}

void ModulePlayer::Input(float dt)
{

	// Player movement when going to the left
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
	{
		if (currentAnimation != &runLeftAnim)
		{
			runLeftAnim.Reset();
			currentAnimation = &runLeftAnim;
		}
		playerPos.x -= 0.4f;
		goingLeft = true;
	}

	// Stop left movement when player stop pressing to the left
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_UP))
	{
		currentAnimation = &idleLeftAnim;
		goingLeft = false;
	}

	// Player movement when going to the right
	if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
	{
		if (currentAnimation != &runRightAnim)
		{
			runRightAnim.Reset();
			currentAnimation = &runRightAnim;
		}
		playerPos.x += 0.4f;
		goingRight = true;
	}

	// Stop left movement when player stop pressing to the right
	if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_UP))
	{
		currentAnimation = &idleRightAnim;
		goingRight = false;
	}

	// Player movement when going up
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
	{
		if (currentAnimation != &runUpAnim)
		{
			runUpAnim.Reset();
			currentAnimation = &runUpAnim;
		}
		playerPos.y -= 0.4f;
		goingUp = true;
	}

	// Stop left movement when player stop pressing up
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_UP))
	{
		currentAnimation = &idleUpAnim;
		goingUp = false;
	}

	// Player movement when going down
	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
	{
		if (currentAnimation != &runDownAnim)
		{
			runDownAnim.Reset();
			currentAnimation = &runDownAnim;
		}
		playerPos.y += 0.4f;
		goingDown = true;
	}

	// Stop left movement when player stop pressing down
	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_UP))
	{
		currentAnimation = &idleDownAnim;
		goingDown = false;
	}
	if ((app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN))
	{
		playerPos = { 320.0f,32.0f };
	}
}

void ModulePlayer::Logic(float dt)
{
	// Borders for the player
	if (playerPos.x < 0)
		playerPos.x = 0;
	if (playerPos.x + playerCollider->rect.w > 40 * 32)
		playerPos.x = 40 * 32 - playerCollider->rect.w;
	if (playerPos.y < 0)
		playerPos.y = 0;
	if (playerPos.y + playerCollider->rect.h > 31 * 32)
		playerPos.y = 31 * 32 - playerCollider->rect.y;

	playerCollider->SetPos(playerPos.x+2, playerPos.y+2);
}



bool ModulePlayer::PostUpdate()
{
	bool ret = true;

	if (currentAnimation != NULL)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(*currentTexture, playerPos.x, playerPos.y, &rect);
	}

	return ret;
}

bool ModulePlayer::CheckCollisions(float dt)
{
	bool ret = true;
	collisionExist = false;

	ListItem<Collider*>* L;

	for (L = app->collisions->colliders.start; L != NULL; L = L->next)
	{
		if (playerCollider->Intersects(L->data->rect))
		{
			collisionExist = this->OnCollision(playerCollider, L->data);
			if (L->data->listener != nullptr && L->data->type != Collider::Type::PLAYER)
			{
				L->data->listener->OnCollision(L->data, playerCollider);
			}
		}
	}
	return ret;
}

bool ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;

	if (c2->type == Collider::Type::BLOCK)
	{
		// Collision from the right
		if (playerCollider->rect.x + playerCollider->rect.w > c2->rect.x && (playerCollider->rect.x + playerCollider->rect.w < c2->rect.x + c2->rect.w / 2))
			playerPos.x = c2->rect.x - playerCollider->rect.w - 1;

		// Collision from the left
		if (playerCollider->rect.x < c2->rect.x + c2->rect.w && (playerCollider->rect.x > c2->rect.x + c2->rect.w / 2))
			playerPos.x = c2->rect.x + c2->rect.w;

		// Collision from the top
		if (playerCollider->rect.y < c2->rect.y + c2->rect.h && (playerCollider->rect.y > c2->rect.y + c2->rect.h / 2))
			playerPos.y = c2->rect.y + c2->rect.h;

		// Collision from the bottom
		if (playerCollider->rect.y + playerCollider->rect.h > c2->rect.y && (playerCollider->rect.y + playerCollider->rect.h < c2->rect.y + c2->rect.h / 2))
			playerPos.y = c2->rect.y - playerCollider->rect.h - 1;

		previousCollision = c2;
	}

	if (c2->type == Collider::Type::CHECKPOINT && (previousCollision->type != Collider::Type::CHECKPOINT))
	{

		previousCollision = c2;
	}

	if (c2->type == Collider::Type::DEATH && (previousCollision->type != Collider::Type::DEATH) && !godMode)
	{

		previousCollision = c2;
	}

	if (c2->type == Collider::Type::ITEM /*&& (previousCollision->type != Collider::Type::ITEM)*/)
	{

		switch (c2->item)
		{
		case Collider::Items::MUSHROOM:
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				previousCollision = c2;
				c2->listener->OnCollision(c2, c1);
				mushroomCount++;
				break;
			}
		//case Collider::Items::DIAMOND:

		//	previousCollision = c2;
		//	c2->listener->OnCollision(c2, c1);
		//	break;
		default:
			break;
		}


	}

	if (c2->type == Collider::Type::WIN && (previousCollision->type != Collider::Type::WIN))
	{

		previousCollision = c2;
	}

	return ret;
}

bool ModulePlayer::CleanUp()
{
	app->tex->UnLoad(characterTexture);

	return true;
}

bool ModulePlayer::RewardXP(int rewardXP)
{
	xp += rewardXP;
	return true;
}

bool ModulePlayer::RewardGold(int rewardGold)
{
	gold += rewardGold;
	return true;
}