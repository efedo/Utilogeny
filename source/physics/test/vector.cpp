#include "precomp.h"
#include "Utilogeny/source/physics/lib/vector/vector.h"

TEST(Physics, Vector) {

	auto vec2 = Vec2D();
	auto vec3 = Vec3D();
	auto vec4 = Vec4D();

	vec3.x() = 3.4;
	vec3.y() = 9.1;
	vec3.z() = 1.2;

	vec2.x() = 1.2;
	vec2.y() = 0.6;

	auto vecadd = vec3 + vec2;
	auto vecsub = vec3 - vec2;
	auto vecunineg = -vec3;
	auto listconst = vec3 + Vec3D{4.5, 4.3, 2.3};
	auto vecmul = vec2 * 4.5;
	auto vecdiv = vec3 / 5.3;
}
