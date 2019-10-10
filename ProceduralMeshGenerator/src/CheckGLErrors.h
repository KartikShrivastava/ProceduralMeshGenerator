#pragma once

#pragma once

#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x)   ClearErrors();\
					x;\
					ASSERT(CheckErrors(#x, __FILE__, __LINE__))

void ClearErrors();

bool CheckErrors(const char* function, const char* file, int line);