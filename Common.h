#pragma once
#include <stdint.h>
#include <vector>
#include <iostream>
#include <string>


#define REFL_MAX UINT16_MAX
#define ROUGH_MAX UINT16_MAX
#define POWER_MAX UINT16_MAX

//camera and screen
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;
const float CAMERA_DISTANCE = 25.0f;
const float CAMERA_ANGLE_X = 45.0f;
const float CAMERA_ANGLE_Y = -45.0f;
const float CAMERA_XY_MOVE_MULTIPLIER = 0.5f;

const float FOV = 60.0f;
const int RT_RENDER_RES = 64;
const int COLOR_DEPTH = 256;

//math consts
const float DEG2RAD = 3.141593f / 180.0f;
const float RAD2DEG = 180.0f / 3.141593f;
const float EPSILON = 0.00001f;

//GL consts
const int RAY_LEN = 20;

//render consts
const int MAX_RAY_COUNT = 10;
const int MAX_OBJ_COUNT = 10;
const int MAX_LGH_COUNT = 10;

const int DEMO_CASTED_RAYS_COUNT = 153;

// ERROR
enum ERR_TYP {
	OK,
	InvalidValues,
	OtherError
};

void raiseErr(std::string msg);
void raiseErr(ERR_TYP err);

uint8_t log2(uint32_t value);