#ifndef LAZ_COMMON_HPP
#define LAZ_COMMON_HPP

#include <cstdint>

#include <SFML/System/Vector2.hpp>

namespace laz
{

// unsigned integers
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// signed integers
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// floating point
using f32 = float;
using f64 = double;

// SFML vectors
using v2u = sf::Vector2u;
using v2i = sf::Vector2i;
using v2f = sf::Vector2f;

}

#endif
