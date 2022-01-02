#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ground = new Cube(SKY_LIMITS, 0, SKY_LIMITS);
	ground->axis = false;
	ground->color = White;

	sky1 = new Cube(0, SKY_LIMITS, SKY_LIMITS);
	sky1->SetPos(SKY_LIMITS/2, 0, 0);
	sky1->axis = false;
	sky1->color = Cyan;

	sky2 = new Cube(0, SKY_LIMITS, SKY_LIMITS);
	sky2->SetPos(-SKY_LIMITS/2, 0, 0);
	sky2->axis = false;
	sky2->color = Cyan;

	sky3 = new Cube(SKY_LIMITS, 0, SKY_LIMITS);
	sky3->SetPos(0, SKY_LIMITS/2, 0);
	sky3->axis = false;
	sky3->color = Cyan;

	sky4 = new Cube(SKY_LIMITS, SKY_LIMITS, 0);
	sky4->SetPos(0, 0, SKY_LIMITS/2);
	sky4->axis = false;
	sky4->color = Cyan;

	sky5 = new Cube(SKY_LIMITS, SKY_LIMITS, 0);
	sky5->SetPos(0, 0, -SKY_LIMITS/2);
	sky5->axis = false;
	sky5->color = Cyan;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	ground->Render();
	sky1->Render();
	sky2->Render();
	sky3->Render();
	sky4->Render();
	sky5->Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

