#ifndef RA_GTK_WINDOW_H_
#define RA_GTK_WINDOW_H_

#include "gtkmm/actionbar.h"
#include "gtkmm/box.h"
#include "gtkmm/button.h"
#include "gtkmm/drawingarea.h"
#include "gtkmm/window.h"

#include "image.h"
#include "message_bubble.h"

namespace rayalto {
namespace gtk {

class Window : public Gtk::Window {
public:
    Window();
    Window(const Window&) = delete;
    Window(Window&&) noexcept = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) noexcept = delete;

    ~Window() override;

protected:
    Gtk::Box box_;
    Gtk::ActionBar action_bar_;
    Gtk::Button close_button_;
    widget::Image image_;
};

} // namespace gtk
} // namespace rayalto

#endif // RA_GTK_WINDOW_H_
