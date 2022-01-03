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

	sky[0] = new Cube(0, SKY_LIMITS, SKY_LIMITS);
	sky[0]->SetPos(SKY_LIMITS / 2, 0, 0);
	sky[0]->axis = false;
	sky[0]->color = Cyan;

	sky[1] = new Cube(0, SKY_LIMITS, SKY_LIMITS);
	sky[1]->SetPos(-SKY_LIMITS / 2, 0, 0);
	sky[1]->axis = false;
	sky[1]->color = Cyan;

	sky[2] = new Cube(SKY_LIMITS, 0, SKY_LIMITS);
	sky[2]->SetPos(0, SKY_LIMITS / 2, 0);
	sky[2]->axis = false;
	sky[2]->color = Cyan;

	sky[3] = new Cube(SKY_LIMITS, SKY_LIMITS, 0);
	sky[3]->SetPos(0, 0, SKY_LIMITS / 2);
	sky[3]->axis = false;
	sky[3]->color = Cyan;

	sky[4] = new Cube(SKY_LIMITS, SKY_LIMITS, 0);
	sky[4]->SetPos(0, 0, -SKY_LIMITS / 2);
	sky[4]->axis = false;
	sky[4]->color = Cyan;

	wall[0] = new Cube(1, 5, 98);
	wall[0]->SetPos(-10, 0, -20);
	wall[0]->axis = false;
	wall[0]->color = Red;

	wall[1] = new Cube(1, 5, 98);
	wall[1]->SetPos(10, 0, -40);
	wall[1]->axis = false;
	wall[1]->color = Red;

	wall[2] = new Cube(18, 5, 1);
	wall[2]->SetPos(0, 0, -10);
	wall[2]->axis = false;
	wall[2]->color = Red;

	wall[3] = new Cube(178, 5, 1);
	wall[3]->SetPos(0, 0, 30);
	wall[3]->axis = false;
	wall[3]->color = Red;

	wall[4] = new Cube(1, 5, 98);
	wall[4]->SetPos(30, 0, -20);
	wall[4]->axis = false;
	wall[4]->color = Red;

	wall[5] = new Cube(78, 5, 1);
	wall[5]->SetPos(50, 0, -90);
	wall[5]->axis = false;
	wall[5]->color = Red;

	wall[6] = new Cube(1, 5, 118);
	wall[6]->SetPos(90, 0, -30);
	wall[6]->axis = false;
	wall[6]->color = Red;

	wall[7] = new Cube(38, 5, 1);
	wall[7]->SetPos(50, 0, -70);
	wall[7]->axis = false;
	wall[7]->color = Red;

	wall[8] = new Cube(1, 5, 78);
	wall[8]->SetPos(70, 0, -30);
	wall[8]->axis = false;
	wall[8]->color = Red;

	wall[9] = new Cube(38, 5, 1);
	wall[9]->SetPos(50, 0, 10);
	wall[9]->axis = false;
	wall[9]->color = Red;

	//RAMP 1
	wall[10] = new Cube(20, 1, 10);
	wall[10]->SetPos(40, 0, 20);
	wall[10]->SetRotation(20.0f, { 0, 0, -1 });
	wall[10]->axis = false;
	wall[10]->color = Green;

	wall[11] = new Cube(1, 5, 98);
	wall[11]->SetPos(-90, 0, -20);
	wall[11]->axis = false;
	wall[11]->color = Red;

	wall[12] = new Cube(58, 5, 1);
	wall[12]->SetPos(-40, 0, 10);
	wall[12]->axis = false;
	wall[12]->color = Red;

	wall[13] = new Cube(1, 5, 58);
	wall[13]->SetPos(-70, 0, -20);
	wall[13]->axis = false;
	wall[13]->color = Red;

	wall[14] = new Cube(78, 5, 1);
	wall[14]->SetPos(-50, 0, -70);
	wall[14]->axis = false;
	wall[14]->color = Red;

	//RAMP 2
	wall[15] = new Cube(10, 1, 20);
	wall[15]->SetPos(-20, 0, 0);
	wall[15]->SetRotation(20.0f, { -1, 0, 0 });
	wall[15]->axis = false;
	wall[15]->color = Green;

	wall[16] = new Cube(38, 5, 1);
	wall[16]->SetPos(-50, 0, -50);
	wall[16]->axis = false;
	wall[16]->color = Red;

	wall[17] = new Cube(1, 5, 18);
	wall[17]->SetPos(-30, 0, -40);
	wall[17]->axis = false;
	wall[17]->color = Red;

	wall[18] = new Cube(38, 5, 1);
	wall[18]->SetPos(-30, 0, -10);
	wall[18]->axis = false;
	wall[18]->color = Red;

	wall[19] = new Cube(1, 5, 18);
	wall[19]->SetPos(-50, 0, -20);
	wall[19]->axis = false;
	wall[19]->color = Red;

	wall[20] = new Cube(180, 5, 1);
	wall[20]->SetPos(0, 0, 90);
	wall[20]->axis = false;
	wall[20]->color = Red;

	wall[21] = new Cube(1, 5, 58);
	wall[21]->SetPos(90, 0, 60);
	wall[21]->axis = false;
	wall[21]->color = Red;

	wall[22] = new Cube(1, 5, 58);
	wall[22]->SetPos(-90, 0, 60);
	wall[22]->axis = false;
	wall[22]->color = Red;

	wall[23] = new Cube(1, 5, 58);
	wall[23]->SetPos(30, 0, 60);
	wall[23]->axis = false;
	wall[23]->color = Red;

	//RAMP 3
	wall[24] = new Cube(20, 1, 20);
	wall[24]->SetPos(20, -1, 80);
	wall[24]->SetRotation(-20.0f, { 0, 0, -1 });
	wall[24]->axis = false;
	wall[24]->color = Green;

	//RAMP 4
	wall[25] = new Cube(20, 1, 20);
	wall[25]->SetPos(20, -1, 60);
	wall[25]->SetRotation(-20.0f, { 0, 0, -1 });
	wall[25]->axis = false;
	wall[25]->color = Green;

	//RAMP 5
	wall[26] = new Cube(20, 1, 20);
	wall[26]->SetPos(20, -1, 40);
	wall[26]->SetRotation(-20.0f, { 0, 0, -1 });
	wall[26]->axis = false;
	wall[26]->color = Green;

	wall[27] = new Cube(100, 10, 1);
	wall[27]->SetPos(40, 10, 30);
	wall[27]->axis = false;
	wall[27]->color = Red;

	wall[28] = new Cube(1, 10, 20);
	wall[28]->SetPos(-10, 10, 0);
	wall[28]->axis = false;
	wall[28]->color = Red;

	wall[29] = new Cube(42, 10, 1);
	wall[29]->SetPos(-48, 10, 10);
	wall[29]->axis = false;
	wall[29]->color = Red;

	// GOAL
	wall[30] = new Cube(1, 15, 1);
	wall[30]->SetPos(70, 7, 40);
	wall[30]->axis = false;
	wall[30]->color = Blue;

	wall[31] = new Cube(1, 15, 1);
	wall[31]->SetPos(70, 7, 80);
	wall[31]->axis = false;
	wall[31]->color = Blue;

	wall[32] = new Cube(1, 1, 40);
	wall[32]->SetPos(70, 15, 60);
	wall[32]->axis = false;
	wall[32]->color = Blue;

	wall[33] = new Cube(10, 15, 1);
	wall[33]->SetPos(75, 7, 40);
	wall[33]->axis = false;
	wall[33]->color = PureWhite;

	wall[34] = new Cube(10, 15, 1);
	wall[34]->SetPos(75, 7, 80);
	wall[34]->axis = false;
	wall[34]->color = PureWhite;

	wall[35] = new Cube(1, 15, 40);
	wall[35]->SetPos(80, 7, 60);
	wall[35]->axis = false;
	wall[35]->color = PureWhite;

	wall[36] = new Cube(10, 1, 40);
	wall[36]->SetPos(75, 15, 60);
	wall[36]->axis = false;
	wall[36]->color = PureWhite;
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

	for (int i = 0; i < MAX_WALLS; i++)
	{
		App->physics->AddBody(*wall[i], WALL_MASS);

	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	delete ground;

	for (int j = 0; j < 5; j++)
	{
		delete sky[j];
		sky[j] = nullptr;
	}

	for (int i = 0; i < MAX_WALLS; i++)
	{
		delete wall[i];
		wall[i] = nullptr;

	}

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	ground->Render();
	
	for (int j = 0; j < 5; j++)
	{
		sky[j]->Render();
	}

	for (int i = 0; i < MAX_WALLS; i++)
	{
		wall[i]->Render();
	}

	if (frames % 60 == 0 && timer > 0)
	{
		/*if (App->player->turboTimer > 0)
		{
			App->player->turboTimer--;
		}
		App->player->turboSoundActive = true;*/

		timer--;
	}

	frames++;
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

