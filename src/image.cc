#include "image.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>

#include "cairomm/context.h"
#include "cairomm/refptr.h"
#include "gdkmm/general.h"
#include "gdkmm/pixbuf.h"
#include "gdkmm/pixbufanimation.h"
#include "gdkmm/pixbufloader.h"
#include "gdkmm/rectangle.h"
#include "giomm/resource.h"
#include "glibmm/bytes.h"
#include "glibmm/error.h" // Glib::Error
#include "glibmm/fileutils.h" // Glib::FileError
#include "glibmm/main.h" // Glib::signal_timeout
#include "glibmm/objectbase.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "gtkmm/snapshot.h"
#include "gtkmm/widget.h"
#include "sigc++/functors/mem_fun.h"

#include "constants.h"
#include "util.h"

namespace rayalto {
namespace gtk {
namespace widget {

Image::Image() : Glib::ObjectBase("Image"), Gtk::Widget() {
    set_expand();
}

void Image::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) {
    if (pixbuf_ == nullptr) {
        return;
    }

    const Gtk::Allocation& allocation = get_allocation();
    const int& canvas_width = allocation.get_width();
    const int& canvas_height = allocation.get_height();

    // prevent some assertion failure
    if (canvas_width < 2 || canvas_height < 2) {
        return;
    }

    const double canvas_shorter_side =
        static_cast<double>(std::min(canvas_width, canvas_height));

    // scale pixbuf to fit canvas
    const int& pixbuf_width = pixbuf_->get_width();
    const int& pixbuf_height = pixbuf_->get_height();
    const double pixbuf_scale_factor = std::min(
        (static_cast<double>(canvas_width) / static_cast<double>(pixbuf_width)),
        (static_cast<double>(canvas_height)
         / static_cast<double>(pixbuf_height)));
    const double scaled_pixbuf_width =
        static_cast<double>(pixbuf_width) * pixbuf_scale_factor;
    const double scaled_pixbuf_height =
        static_cast<double>(pixbuf_height) * pixbuf_scale_factor;
    Glib::RefPtr<Gdk::Pixbuf> scaled_pixbuf;
    if (is_svg_) {
        scaled_pixbuf = scale_svg_(scaled_pixbuf_width, scaled_pixbuf_height);
    }
    else {
        scaled_pixbuf = pixbuf_->scale_simple(scaled_pixbuf_width,
                                              scaled_pixbuf_height,
                                              Gdk::InterpType::BILINEAR);
    }

    // pixbuf coordinate
    const double pixbuf_x =
        static_cast<double>(canvas_width - scaled_pixbuf_width) / 2.0;
    const double pixbuf_y =
        static_cast<double>(canvas_height - scaled_pixbuf_height) / 2.0;

    // create cairo context
    const Gdk::Rectangle canvas(0, 0, canvas_width, canvas_height);
    Cairo::RefPtr<Cairo::Context> context = snapshot->append_cairo(canvas);

    // put pixbuf in cairo context
    Gdk::Cairo::set_source_pixbuf(context, scaled_pixbuf, pixbuf_x, pixbuf_y);

    // draw pixbuf
    switch (shape_) {
    case Shape::CIRCLE: {
        double radius = canvas_shorter_side / 2.0;
        context->arc(static_cast<double>(canvas_width) / 2.0,
                     static_cast<double>(canvas_height) / 2.0,
                     radius,
                     0,
                     constants::pi(2.0));
        context->fill();
        break;
    }
    case Shape::ROUNDED: {
        double curve_point = canvas_shorter_side / 2.0;
        context->move_to(0, curve_point);
        context->curve_to(0, 0, 0, 0, curve_point, 0);
        context->line_to(canvas_width - curve_point, 0);
        context->curve_to(
            canvas_width, 0, canvas_width, 0, canvas_width, curve_point);
        context->line_to(canvas_width, canvas_height - curve_point);
        context->curve_to(canvas_width,
                          canvas_height,
                          canvas_width,
                          canvas_height,
                          canvas_width - curve_point,
                          canvas_height);
        context->line_to(curve_point, canvas_height);
        context->curve_to(
            0, canvas_height, 0, canvas_height, 0, canvas_height - curve_point);
        context->line_to(0, curve_point);
        context->close_path();
        context->fill();
        break;
    }
    case Shape::RECTANGLE: {
        context->rectangle(0, 0, canvas_width, canvas_height);
        context->fill();
        break;
    }
    default: break;
    }

    // load next frame and queue draw
    if (is_animation_ && pixbuf_animation_iter_ != nullptr) {
        pixbuf_animation_iter_->advance();
        pixbuf_ = pixbuf_animation_iter_->get_pixbuf();
        Glib::signal_timeout().connect_once(
            sigc::mem_fun(*this, &Image::queue_draw),
            pixbuf_animation_iter_->get_delay_time());
    }
}

std::string& Image::resource_path() {
    return source_;
}

const std::string& Image::resource_path() const {
    return source_;
}

Image& Image::resource_path(const std::string& resource_path) {
    source_ = resource_path;
    is_gio_resource_ = true;
    determine_svg_();
    refresh_source_();
    return *this;
}

Image& Image::resource_path(std::string&& resource_path) {
    source_ = resource_path;
    is_gio_resource_ = true;
    determine_svg_();
    refresh_source_();
    return *this;
}

std::string& Image::file_name() {
    return source_;
}

const std::string& Image::file_name() const {
    return source_;
}

Image& Image::file_name(const std::string& file_name) {
    source_ = file_name;
    is_gio_resource_ = false;
    determine_svg_();
    refresh_source_();
    return *this;
}

Image& Image::file_name(std::string&& file_name) {
    source_ = std::move(file_name);
    is_gio_resource_ = false;
    determine_svg_();
    refresh_source_();
    return *this;
}

Image::Shape& Image::shape() {
    return shape_;
}

const Image::Shape& Image::shape() const {
    return shape_;
}

Image& Image::shape(const Shape& shape) {
    shape_ = shape;
    queue_draw();
    return *this;
}

Image& Image::shape(Shape&& shape) {
    shape_ = std::move(shape);
    queue_draw();
    return *this;
}

bool& Image::is_animation() {
    return is_animation_;
}

const bool& Image::is_animation() const {
    return is_animation_;
}

Image& Image::is_animation(const bool& is_animation) {
    is_animation_ = is_animation;
    refresh_source_();
    queue_draw();
    return *this;
}

Image& Image::is_animation(bool&& is_animation) {
    is_animation_ = std::move(is_animation);
    refresh_source_();
    queue_draw();
    return *this;
}

void Image::determine_svg_() {
    if (is_gio_resource_) {
        // read from resource
        Glib::RefPtr<const Glib::Bytes> data_bytes =
            Gio::Resource::lookup_data_global(source_);
        gio_resource_image_data_ = reinterpret_cast<const unsigned char*>(
            data_bytes->get_data(gio_resource_image_data_length_));
        if (utils::content_type_guess(gio_resource_image_data_,
                                      gio_resource_image_data_length_)
                .find("image/svg")
            != Glib::ustring::npos) {
            // "image/svg" in mime type, regard as svg
            is_svg_ = true;
        }
        else {
            // not a svg
            is_svg_ = false;
            gio_resource_image_data_ = nullptr;
            gio_resource_image_data_length_ = 0;
        }
    }
    else {
        // read from file
        image_data_ = std::make_unique<std::vector<unsigned char>>(
            utils::read_binary_file(source_));
        if (utils::content_type_guess(*image_data_).find("image/svg")
            != Glib::ustring::npos) {
            // "image/svg" in mime type, regard as svg
            is_svg_ = true;
        }
        else {
            // not a svg
            is_svg_ = false;
            image_data_.reset();
        }
    }
}

void Image::refresh_source_() {
    if (is_animation_) {
        refresh_pixbuf_animation_();
    }
    else {
        refresh_pixbuf_();
    }
}

void Image::refresh_pixbuf_() {
    try {
        pixbuf_ = is_gio_resource_ ? Gdk::Pixbuf::create_from_resource(source_)
                                   : Gdk::Pixbuf::create_from_file(source_);
        queue_draw();
    }
    catch (Glib::FileError& err) {
        std::cerr << "Failed to read image \"" << source_ << "\": ("
                  << err.what() << "), ignore it.\n";
    }
    catch (Gio::ResourceError& err) {
        std::cerr << "Failed to load resource \"" << source_ << "\": ("
                  << err.what() << "), ignore it.\n";
    }
    catch (Gdk::PixbufError& err) {
        std::cerr << "Failed to create pixbuf from source \"" << source_
                  << "\": (" << err.what() << "), ignore it.\n";
    }
}

void Image::refresh_pixbuf_animation_() {
    try {
        pixbuf_animation_ =
            is_gio_resource_
                ? Gdk::PixbufAnimation::create_from_resource(source_)
                : Gdk::PixbufAnimation::create_from_file(source_);
        pixbuf_animation_iter_ = pixbuf_animation_->get_iter();
        pixbuf_ = pixbuf_animation_iter_->get_pixbuf();
    }
    catch (Glib::Error& err) {
        std::cerr << "Failed to create pixbuf animation from source \""
                  << source_ << "\": (" << err.what() << "), ignore it.\n";
    }
}

Glib::RefPtr<Gdk::Pixbuf> Image::scale_svg_(const int& width,
                                            const int& height) {
    pixbuf_loader_ = Gdk::PixbufLoader::create();
    pixbuf_loader_->set_size(width, height);
    try {
        if (is_gio_resource_) {
            pixbuf_loader_->write(gio_resource_image_data_,
                                  gio_resource_image_data_length_);
        }
        else {
            pixbuf_loader_->write(image_data_->data(), image_data_->size());
        }
        pixbuf_loader_->close();
    }
    catch (Gdk::PixbufError& err) {
        std::cerr << "Failed to create pixbuf from source \"" << source_
                  << "\": (" << err.what() << "), ignore it.\n";
    }
    catch (Glib::FileError& err) {
        std::cerr << "Failed to create pixbuf from source \"" << source_
                  << "\": (" << err.what() << "), ignore it.\n";
    }
    return pixbuf_loader_->get_pixbuf();
}

} // namespace widget
} // namespace gtk
} // namespace rayalto
