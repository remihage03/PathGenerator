#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef enum level
{
	error,
	warning,
	info,
	none,
}Level;

void LogConsole(const char* message, )