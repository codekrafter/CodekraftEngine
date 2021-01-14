#pragma once

#define DISABLE_FATAL_SIGNALHANDLING
#include "g3log/loglevels.hpp"
#include "g3log/logworker.hpp"

const LEVELS LOG_ERROR {WARNING.value +1, {"ERROR"}};

const char *demangle(const char *name);