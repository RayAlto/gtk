#ifndef RA_GTK_MESSAGE_BUBBLE_H_
#define RA_GTK_MESSAGE_BUBBLE_H_

#include <cstdint>

#include "gdkmm/rgba.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "gtkmm/label.h"

namespace rayalto {
namespace gtk {
namespace widget {

/**
 * Message bubble, real basic message bubble
 *  ________________________
 *  |                       |
 *  | The quick brown fuck  |
 * /________________________|
 *
 */
class MessageBubble : public Gtk::Widget {
public: // which side did the message come from?
    enum class Side : std::uint8_t { LEFT, RIGHT };

public:
    MessageBubble();
    MessageBubble(const Side& side);
    MessageBubble(const Glib::ustring& text);
    MessageBubble(const Glib::ustring& text, const Side& side);
    MessageBubble(const MessageBubble&) = delete;
    MessageBubble(MessageBubble&&) noexcept = default;
    MessageBubble& operator=(const MessageBubble&) = delete;
    MessageBubble& operator=(MessageBubble&&) noexcept = default;

    virtual ~MessageBubble();

    Gtk::SizeRequestMode get_request_mode_vfunc() const override;
    void measure_vfunc(Gtk::Orientation orientation,
                       int for_size,
                       int& minimum,
                       int& natural,
                       int& minimum_baseline,
                       int& natural_baseline) const override;
    void size_allocate_vfunc(int width, int height, int baseline) override;
    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) override;

    // get side the message come from
    const Side& from_side() const;
    Side& from_side();
    // set side the message come from
    MessageBubble& from_side(const Side& side);
    MessageBubble& from_side(Side&& side);

    // get bubble background color
    const Gdk::RGBA& background_color() const;
    Gdk::RGBA& background_color();
    // set bubble background color
    MessageBubble& background_color(const Gdk::RGBA& rgba);
    MessageBubble& background_color(Gdk::RGBA&& rgba);

    Gtk::Label gtk_label;

protected: // member
    Side from_side_ {Side::LEFT};
    Gdk::RGBA rgba_ {0.196f, 0.196f, 0.196f};
};

} // namespace widget
} // namespace gtk
} // namespace rayalto

#endif // RA_GTK_MESSAGE_BUBBLE_H_
