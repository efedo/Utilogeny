#include <cstdint>
#include <type_traits>

template <class T>
struct ColorData {
	T get_max() {
		constexpr if (std::is_floating_point<T>::value) {
			return 1.0f;
		}
		else {
			return std::numeric_limits<T>::max;
		}
	}
};

template <class T>
struct ColorDataRGBA : public ColorData<T> {
	T r;
	T g;
	T b;
	T a;
};

using ColorDataRGBA8 = ColorDataRGBA<uint8_t>;
using ColorDataRGBA16 = ColorDataRGBA<uint16_t>;
using ColorDataRGBA32 = ColorDataRGBA<uint32_t>;
using ColorDataRGBAf = ColorDataRGBA<float>;

template <class T>
struct ColorDataCMYK : public ColorData<T> {
	T c;
	T m;
	T y;
	T k;
};

using ColorDataCMYK8 = ColorDataCMYK<uint8_t>;
using ColorDataCMYK16 = ColorDataCMYK<uint16_t>;
using ColorDataCMYK32 = ColorDataCMYK<uint32_t>;
using ColorDataCMYKf = ColorDataCMYK<float>;

//Color(double c, double m, double y, double k, uint8_t _a)
//	:
//	r(static_cast<uint8_t>(255. * (1. - c) * (1. - k))),
//	g(static_cast<uint8_t>(255. * (1. - m) * (1. - k))),
//	b(static_cast<uint8_t>(255. * (1. - y) * (1. - k))),
//	a(_a)
//{}

