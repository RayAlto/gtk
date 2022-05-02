#include <string>

#include "glibmm/refptr.h"
#include "gtkmm/application.h"
#include "gtkmm/window.h"

namespace rayalto {
namespace gtk {

/**
 * A Gtk application runner allow you to create Gtk::Window on you own
 */
class Runner {
public:
    Runner(const std::string& application_id);
    Runner(std::string&& application_id);
    Runner();

    Runner(const Runner&) = default;
    Runner(Runner&&) noexcept = default;
    Runner& operator=(const Runner&) = default;
    Runner& operator=(Runner&&) noexcept = default;

    virtual ~Runner() = default;

    // pass your Gtk::Window and run the application
    int run(Gtk::Window& window, int& argc, char* argv[]);

protected:
    Glib::RefPtr<Gtk::Application> application_;
};

} // namespace gtk
} // namespace rayalto
