#include "red_dot.h"

#include <algorithm>
#include <string>
#include <utility>
#include "cairomm/context.h"
#include "cairomm/refptr.h"
#include "constants.h"
#include "gdkmm/rectangle.h"
#include "gdkmm/rgba.h"
#include "glibmm/objectbase.h"
#include "gtkmm/enums.h"
#include "gtkmm/widget.h"
#include "pangomm/font.h"

namespace rayalto {
namespace gtk {
namespace widget {

RedDot::RedDot() : Glib::ObjectBase("RedDot"), Gtk::Widget() {
    update_number_();
}

RedDot::RedDot(const std::uint8_t& number) :
    Glib::ObjectBase("RedDot"), Gtk::Widget(), number_(number) {
    update_number_();
}

RedDot::RedDot(const Gdk::RGBA& rgba) :
    Glib::ObjectBase("RedDot"), Gtk::Widget(), background_color_(rgba) {
    update_number_();
}

RedDot::RedDot(const std::uint8_t& number, const Gdk::RGBA& rgba) :
    Glib::ObjectBase("RedDot"),
    Gtk::Widget(),
    number_(number),
    background_color_(rgba) {
    update_number_();
}

void RedDot::measure_vfunc(Gtk::Orientation orientation,
                           int for_size,
                           int& minimum,
                           int& natural,
                           int& minimum_baseline,
                           int& natural_baseline) const {
    // no baseline
    minimum_baseline = -1;
    natural_baseline = -1;

    if (text_layout_ == nullptr) {
        minimum = 20;
        natural = 20;
        return;
    }

    // get a square
    int diameter = std::max(text_width_, text_height_) + 20;
    minimum = diameter;
    natural = diameter;
}

void RedDot::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) {
    //create cairo context
    const Gtk::Allocation& allocation = get_allocation();
    const int diameter =
        std::min(allocation.get_width(), allocation.get_height());
    const Gdk::Rectangle canvas(0, 0, diameter, diameter);
    Cairo::RefPtr<Cairo::Context> context = snapshot->append_cairo(canvas);

    // set background color
    context->set_source_rgba(background_color_.get_red(),
                             background_color_.get_green(),
                             background_color_.get_blue(),
                             background_color_.get_alpha());

    // draw circle
    const double radius = static_cast<double>(diameter) / 2.0;
    context->arc(radius, radius, radius, 0, constants::pi(2));
    context->fill();

    // draw label
    context->set_source_rgba(text_color_.get_red(),
                             text_color_.get_green(),
                             text_color_.get_blue(),
                             text_color_.get_alpha());
    context->move_to(static_cast<double>(diameter - text_width_) / 2.0,
                     static_cast<double>(diameter - text_height_) / 2.0);
    text_layout_->show_in_cairo_context(context);
}

const std::uint8_t& RedDot::number() const {
    return number_;
}

RedDot& RedDot::number(const std::uint8_t& number) {
    number_ = number;
    update_number_();
    queue_draw();
    return *this;
}

RedDot& RedDot::number(std::uint8_t&& number) {
    number_ = std::move(number);
    update_number_();
    queue_draw();
    return *this;
}

const Gdk::RGBA& RedDot::background_color() const {
    return background_color_;
}

RedDot& RedDot::background_color(const Gdk::RGBA& rgba) {
    background_color_ = rgba;
    queue_draw();
    return *this;
}

RedDot& RedDot::background_color(Gdk::RGBA&& rgba) {
    background_color_ = std::move(rgba);
    queue_draw();
    return *this;
}

const Gdk::RGBA& RedDot::text_color() const {
    return text_color_;
}

RedDot& RedDot::text_color(const Gdk::RGBA& rgba) {
    text_color_ = rgba;
    queue_draw();
    return *this;
}

RedDot& RedDot::text_color(Gdk::RGBA&& rgba) {
    text_color_ = std::move(rgba);
    queue_draw();
    return *this;
}

int RedDot::font_size() const {
    return font_.get_size();
}

RedDot& RedDot::font_size(int size) {
    font_.set_size(size * Pango::SCALE);
    update_font_();
    queue_resize();
    queue_draw();
    return *this;
}

std::string RedDot::font_family() const {
    return font_.get_family();
}

RedDot& RedDot::font_family(std::string family) {
    font_.set_family(family);
    update_font_();
    queue_resize();
    queue_draw();
    return *this;
}

Pango::Style RedDot::font_style() const {
    return font_.get_style();
}

RedDot& RedDot::font_style(Pango::Style style) {
    font_.set_style(style);
    update_font_();
    queue_resize();
    queue_draw();
    return *this;
}

void RedDot::update_number_() {
    number_text_ = (number_ > 99 ? "99+" : std::to_string(number_));
    update_font_();
}

void RedDot::update_font_() {
    text_layout_ = create_pango_layout(number_text_);
    text_layout_->set_font_description(font_);
    text_layout_->get_pixel_size(text_width_, text_height_);
}

} // namespace widget
} // namespace gtk
} // namespace rayalto
