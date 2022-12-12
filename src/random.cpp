
#include "random.hpp"

#include <numbers>
#include <openssl/rand.h>
#include <cmath>

double Random::num() noexcept
{
	uint64_t num;
	RAND_bytes((unsigned char*)&num, 8);

	int exp;
	double base = frexp(num, &exp);

	return ldexp(base, exp - 64);
}

Vector Random::vector() noexcept
{
	return Vector::from_radius(num() * M_PI * 2);
}

