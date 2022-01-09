#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2List.h"
#include "Primitive.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

#define MAX_SNAKE 2
#define INITIAL_TIME 120

#define SKY_LIMITS 400
#define WALL_MASS 0
#define MAX_WALLS 40
#define MAX_GOALKEEPERS 5

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/
	Cube* ground;

	Cube* sky[5];

	Cube* wall[MAX_WALLS];

	Cube* water[2];

	PhysBody3D* waterbody[2];

	Cube* cubeConstFix[MAX_GOALKEEPERS];
	Cube* cubeConstMobile[MAX_GOALKEEPERS];
	PhysBody3D* constFix[MAX_GOALKEEPERS];
	PhysBody3D* constMobile[MAX_GOALKEEPERS];
	btHingeConstraint* hinge[MAX_GOALKEEPERS];


	Cube* sensor;

	uint goalFX;
	bool is_playing_goal = false;

	uint looseFX;
	bool is_playing_looser = false;

	Sphere* ball;

	PhysBody3D* b;


	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	uint timer = INITIAL_TIME;
	uint frames = 0;

private:
	p2DynArray<Primitive*> primitives;

};
