#pragma once

#ifdef SHOULD_EXPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif