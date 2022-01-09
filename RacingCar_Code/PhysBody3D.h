#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btQuaternion;
class btRigidBody;
class Module;
class vec3;

struct Euler
{
	float roll, pitch, yaw;
};

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetRotation(btQuaternion q);
	Euler GetEulerAngles(btQuaternion q);
	

	const vec3 GetPos() const;

private:
	

public:
	p2List<Module*> collision_listeners;
	bool is_sensor;
	bool is_ball;
	bool is_water;
	bool is_vehicle;
	btRigidBody* body = nullptr;
};

#endif // __PhysBody3D_H__