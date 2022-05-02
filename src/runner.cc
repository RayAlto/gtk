#include "runner.h"

#include <string>
#include <utility>

#include "gtkmm/application.h"
#include "gtkmm/window.h"

namespace rayalto {
namespace gtk {

Runner::Runner(const std::string& application_id) :
    application_(Gtk::Application::create(application_id)) {}

Runner::Runner(std::string&& application_id) :
    application_(Gtk::Application::create(std::move(application_id))) {}

Runner::Runner() :
    application_(Gtk::Application::create("top.rayalto.gtk.default")) {}

int Runner::run(Gtk::Window& window, int& argc, char* argv[]) {
    application_->signal_startup().connect([&]() -> void {
        application_->add_window(window);
        window.show();
    });
    return application_->run(argc, argv);
}

} // namespace gtk
} // namespace rayalto
