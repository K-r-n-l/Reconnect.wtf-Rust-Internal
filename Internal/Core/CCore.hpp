#pragma once

#include "Globals/Globals.hpp"
#include "Utils/Utils.hpp"

class CCore {
public:
	static auto CreateConsole() -> bool;
	static auto Init() -> bool;
};