#include "util.h"

#include <cstddef>
#include <fstream>
#include <ios>
#include <iterator>
#include <string>

#include "gdkmm/rgba.h"
#include "giomm/contenttype.h"
#include "giomm/resource.h"
#include "glibmm/bytes.h"
#include "glibmm/refptr.h"

namespace rayalto {
namespace gtk {
namespace utils {

std::string to_hex_color(const Gdk::RGBA& rgba) {
    char hex_map[] {"0123456789ABCDEF"};
    std::string result;
    result.reserve(9);
    result.push_back('#');
    int channel_value = rgba.get_red() * 255.0;
    result.push_back(hex_map[((channel_value & 0xf0) >> 4)]);
    result.push_back(hex_map[(channel_value & 0x0f)]);
    channel_value = rgba.get_green() * 255.0;
    result.push_back(hex_map[((channel_value & 0xf0) >> 4)]);
    result.push_back(hex_map[(channel_value & 0x0f)]);
    channel_value = rgba.get_blue() * 255.0;
    result.push_back(hex_map[((channel_value & 0xf0) >> 4)]);
    result.push_back(hex_map[(channel_value & 0x0f)]);
    channel_value = rgba.get_alpha() * 255.0;
    result.push_back(hex_map[((channel_value & 0xf0) >> 4)]);
    result.push_back(hex_map[(channel_value & 0x0f)]);
    return result;
}

double relative_coordinate(int length, double position, double padding) {
    return (static_cast<double>(length)
            * ((padding / 2.0) + position * (1.0 - padding)));
}

double relative_coordinate(int length, double position) {
    return (static_cast<double>(length) * position);
}

std::string read_text_file(const std::string& filename) {
    std::ifstream file_stream(filename);
    std::streampos file_length;
    file_stream.seekg(0, std::ios::end);
    file_length = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);
    std::string result;
    result.reserve(file_length);
    result.insert(result.begin(),
                  std::istream_iterator<char>(file_stream),
                  std::istream_iterator<char>());
    return result;
}

std::vector<unsigned char> read_binary_file(const std::string& filename) {
    std::ifstream file_stream(filename, std::ios::binary);
    file_stream.unsetf(std::ios::skipws);
    std::streampos file_length;
    file_stream.seekg(0, std::ios::end);
    file_length = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);
    std::vector<unsigned char> result;
    result.reserve(file_length);
    result.insert(result.begin(),
                  std::istream_iterator<unsigned char>(file_stream),
                  std::istream_iterator<unsigned char>());
    return result;
}

Glib::ustring content_type_guess(const std::string& filename) {
    std::vector<unsigned char> data = read_binary_file(filename);
    return content_type_guess(data);
}

Glib::ustring content_type_guess(const std::vector<unsigned char>& data) {
    return content_type_guess(data.data(), data.size());
}

Glib::ustring content_type_guess(const unsigned char* data,
                                 const std::size_t& data_length) {
    bool certain {false};
    return Gio::content_type_guess(std::string {}, data, data_length, certain);
}

Glib::ustring content_type_guess_gio_resource(const std::string& path) {
    Glib::RefPtr<const Glib::Bytes> data_bytes =
        Gio::Resource::lookup_data_global(path);
    std::size_t data_length;
    const unsigned char* data = reinterpret_cast<const unsigned char*>(
        data_bytes->get_data(data_length));
    return content_type_guess(data, data_length);
}

} // namespace utils
} // namespace gtk
} // namespace rayalto
