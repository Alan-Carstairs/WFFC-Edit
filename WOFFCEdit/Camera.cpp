#include "Camera.h"

Camera::Camera(std::string id)
{
	_ID = id;

	_MovementSpeed = 5.3f;
	_RotationSpeed = 3.0f;

	Reset();
}


Camera::~Camera()
{

}

void Camera::Reset()
{
	_Eye = Vector3(0.0f, 3.7f, -3.5f);
	_LookAt = Vector3(0.0f, 0.0f, 0.0f);
	_Up = Vector3(0.0f, 1.0f, 0.0f);
	_Right = Vector3(0.0f, 0.0f, 0.0f);
	_Forward = Vector3(0.0f, 0.0f, 0.0f);

	_FOV = 70.0f;
	_AspectRatio = 1.77f;
	_NearPlane = 0.01f;
	_FarPlane = 1000.0f;

	_Pitch = 107.0f;
	_Yaw = 0.0f;
	_Roll = 0.0f;
}

void Camera::OnWindowSizeChanged(RECT newSize)
{
	_AspectRatio = float(newSize.right) / float(newSize.bottom);
	_FOV = 70.0f;

	if (_AspectRatio < 1.0f)
	{
		_FOV *= 2.0f;
	}

	_ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(_FOV), _AspectRatio, _NearPlane, _FarPlane);
}