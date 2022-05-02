#include "window.h"

#include <cstddef>

#include "gtkmm/enums.h"
#include "gtkmm/headerbar.h"
#include "gtkmm/label.h"

#include "image.h"

namespace rayalto {
namespace gtk {

Window::Window() : Gtk::Window() {
    set_title("你是一个一个一个");
    set_default_size(400, 400);

    close_button_.set_label("Close");
    close_button_.signal_clicked().connect([&]() -> void { hide(); });
    Gtk::Label& left_label = *Gtk::make_managed<Gtk::Label>("打个🦶先");
    left_label.set_wrap();

    action_bar_.pack_start(left_label);
    box_.set_orientation(Gtk::Orientation::VERTICAL);
    action_bar_.pack_end(close_button_);

    // =====

    image_.file_name("/home/rayalto/Pictures/gif/97988116.gif")
        .is_animation(true)
        .shape(widget::Image::Shape::ROUNDED);
    box_.append(image_);

    // =====

    box_.append(action_bar_);
    set_child(box_);
    Gtk::HeaderBar& header_bar = *Gtk::make_managed<Gtk::HeaderBar>();
    set_titlebar(header_bar);

    show();
}

Window::~Window() {}

} // namespace gtk
} // namespace rayalto
