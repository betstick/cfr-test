#pragma once

#include <assert.h>
#include <stdio.h>
#include <cstdint>
#include <stdint.h>
#include <string>

namespace cfr
{
	//prints message and exits if result is 0
	void assertMsg(int result, std::string);
};