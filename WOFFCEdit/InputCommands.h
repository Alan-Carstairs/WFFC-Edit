#pragma once

struct InputCommands
{
	bool up;
	bool down;
	bool forward;
	bool back;
	bool right;
	bool left;
	bool rotRight;
	bool rotLeft;
	bool rotUp;
	bool rotDown;

	//Mouse
	int mouse_X;
	int mouse_Y;
	bool mouse_LB_Down;
	bool mouse_RB_Down;
};
