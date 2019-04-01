#pragma once
#include <pdf/config.hpp>
#include <pdf/color.hpp>
#include <pdf/font.hpp>
#include <pdf/point.hpp>
#include <pdf/rect.hpp>
#include <pdf/units.hpp>
#include <memory>
#include <string>
#include <istream>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

namespace PoDoFo {

class PdfPage;

}  // namespace PoDoFo

namespace pdf {

enum class alignment { left, center, right };

enum class style { miter, round, bevel };

enum class preset { a0, a1, a2, a3, a4, a5, a6, letter, legal, tabloid };

class document;

class PDF_API page {
public:
  page(document& doc, rect ps);
  page(document& doc, preset preset = preset::a4, bool landscape = false);

  page(page&& other);
  page(const page& other) = delete;

  page& operator=(page&& other);
  page& operator=(const page& other) = delete;

  virtual ~page();

  void color(const pdf::color& col);
  void stroke_color(const pdf::color& col);
  void stroke_style(pdf::style style);
  void stroke_width(const units& width);

  // Starts a new path.
  void move_to(const point& pt);

  // Adds a line to the current path.
  void line_to(const point& pt);

  // Closes the current path.
  // connect  : connects the current point with the start of the path
  // stroke   : strokes the closed path
  // fill     : fills the closed path
  void close(bool connect, bool stroke, bool fill);

  void draw_line(const point& a, const point& b);
  void draw_rect(const rect& rc, bool stroke, bool fill);

  void font(font font, units size = 10);

  units draw_text(const point& pt, const std::string& text, units cx = 0, alignment align = alignment::left);
  units draw_text_90(const double& angle,
    const units& x,
    const units& y,
    const std::string& text,
    units width = 0,
    alignment align = alignment::left);
  units text_width(const std::string& text) const;

  void draw_image(rect rc, const std::string& path, bool keep_aspect = true);

  // Font height above the line.
  units font_top() const;

  // Font height below the line.
  units font_bottom() const;

  // Font height (total).
  units font_height() const;

  units cx() const;
  units cy() const;

  void painter_save();
  void painter_restore();

  PoDoFo::PdfPage* handle() const;

private:
  class impl;
  std::unique_ptr<impl> impl_;
};

}  // namespace pdf

#ifdef _MSC_VER
#pragma warning(pop)
#endif
