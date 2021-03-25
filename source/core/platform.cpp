#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/platform.h"

constexpr int _ce_strcmp_internal(const char* a, const char* b)
{
	return *a == 0 && *b == 0 ? 0 :
		*a == 0 ? -1 :
		*b == 0 ? 1 :
		*a < *b ? -1 :
		*a > *b ? 1 :
		*a == *b ? _ce_strcmp_internal(a + 1, b + 1) :
		throw;
}

constexpr int ce_strcmp(const char* a, const char* b)
{
	return (_ce_strcmp_internal(a, b) == 0);
}