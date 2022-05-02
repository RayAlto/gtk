#ifndef RA_GTK_IMAGE_H_
#define RA_GTK_IMAGE_H_

#include <cstddef>
#include <cstdint>
#include <memory>

#include "gdkmm/pixbuf.h"
#include "gdkmm/pixbufanimation.h"
#include "gdkmm/pixbufloader.h"
#include "glibmm/refptr.h"
#include "gtkmm/snapshot.h"
#include "gtkmm/widget.h"

namespace rayalto {
namespace gtk {
namespace widget {

/**
 * Display images like Gtk::Picture, but:
 *   - Has three shape: circle, rounded, ractangle
 *   - Scale on svg image is lossless
 *   - Animation images are automatically played
 *   - ...
 */
class Image : public Gtk::Widget {
public:
    enum class Shape : std::uint8_t { CIRCLE, ROUNDED, RECTANGLE };

public:
    Image();
    Image(const Image&) = delete;
    Image(Image&&) noexcept = default;
    Image& operator=(const Image&) = delete;
    Image& operator=(Image&&) noexcept = default;

    virtual ~Image() = default;

    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) override;

    // get resource path of image
    std::string& resource_path();
    const std::string& resource_path() const;
    // load image from resource
    Image& resource_path(const std::string& resource_path);
    Image& resource_path(std::string&& resource_path);

    // get file name of image
    std::string& file_name();
    const std::string& file_name() const;
    // load image from file
    Image& file_name(const std::string& file_name);
    Image& file_name(std::string&& file_name);

    // get display shape
    Shape& shape();
    const Shape& shape() const;
    // set display shape
    Image& shape(const Shape& shape);
    Image& shape(Shape&& shape);

    // whether imagae is set animated
    bool& is_animation();
    const bool& is_animation() const;
    // set whether image is animated (necessary for images like gif)
    Image& is_animation(const bool& is_animation);
    Image& is_animation(bool&& is_animation);

protected:
    bool is_animation_ {false};
    bool is_gio_resource_ {false};
    bool is_svg_ {false};
    Shape shape_ {Shape::RECTANGLE};
    std::string source_;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf_;
    Glib::RefPtr<Gdk::PixbufAnimation> pixbuf_animation_;
    Glib::RefPtr<Gdk::PixbufAnimationIter> pixbuf_animation_iter_;
    Glib::RefPtr<Gdk::PixbufLoader> pixbuf_loader_;
    std::unique_ptr<std::vector<unsigned char>> image_data_;
    const unsigned char* gio_resource_image_data_ {nullptr};
    std::size_t gio_resource_image_data_length_;

    // whether source is svg image (base on file content)
    void determine_svg_();

    // reload pixbuf
    void refresh_source_();
    void refresh_pixbuf_();
    void refresh_pixbuf_animation_();

    // reload the svg to a specified size
    Glib::RefPtr<Gdk::Pixbuf> scale_svg_(const int& width, const int& height);
};

} // namespace widget
} // namespace gtk
} // namespace rayalto

#endif // RA_GTK_IMAGE_H_
