# RA-Gtk

RayAlto's Stupid Self-taught Noob Custom Gtk Widgets

## Examples

### `rayalto::gtk::widget::Image`

A simple extension to `Gtk::Image`

- Animated image like gif will be played automatically:

![gif in Image](./screenshot/image_gif_rectangle.gif "gif in Image")

- There are three variants:

| `Image::Shape::RECTANGLE`                                                  | `Image::Shape::ROUNDED`                                              | `Image::Shape::CIRCLE`                                            |
| -------------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------------------------------------------------------- |
| ![rectangle image](./screenshot/image_gif_rectangle.gif "rectangle image") | ![rounded image](./screenshot/image_gif_rounded.gif "rounded image") | ![circle image](./screenshot/image_gif_circle.gif "circle image") |

- Scaling on vector image like svg is lossless, `Image` will load svg text into memory and generate pixbuf use the text in specified size every time while scaling:

![scaling on vector image](./screenshot/image_svg_scale.gif "scaling on vector image")

### `rayalto::gtk::widget::MessageBubble`

A simple message bubble

| `MessageBubble::Side::LEFT`                                                                  | `MessageBubble::Side::RIGHT`                                                                    |
| -------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- |
| ![message bubble from left](./screenshot/message_bubble_left.gif "message bubble from left") | ![message bubble from right](./screenshot/message_bubble_right.gif "message bubble from right") |

### `rayalto::gtk::widget::RedDot`

A simple red dot

![red dot 99 plus](./screenshot/red_dot_99plus.png "red dot 99 plus")
