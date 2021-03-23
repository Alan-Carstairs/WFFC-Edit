#include "Camera.h"

class FPSCamera : public Camera 
{
public:
	FPSCamera(std::string id);
	~FPSCamera();

	void Reset() override;
	void HandleInput(InputCommands input, float deltaTime, float w, float h) override;
	void Update(float deltaTime) override;

	void TryFocus(SceneObject obj) override;

	bool IsMouseControlEnabled() { return _MouseControlEnabled; }

private:
	void MoveForward(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void MovePitch(float deltaTime);
	void MoveYaw(float deltaTime);
	void MoveRoll(float deltaTime);

	bool _MouseControlEnabled;
	float _MouseControlSpeed;
	Vector2 mousePos;
};

