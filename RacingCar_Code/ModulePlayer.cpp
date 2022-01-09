#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;

	looser = false;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

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
	vehicle->is_vehicle = true;
	vehicle->is_ball = false;
	vehicle->is_water = false;
	vehicle->is_sensor = false;
	vehicle->SetPos(0, 0.5f, 0);
	btQuaternion q;
	q.setEuler(btScalar(0 * DEGTORAD), btScalar(0), btScalar(0));
	vehicle->SetRotation(q);
	vehicle->body->setUserPointer(vehicle);
	breakFX = App->audio->LoadFx("Assets/FX/break.wav");
	hornFX = App->audio->LoadFx("Assets/FX/horn.wav");

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
	forwardVector = vehicle->vehicle->getForwardVector().normalize();
	vehicle->vehicle->getChassisWorldTransform();
	perpendicularVector = { -forwardVector.getZ(), forwardVector.getY(), forwardVector.getX() };

	if (INITIAL_TIME - App->scene_intro->timer == 5)
	{
		canMove = true;
	}
	turn = acceleration = brake = 0.0f;

	// UPTADE AERODYNAMIC FORCE
	App->physics->Aerodynamics(car, *vehicle);

	//UPDATE HYDRODYNAMIC FORCE
	
	App->physics->Hidrodynamics(car, *vehicle, *App->scene_intro->water[0]);
	App->physics->Hidrodynamics(car, *vehicle, *App->scene_intro->water[1]);
	
	LOG("%d", App->physics->isInWater);

	// PLAYER LIMITS
	if (vehicle->GetPos().x < -95 || vehicle->GetPos().x > 95 || vehicle->GetPos().z < -95 || vehicle->GetPos().z > 95)
	{
		Restart();
	}
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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->audio->PlayFx(hornFX);

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
		//App->audio->PlayFx(breakFX);
		brake = BRAKE_POWER;
	}


	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && vehicle->GetKmh() <= 0 && !minVelAch)
	{
		acceleration = - MAX_ACCELERATION;
		backwards = true;

	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Push(App->physics->FdAx, App->physics->FdAy, App->physics->FdAz);
	vehicle->Push(App->physics->FdHx, 0, App->physics->FdHz);

	//LOG("X: %.2f, Y: %.2f, Z: %.2f", App->physics->FdHx, App->physics->FdAy, App->physics->FdHz);

	vehicle->Render();

	if (App->scene_intro->timer <= 0)
	{
		looser = true;
	}

	if (vehicle->GetPos().y < -1)
	{
		vehicle->SetPos(0, 0.5, 0);
	}

	btVector3 airControl;
	airControl = vehicle->vehicle->getChassisWorldTransform().getOrigin();
	if (airControl.getY() > 3)
	{
		Euler angles = vehicle->GetEulerAngles(vehicle->vehicle->getChassisWorldTransform().getRotation());

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			vehicle->body->applyTorque(perpendicularVector * -1500);
			/*angles.yaw -= (DEGTORAD * 2);
			btQuaternion q;
			q.setEulerZYX(btScalar(angles.yaw), btScalar(angles.pitch), btScalar(angles.roll));
			vehicle->SetRotation(q);*/
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			vehicle->body->applyTorque(perpendicularVector * 2500);
			/*angles.yaw += (DEGTORAD * 2);
			btQuaternion q;
			q.setEulerZYX(btScalar(angles.yaw), btScalar(angles.pitch), btScalar(angles.roll));
			vehicle->SetRotation(q);*/
		}


		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			vehicle->body->applyTorque(forwardVector * -2500);
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			vehicle->body->applyTorque(forwardVector * 2500);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) Restart();

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		float x = 0, y = 0, z = 0;
		x = vehicle->GetPos().x;
		z = vehicle->GetPos().z;
		vehicle->SetPos(x, 0.5f, z);
		btQuaternion q;
		q.setEuler(btScalar(vehicle->GetForwardVector().x * DEGTORAD), btScalar(0), btScalar(0));
		vehicle->SetRotation(q);
	}


	char title[80];
	if (!App->scene_intro->is_playing_goal && !looser)
	{
		if (vehicle->GetKmh() < 0.5 && vehicle->GetKmh() > -0.5)
		{
			sprintf_s(title, "0 Km/h --- Time Left %d s", App->scene_intro->timer);

		}
		else
		{
			sprintf_s(title, "%.1f Km/h --- Time Left %d s", vehicle->GetKmh(), App->scene_intro->timer);

		}

	}
	else if (App->scene_intro->is_playing_goal && !looser)
	{
		sprintf_s(title, " YOU WIN!! --- PRESS 'R' TO RESTART --- %.1f Km/h", vehicle->GetKmh());

	}
	else if (!App->scene_intro->is_playing_goal && looser)
	{
		vec3 position = vec3(-150.0f, 40.0f, 5.0f);
		vec3 reference = vec3(0.0f, 0.0f, 0.0f);

		cam_follow = false;
		App->camera->Look(position, reference);

		if (!App->scene_intro->is_playing_looser)
		{
			App->audio->PlayFx(App->scene_intro->looseFX);
			App->scene_intro->is_playing_looser = true;
		}

		sprintf_s(title, " YOU LOOSE :( --- PRESS 'R' TO RESTART --- %.1f Km/h", vehicle->GetKmh());
	}
	else
	{
		sprintf_s(title, "PLEASE PRESS 'R' TO RESTART --- %.1f Km/h", vehicle->GetKmh());

	}

	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::Restart()
{
	turn = 0;
	acceleration = 0;
	vehicle->SetPos(0, 0.5f, 0);
	App->scene_intro->b->SetPos(-50, 2, 60);
	looser = false;
	cam_follow = true;
	App->scene_intro->is_playing_goal = false;
	App->scene_intro->is_playing_looser = false;
	btQuaternion q;
	q.setEuler(btScalar(0 * DEGTORAD), btScalar(0), btScalar(0));
	vehicle->SetRotation(q);
	App->scene_intro->timer = INITIAL_TIME;

}
