#ifndef RA_GTK_RED_DOT_H_
#define RA_GTK_RED_DOT_H_

#include <cstdint>
#include <string>

#include "gdkmm/rgba.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "gtkmm/enums.h"
#include "gtkmm/snapshot.h"
#include "gtkmm/widget.h"
#include "pangomm/fontdescription.h"
#include "pangomm/layout.h"

namespace rayalto {
namespace gtk {
namespace widget {

class RedDot : public Gtk::Widget {
public:
    RedDot();
    RedDot(const std::uint8_t& number);
    RedDot(const Gdk::RGBA& rgba);
    RedDot(const std::uint8_t& number, const Gdk::RGBA& rgba);
    RedDot(const RedDot&) = delete;
    RedDot(RedDot&&) noexcept = default;
    RedDot& operator=(const RedDot&) = delete;
    RedDot& operator=(RedDot&&) noexcept = default;

    virtual ~RedDot() = default;

    void measure_vfunc(Gtk::Orientation orientation,
                       int for_size,
                       int& minimum,
                       int& natural,
                       int& minimum_baseline,
                       int& natural_baseline) const override;
    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) override;

    // get number displayed by this red dot
    const std::uint8_t& number() const;
    // set number
    RedDot& number(const std::uint8_t& number);
    RedDot& number(std::uint8_t&& number);

    // get background color of "red" dot
    const Gdk::RGBA& background_color() const;
    // set background color
    RedDot& background_color(const Gdk::RGBA& rgba);
    RedDot& background_color(Gdk::RGBA&& rgba);

    // get text color of "red" dot
    const Gdk::RGBA& text_color() const;
    // set text color
    RedDot& text_color(const Gdk::RGBA& rgba);
    RedDot& text_color(Gdk::RGBA&& rgba);

    // font size
    int font_size() const;
    RedDot& font_size(int size);

    // font family
    std::string font_family() const;
    RedDot& font_family(std::string family);

    // font style
    Pango::Style font_style() const;
    RedDot& font_style(Pango::Style style);

protected:
    void update_number_();
    void update_font_();

protected:
    std::uint8_t number_ {0};
    Glib::ustring number_text_;
    Gdk::RGBA background_color_ {0.992f, 0.455f, 0.424f};
    Gdk::RGBA text_color_ {1.0f, 1.0f, 1.0f};
    Pango::FontDescription font_;
    Glib::RefPtr<Pango::Layout> text_layout_;
    int text_width_;
    int text_height_;
};

} // namespace widget
} // namespace gtk
} // namespace rayalto

#endif // RA_GTK_RED_DOT_H_
