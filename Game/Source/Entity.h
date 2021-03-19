#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "EntityManager.h"

struct SDL_Texture;
struct Collider;

class Entity
{
public:

	// Constructor
	Entity(int x, int y);

	// Destructor
	virtual ~Entity();

	// Returns the enemy's collider
	const Collider* GetCollider() const;

	// Called from inhering enemies' Udpate
	// Updates animation and collider position
	virtual void Update(float dt);

	// Called from ModuleEnemies' Update
	virtual void Draw();

	// Collision response
	virtual void OnCollision(Collider* collider);

	// Sets flag for deletion and for the collider aswell
	virtual void SetToDelete();

public:
	iPoint position;

	Collider* collider = nullptr;

	SDL_Texture* texture = nullptr;
	SDL_Texture* debugTexture = nullptr;

	int destroyedFx = 0;

	bool pendingToDelete = false;

	SString name = SString("NONE");

	bool isDead = false;

	EntityType entityType;

protected:
	Animation* currentAnim = nullptr;

	iPoint spawnPos;

	float counter = 0;
	bool inCollision = false;

	int tempx = 0;
};

#endif // __ENEMY_H__
