#ifndef RA_GTK_CONSTANTS_H_
#define RA_GTK_CONSTANTS_H_

#include <cmath>

namespace rayalto {
namespace gtk {
namespace constants {

/* no fucking macros */
constexpr double pi() {
    return std::atan(1) * 4.0;
}
constexpr double pi(double times) {
    return times * pi();
}

} // namespace constants
} // namespace gtk
} // namespace rayalto

#endif // RA_GTK_CONSTANTS_H_
