#pragma once
#include "../InputID.h"

struct Header
{
	float ver;
	size_t size;
	int sum;
};

struct Data
{
	ControllerInputID id;
	int code;

};