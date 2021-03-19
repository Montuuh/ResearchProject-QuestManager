#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	// Constructor
	ModulePlayer(bool b);

	// Destructor
	~ModulePlayer();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start();

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate();

	bool CleanUp();

	// Collision callback, called when the player intersects with another collider
	bool OnCollision(Collider* c1, Collider* c2);

	bool CheckCollisions(float dt);

public:
	Animation* currentAnimation = nullptr;
	Animation idleLeftAnim;
	Animation idleRightAnim;
	Animation idleDownAnim;
	Animation idleUpAnim;
	Animation runLeftAnim;
	Animation runRightAnim;
	Animation runDownAnim;
	Animation runUpAnim;

	fPoint velocity = { 0.0f, 0.0f };

	fPoint playerPos;
	fPoint colliderPos;

	Collider* playerCollider = nullptr;
	Collider* previousCollision = nullptr;

	SDL_Texture* characterTexture = nullptr;
	SDL_Texture** currentTexture = nullptr;

	bool collisionExist = false;

private:
	void Input(float dt);
	void Logic(float dt);

	bool godMode;
	bool goingUp;
	bool goingDown;
	bool goingLeft;
	bool goingRight;

};

#endif //!__MODULE_PLAYER_H__
