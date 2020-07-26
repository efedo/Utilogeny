

#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/zip/zipiter.h"

std::vector<int> testA;
std::vector<float> testB;

void testFunc() {

	auto zipit = it::zip(testA, testB);

	for (auto [first, second] : zipit) {
		auto what_is_this = (second);
	}
}