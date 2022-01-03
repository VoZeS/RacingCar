#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 0.75f, 4);
	car.chassis_offset.Set(0, 1, 0);

	car.cabin_size.Set(1, 1.25f, 1.5f);
	car.cabin_offset.Set(0, 2, -1);

	car.tube_size.Set(0.25f, 0.75f, 0.25f);
	car.tube_offset.Set(0.75f, 1.75f, -1.5f);

	car.tubeThin_size.Set(0.25f/2, 0.75f, 0.25f/2);
	car.tubeThin_offset.Set(0.75f, 2.5f, -1.5f);

	car.hat_size.Set(1.25f, 0.1f, 1.75f);
	car.hat_offset.Set(0, 2.6f, -1);

	car.motor_size.Set(1, 0.75f, 1.75f);
	car.motor_offset.Set(0, 1.6f, 1);

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0.5f, 0);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	if (INITIAL_TIME - App->scene_intro->timer == 5)
	{
		canMove = true;
	}
	turn = acceleration = brake = 0.0f;
	
	//-------------------------------------------------------------------------------------------------------------- FOLLOWING CAMERA
	if (cam_follow)
	{
		float speed_cam = 0.2;
		vec3 p = vehicle->GetPos();
		vec3 f = vehicle->GetForwardVector();

		if (backwards)
		{
			vec3 dist_to_car = { 8.0f, 4.0f, 10.0f };
			vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
			vec3 speed_camera = camera_new_position - App->camera->Position;
			App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);

		}

		else
		{
			vec3 dist_to_car = { -8.0f, 4.0f, -10.0f };
			vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
			vec3 speed_camera = camera_new_position - App->camera->Position;
			App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);

		}


	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (cam_follow)
			cam_follow = false;
		else
			cam_follow = true;
	}
	
	//---------------------------------- END FOLLOWING CAMERA

	if (vehicle->GetKmh() >= MAX_VEL)
	{
		maxVelAch = true;
	}
	else
	{
		maxVelAch = false;

	}

	if (vehicle->GetKmh() <= MIN_VEL)
	{
		minVelAch = true;
	}
	else
	{
		minVelAch = false;

	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && vehicle->GetKmh() < 0 && !maxVelAch)
	{
		brake = BRAKE_POWER;
		backwards = false;
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && vehicle->GetKmh() >= 0 && !maxVelAch)
	{
		acceleration = MAX_ACCELERATION;
		backwards = false;

	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && vehicle->GetKmh() > 0 && !minVelAch)
	{
		brake = BRAKE_POWER;
		backwards = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && vehicle->GetKmh() <= 0 && !minVelAch)
	{
		acceleration = - MAX_ACCELERATION;
		backwards = true;

	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	if (App->scene_intro->timer <= 0)
	{
		cam_follow = false;
		
	}

	if (vehicle->GetPos().y < -1)
	{
		vehicle->SetPos(0, 0.5, 0);
	}

	char title[80];
	sprintf_s(title, "%.1f Km/h --- Time Left %d s", vehicle->GetKmh(), App->scene_intro->timer);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}



