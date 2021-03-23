#include "FPSCamera.h"

FPSCamera::FPSCamera(std::string id) : Camera(id)
{
	_MouseControlSpeed = 150.0f;
	Reset();
}

FPSCamera::~FPSCamera()
{

}

void FPSCamera::Reset()
{
	_MouseControlEnabled = false;
	Camera::Reset();
}

void FPSCamera::HandleInput(InputCommands input, float deltaTime, float w, float h)
{
	if (input.forward)
	{
		MoveForward(deltaTime);
	}

	if (input.back)
	{
		MoveForward(-deltaTime);
	}

	if (input.left)
	{
		MoveRight(-deltaTime);
	}

	if (input.right)
	{
		MoveRight(deltaTime);
	}

	if (input.up)
	{
		MoveUp(deltaTime);
	}

	if (input.down)
	{
		MoveUp(-deltaTime);
	}
	if (input.mouse_RB_Down)
	{
		if (!_MouseControlEnabled) {
			mousePos = Vector2(input.mouse_X, input.mouse_Y);
			_MouseControlEnabled = true;
		}

		if (_MouseControlEnabled) {
			if (input.mouse_X > mousePos.x) {
				_Yaw -= _MouseControlSpeed * 1 * deltaTime;

				if (input.mouse_Y > mousePos.y) {
					_Pitch += _MouseControlSpeed * 1 * deltaTime;
				}
				else if (input.mouse_Y < mousePos.y) {
					_Pitch -= _MouseControlSpeed * 1 * deltaTime;
				}
			}
			else if (input.mouse_X < mousePos.x) {
				_Yaw += _MouseControlSpeed * 1 * deltaTime;

				if (input.mouse_Y > mousePos.y) {
					_Pitch += _MouseControlSpeed * 1 * deltaTime;
				}
				else if (input.mouse_Y < mousePos.y) {
					_Pitch -= _MouseControlSpeed * 1 * deltaTime;
				}
			}
			mousePos = Vector2(input.mouse_X, input.mouse_Y);
		}
		else
		{
			_MouseControlEnabled = false;
		}
		//if (input.rotLeft)
		//{
		//	MoveYaw(-deltaTime);
		//}

		//if (input.rotRight)
		//{
		//	MoveYaw(deltaTime);
		//}

		//if (input.mouseMMBDown)
		//{
		//	_MovedThisFrame = _MouseControlEnabled != true;
		//	_MouseControlEnabled = true;
		//}

		//else
		//{
		//	_MovedThisFrame = _MouseControlEnabled != false;
		//	_MouseControlEnabled = false;
		//}

		//if (_MouseControlEnabled && !_MovedThisFrame)
		//{
		//	if (input.mouseXDelta > 0)
		//	{
		//		_Yaw += _MouseControlSpeed * deltaTime * log(abs(input.mouseXDelta));
		//		_MovedThisFrame = true;
		//	}

		//	else if (input.mouseXDelta < 0)
		//	{
		//		_Yaw += _MouseControlSpeed * deltaTime * log(abs(input.mouseXDelta)) * -1;
		//		_MovedThisFrame = true;
		//	}

		//	if (input.mouseYDelta > 0)
		//	{
		//		_Pitch += _MouseControlSpeed * deltaTime * log(abs(input.mouseYDelta)) * -1;
		//		_MovedThisFrame = true;
		//	}

		//	else if (input.mouseYDelta < 0)
		//	{
		//		_Pitch += _MouseControlSpeed * deltaTime * log(abs(input.mouseYDelta));
		//		_MovedThisFrame = true;
		//	}
		//}
	}
}

void FPSCamera::Update(float deltaTime)
{
	Vector3 planarMotionVector = _Forward;
	planarMotionVector.y = 0.0;

	_Forward.z = cos(XMConvertToRadians(_Yaw)) * sin(XMConvertToRadians(_Pitch));
	_Forward.x = sin(XMConvertToRadians(_Yaw)) * sin(XMConvertToRadians(_Pitch));
	_Forward.y = cos(XMConvertToRadians(_Pitch));
	_Forward.Normalize();

	_Up = Vector3::UnitY;
	_Forward.Cross(_Up, _Right);

	_LookAt = _Eye + _Forward;
	_ViewMatrix = Matrix::CreateLookAt(_Eye, _LookAt, _Up);

}

void FPSCamera::TryFocus(SceneObject obj)
{
	_Pitch = 225.0f;
	_Yaw = 180.0f;
	_Roll = 0.0f;

	_Eye = XMFLOAT3(obj.posX, obj.posY, obj.posZ) - (XMFLOAT3(0.0f, -7.0f, 7.0f) * XMFLOAT3(obj.scaX, obj.scaY, obj.scaZ));
	_MovedThisFrame = true;
}

void FPSCamera::MoveForward(float deltaTime)
{
	float xIncr = _Forward.x  * _MovementSpeed * deltaTime;
	float yIncr = _Forward.y  * _MovementSpeed * deltaTime;
	float zIncr = _Forward.z  * _MovementSpeed * deltaTime;

	_Eye.x += xIncr;
	_Eye.y += yIncr;
	_Eye.z += zIncr;

	_MovedThisFrame = true;
}

void FPSCamera::MoveRight(float deltaTime)
{
	float xIncr = _Right.x  * _MovementSpeed * deltaTime;
	float yIncr = _Right.y  * _MovementSpeed * deltaTime;
	float zIncr = _Right.z  * _MovementSpeed * deltaTime;

	_Eye.x += xIncr;
	_Eye.y += yIncr;
	_Eye.z += zIncr;

	_MovedThisFrame = true;
}

void FPSCamera::MoveUp(float deltaTime)
{
	float xIncr = _Up.x  * _MovementSpeed * deltaTime;
	float yIncr = _Up.y  * _MovementSpeed * deltaTime;
	float zIncr = _Up.z  * _MovementSpeed * deltaTime;

	_Eye.x += xIncr;
	_Eye.y += yIncr;
	_Eye.z += zIncr;

	_MovedThisFrame = true;
}

void FPSCamera::MovePitch(float deltaTime)
{
	_Pitch += _RotationSpeed * deltaTime;
	_MovedThisFrame = true;
}

void FPSCamera::MoveYaw(float deltaTime)
{
	_Yaw += _RotationSpeed * deltaTime;
	_MovedThisFrame = true;
}

void FPSCamera::MoveRoll(float deltaTime)
{
	_Roll += _RotationSpeed * deltaTime;
	_MovedThisFrame = true;
}