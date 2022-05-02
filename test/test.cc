#include <iostream>

#include "gdkmm/display.h"
#include "giomm/file.h"
#include "glibmm/error.h"
#include "glibmm/refptr.h"
#include "gtkmm/csslocation.h"
#include "gtkmm/cssprovider.h"
#include "gtkmm/csssection.h"
#include "gtkmm/settings.h"
#include "gtkmm/stylecontext.h"

#include "runner.h"
#include "window.h"

int main(int argc, char* argv[]) {
    rayalto::gtk::Runner runner("top.rayalto.gtkmm.test");
    rayalto::gtk::Window window;

    Glib::RefPtr<Gtk::Settings> settings = Gtk::Settings::get_default();
    settings->property_gtk_decoration_layout().set_value("menu:close");

    Glib::RefPtr<Gtk::StyleContext> style_context = window.get_style_context();
    Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
    css_provider->load_from_resource(
        "/top/rayalto/gtk/css/material-dark.theme.css");
    style_context->add_provider_for_display(
        Gdk::Display::get_default(),
        css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    css_provider->signal_parsing_error().connect(
        [](const Glib::RefPtr<const Gtk::CssSection>& section,
           const Glib::Error& error) -> void {
            std::cerr << "on_parsing_error(): " << error.what() << std::endl;
            if (section) {
                const Glib::RefPtr<const Gio::File>& file = section->get_file();
                if (file) {
                    std::cerr << "  URI = " << file->get_uri() << std::endl;
                }

                Gtk::CssLocation start_location = section->get_start_location();
                Gtk::CssLocation end_location = section->get_end_location();
                std::cerr << "  start_line = " << start_location.get_lines() + 1
                          << ", end_line = " << end_location.get_lines() + 1
                          << std::endl;
                std::cerr << "  start_position = "
                          << start_location.get_line_chars()
                          << ", end_position = "
                          << end_location.get_line_chars() << std::endl;
            }
        });

    runner.run(window, argc, argv);
    return 0;
}
