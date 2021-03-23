#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <DirectXMath.h>
#include "InputCommands.h"
#include "SceneObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera(std::string id);
	~Camera();

	Matrix GetViewMatrix() { return _ViewMatrix; }
	Matrix GetProjectionMatrix() { return _ProjectionMatrix; }
	void SetProjectionMatrix(Matrix projection) { _ProjectionMatrix = projection; }
	Vector3 GetLookAtVector() { return _LookAt; }

	virtual void Reset() = 0;
	virtual void HandleInput(InputCommands input, float deltaTime, float w, float h) = 0;
	virtual void Update(float deltaTime) = 0;

	std::string GetID() { return _ID; }
	void SetID(std::string id) { _ID = id; }

	Vector3 GetEyePosition() { return _Eye; }
	void SetEyePosition(Vector3 position) { _Eye = position; }

	Vector3 GetPitchYawRoll() { return Vector3(_Pitch, _Yaw, _Roll); }
	void SetPitchYawRoll(Vector3 pitchYawRoll) { _Pitch = pitchYawRoll.x; _Yaw = pitchYawRoll.y; _Roll = pitchYawRoll.z; }

	bool GetMovedThisFrame() { return _MovedThisFrame; }
	void SetMovedThisFrame(bool moved) { _MovedThisFrame = moved; }

	virtual void TryFocus(SceneObject obj) {};

	void OnWindowSizeChanged(RECT newSize);

protected:
	std::string _ID;

	Vector3 _Eye;
	Vector3 _LookAt;
	Vector3 _Up;
	Vector3 _Forward;
	Vector3 _Right;

	float _FOV;
	float _AspectRatio;
	float _NearPlane;
	float _FarPlane;

	float _Pitch;
	float _Yaw;
	float _Roll;

	float _MovementSpeed;
	float _RotationSpeed;

	Matrix _ViewMatrix;
	Matrix _ProjectionMatrix;

	bool _MovedThisFrame;
};

