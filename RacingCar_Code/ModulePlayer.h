#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define MAX_VEL 100.0f
#define MIN_VEL -50.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 100.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Restart();

	bool cam_follow = true;
	bool looser = false;
	bool maxVelAch = false;
	bool minVelAch = false;

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	bool canMove;
	bool backwards = false;

	btVector3 forwardVector;
	btVector3 perpendicularVector;

	//FX
	uint breakFX;
	uint hornFX;
	uint engineOnFX;

};