#pragma once
#include "pch.h"
#include "CorePch.h"
#define CRASH(cause)					\
{										\
	uint32* crash = nullptr;			\
	__analysis_assume(crash != nullptr);\
	*crash = 0xEADDAABA;				\
}

#define ASSERT_CRASH(expr)				 \
{\
	if (!(expr))						\
	{									\
		CRASH("ASSERT CRASH");			\
		__analysis_assume(expr);		\
	}									\
}