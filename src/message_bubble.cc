#include "message_bubble.h"

#include <cmath> // std::sqrt
#include <utility>

#include "cairomm/context.h"
#include "cairomm/refptr.h"
#include "gdkmm/rectangle.h"
#include "gtkmm/enums.h"
#include "gtkmm/snapshot.h"
#include "gtkmm/widget.h"
#include "pangomm/layout.h"

#include "util.h"

namespace rayalto {
namespace gtk {
namespace widget {

constexpr double circle_curve_point(const int& times = 1) {
    return static_cast<double>(times) * ((4.0 * (std::sqrt(2) - 1.0)) / 3.0);
}

MessageBubble::MessageBubble() :
    Glib::ObjectBase("MessageBubble"), Gtk::Widget(), gtk_label() {
    gtk_label.set_wrap();
    gtk_label.set_wrap_mode(Pango::WrapMode::WORD_CHAR);
    gtk_label.set_parent(*this);
}

MessageBubble::MessageBubble(const Side& side) :
    Glib::ObjectBase("MessageBubble"),
    Gtk::Widget(),
    gtk_label(),
    from_side_(side) {}

MessageBubble::MessageBubble(const Glib::ustring& text) :
    Glib::ObjectBase("MessageBubble"), Gtk::Widget(), gtk_label(text) {}

MessageBubble::MessageBubble(const Glib::ustring& text, const Side& side) :
    Glib::ObjectBase("MessageBubble"),
    Gtk::Widget(),
    gtk_label(text),
    from_side_(side) {}

MessageBubble::~MessageBubble() {
    gtk_label.unparent();
}

Gtk::SizeRequestMode MessageBubble::get_request_mode_vfunc() const {
    return gtk_label.get_request_mode();
}

void MessageBubble::measure_vfunc(Gtk::Orientation orientation,
                                  int for_size,
                                  int& minimum,
                                  int& natural,
                                  int& minimum_baseline,
                                  int& natural_baseline) const {
    minimum_baseline = -1;
    natural_baseline = -1;
    int dummy_minimum_baseline = 0;
    int dummy_natural_baseline = 0;
    if (orientation == Gtk::Orientation::HORIZONTAL) {
        int label_minimum_width = 0;
        int lable_natural_width = 0;
        gtk_label.measure(orientation,
                          for_size,
                          label_minimum_width,
                          lable_natural_width,
                          dummy_minimum_baseline,
                          dummy_natural_baseline);
        // add space to draw border
        minimum = label_minimum_width + 20;
        natural = lable_natural_width + 20;
    }
    else if (orientation == Gtk::Orientation::VERTICAL) {
        int label_minimum_height = 0;
        int label_natural_height = 0;
        gtk_label.measure(orientation,
                          for_size,
                          label_minimum_height,
                          label_natural_height,
                          dummy_minimum_baseline,
                          dummy_natural_baseline);
        // add space to draw border
        minimum = label_minimum_height + 10;
        natural = label_natural_height + 10;
    }
}

void MessageBubble::size_allocate_vfunc(int width, int height, int baseline) {
    Gtk::Allocation label_allocation;
    if (from_side_ == Side::LEFT) {
        label_allocation.set_x(15);
    }
    else if (from_side_ == Side::RIGHT) {
        label_allocation.set_x(5);
    }
    label_allocation.set_y(5);
    label_allocation.set_width(width - 20);
    label_allocation.set_height(height - 10);
    gtk_label.size_allocate(label_allocation, baseline);
}

void MessageBubble::snapshot_vfunc(
    const Glib::RefPtr<Gtk::Snapshot>& snapshot) {
    const double curve_point = circle_curve_point(10);

    // create cairo context
    const Gtk::Allocation& allocation = get_allocation();
    const int& canvas_width = allocation.get_width();
    const int& canvas_height = allocation.get_height();
    const Gdk::Rectangle canvas(0, 0, canvas_width, canvas_height);
    Cairo::RefPtr<Cairo::Context> context = snapshot->append_cairo(canvas);

    // set color
    context->set_source_rgba(rgba_.get_red(),
                             rgba_.get_green(),
                             rgba_.get_blue(),
                             rgba_.get_alpha());

    // draw border
    if (from_side_ == Side::LEFT) {
        context->move_to(10, 10);
        context->curve_to(10, 10 - curve_point, 20 - curve_point, 0, 20, 0);
        context->line_to(canvas_width - 10, 0);
        context->curve_to(canvas_width - 10 + curve_point,
                          0,
                          canvas_width,
                          10 - curve_point,
                          canvas_width,
                          10);
        context->line_to(canvas_width, canvas_height - 10);
        context->curve_to(canvas_width,
                          canvas_height - 10 + curve_point,
                          canvas_width - 10 + curve_point,
                          canvas_height,
                          canvas_width - 10,
                          canvas_height);
        context->line_to(0, canvas_height);
        context->line_to(10, canvas_height - 10);
        context->line_to(10, 10);
    }
    else if (from_side_ == Side::RIGHT) {
        context->move_to(0, 10);
        context->curve_to(0, 10 - curve_point, 10 - curve_point, 0, 10, 0);
        context->line_to(canvas_width - 20, 0);
        context->curve_to(canvas_width - 20 + curve_point,
                          0,
                          canvas_width - 10,
                          10 - curve_point,
                          canvas_width - 10,
                          10);
        context->line_to(canvas_width - 10, canvas_height - 10);
        context->line_to(canvas_width, canvas_height);
        context->line_to(10, canvas_height);
        context->curve_to(10 - curve_point,
                          canvas_height,
                          0,
                          canvas_height - 10 + curve_point,
                          0,
                          canvas_height - 10);
        context->line_to(0, 10);
    }
    context->close_path();
    context->fill();

    // draw label
    Gtk::Widget::snapshot_vfunc(snapshot);
}

const MessageBubble::Side& MessageBubble::from_side() const {
    return from_side_;
}

MessageBubble::Side& MessageBubble::from_side() {
    return from_side_;
}

MessageBubble& MessageBubble::from_side(const Side& side) {
    from_side_ = side;
    queue_draw();
    return *this;
}

MessageBubble& MessageBubble::from_side(Side&& side) {
    from_side_ = std::move(side);
    queue_draw();
    return *this;
}

const Gdk::RGBA& MessageBubble::background_color() const {
    return rgba_;
}

Gdk::RGBA& MessageBubble::background_color() {
    return rgba_;
}

MessageBubble& MessageBubble::background_color(const Gdk::RGBA& rgba) {
    rgba_ = rgba;
    queue_draw();
    return *this;
}

MessageBubble& MessageBubble::background_color(Gdk::RGBA&& rgba) {
    rgba_ = std::move(rgba);
    queue_draw();
    return *this;
}

} // namespace widget
} // namespace gtk
} // namespace rayalto
