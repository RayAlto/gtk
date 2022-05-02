#ifndef RA_GTK_UTIL_H_
#define RA_GTK_UTIL_H_

#include <cstddef>
#include <cstdint>
#include <string>

#include "gdkmm/rgba.h"
#include "glibmm/ustring.h"

namespace rayalto {
namespace gtk {
namespace utils {

/* convert Gdk::RGBA to hex color like #ABCDEFFF */
std::string to_hex_color(const Gdk::RGBA& rgba);

/* calculate relative coordinate value */
double relative_coordinate(int length, double position, double padding);
double relative_coordinate(int length, double position);

/* convert 0-255 to 0.0-1.0 */
constexpr double color2float(std::uint8_t color) {
    return static_cast<double>(color) / 255.0;
}

/* read file content to std::string */
std::string read_text_file(const std::string& filename);

/* read file content to std::vector<unsigned char> */
std::vector<unsigned char> read_binary_file(const std::string& filename);

/* guess content type base on file content */
Glib::ustring content_type_guess(const std::string& filename);
Glib::ustring content_type_guess(const std::vector<unsigned char>& data);
Glib::ustring content_type_guess(const unsigned char* data,
                                 const std::size_t& data_length);
Glib::ustring content_type_guess_gio_resource(const std::string& path);

} // namespace utils
} // namespace gtk
} // namespace rayalto

#endif // RA_GTK_UTIL_H_
