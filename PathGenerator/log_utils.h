#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef enum level
{
	error = 'x',
	info = '!',
	none = '+',
}Level;

void LogConsole(const char* message, Level logLevel)
{
	printf("\n[%c] %s\n", logLevel, message);
}