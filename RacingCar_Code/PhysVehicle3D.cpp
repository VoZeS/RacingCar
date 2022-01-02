#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	chassis.color = Yellow;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();


	Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z);
	cabin.color = LightBlue;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);
	btVector3 cabin_offset(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	cabin_offset = cabin_offset.rotate(q.getAxis(), q.getAngle());

	cabin.transform.M[12] += cabin_offset.getX();
	cabin.transform.M[13] += cabin_offset.getY();
	cabin.transform.M[14] += cabin_offset.getZ();

	Cube tube(info.tube_size.x, info.tube_size.y, info.tube_size.z);
	tube.color = Yellow;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&tube.transform);
	btVector3 tube_offset(info.tube_offset.x, info.tube_offset.y, info.tube_offset.z);
	tube_offset = tube_offset.rotate(q.getAxis(), q.getAngle());

	tube.transform.M[12] += tube_offset.getX();
	tube.transform.M[13] += tube_offset.getY();
	tube.transform.M[14] += tube_offset.getZ();

	Cube tubeThin(info.tubeThin_size.x, info.tubeThin_size.y, info.tubeThin_size.z);
	tubeThin.color = Grey;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&tubeThin.transform);
	btVector3 tubeThin_offset(info.tubeThin_offset.x, info.tubeThin_offset.y, info.tubeThin_offset.z);
	tubeThin_offset = tubeThin_offset.rotate(q.getAxis(), q.getAngle());

	tubeThin.transform.M[12] += tubeThin_offset.getX();
	tubeThin.transform.M[13] += tubeThin_offset.getY();
	tubeThin.transform.M[14] += tubeThin_offset.getZ();

	Cube hat(info.hat_size.x, info.hat_size.y, info.hat_size.z);
	hat.color = Yellow;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&hat.transform);
	btVector3 hat_offset(info.hat_offset.x, info.hat_offset.y, info.hat_offset.z);
	hat_offset = hat_offset.rotate(q.getAxis(), q.getAngle());

	hat.transform.M[12] += hat_offset.getX();
	hat.transform.M[13] += hat_offset.getY();
	hat.transform.M[14] += hat_offset.getZ();

	Cube motor(info.motor_size.x, info.motor_size.y, info.motor_size.z);
	motor.color = Yellow;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&motor.transform);
	btVector3 motor_offset(info.motor_offset.x, info.motor_offset.y, info.motor_offset.z);
	motor_offset = motor_offset.rotate(q.getAxis(), q.getAngle());

	motor.transform.M[12] += motor_offset.getX();
	motor.transform.M[13] += motor_offset.getY();
	motor.transform.M[14] += motor_offset.getZ();

	chassis.Render();
	cabin.Render();
	tube.Render();
	tubeThin.Render();
	hat.Render();
	motor.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

vec3 PhysVehicle3D::GetForwardVector() const
{
	btVector3 h = vehicle->getForwardVector();
	vec3 ret;
	ret.Set(h.getX(), h.getY(), h.getZ());
	return ret;
}