#include <boost/cstdint.hpp>

constexpr boost::uint16_t PORT = 5555;

enum class Requests {
    Registration,
    ViewBalance,
    AddOrder
};
