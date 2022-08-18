#ifndef MAFS_UTILS_H
#define MAFS_UTILS_H

#include <type_traits>

inline auto AreEnumsEqual(int lEnum, int rEnum) -> bool { return lEnum == rEnum; }

template <int lEnum, int rEnum> constexpr auto AreEnumsEqual() -> bool { return lEnum == rEnum; }

#endif // MAFS_UTILS_H