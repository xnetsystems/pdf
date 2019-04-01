#include <pdf/page.hpp>
#include <pdf/document.hpp>
#include <algorithm>
#include <limits>
#include <vector>
#include <iterator>
#include <iostream>
#include <utf8proc.h>
#include "encode.hpp"
using namespace PoDoFo;

namespace pdf {
namespace {

template <typename T>
void check(const T& impl) {
  if (!impl) {
    throw std::runtime_error("pdf: invalid page");
  }
}

}  // namespace

class page::impl {
public:
  document& doc;
  PdfPage* page = nullptr;
  PdfPainter painter;
  units cx;
  units cy;

  pdf::font font = nullptr;
  double font_size = 10.0;
  double font_ascent = 0.0;
  double font_descent = 0.0;

  impl(document& doc, PdfRect rc) : doc(doc) {
    page = doc.handle()->CreatePage(rc);
    cx = page->GetPageSize().GetWidth();
    cy = page->GetPageSize().GetHeight();
    painter.SetPage(page);
  }

  ~impl() {
    painter.FinishPage();
  }
};

page::page(document& doc, rect ps) : impl_(std::make_unique<impl>(doc, PdfRect(ps.x, -(ps.cy - ps.y), ps.cx, -ps.cy))) {
}

page::page(document& doc, preset preset, bool landscape) {
  auto size = ePdfPageSize_A4;
  switch (preset) {
  case preset::a0: size = ePdfPageSize_A0; break;
  case preset::a1: size = ePdfPageSize_A1; break;
  case preset::a2: size = ePdfPageSize_A2; break;
  case preset::a3: size = ePdfPageSize_A3; break;
  case preset::a4: size = ePdfPageSize_A4; break;
  case preset::a5: size = ePdfPageSize_A5; break;
  case preset::a6: size = ePdfPageSize_A6; break;
  case preset::letter: size = ePdfPageSize_Letter; break;
  case preset::legal: size = ePdfPageSize_Legal; break;
  case preset::tabloid: size = ePdfPageSize_Tabloid; break;
  }
  auto rc = PdfPage::CreateStandardPageSize(size, landscape);
  impl_ = std::make_unique<impl>(doc, rc);
}

page::page(page&& other) : impl_(std::move(other.impl_)) {
}

page& page::operator=(page&& other) {
  impl_ = std::move(other.impl_);
  return *this;
}

page::~page() {
}

void page::color(const pdf::color& color) {
  check(impl_);
  double r = color.r / 255.0;
  double g = color.g / 255.0;
  double b = color.b / 255.0;
  impl_->painter.SetColor(PdfColor(r, g, b));
}

void page::stroke_color(const pdf::color& col) {
  check(impl_);
  double r = col.r / 255.0;
  double g = col.g / 255.0;
  double b = col.b / 255.0;
  impl_->painter.SetStrokingColor(PdfColor(r, g, b));
}

void page::stroke_style(pdf::style style) {
  check(impl_);
  auto pdf_style = EPdfLineJoinStyle::ePdfLineJoinStyle_Miter;
  switch (style) {
  case pdf::style::miter: pdf_style = EPdfLineJoinStyle::ePdfLineJoinStyle_Miter; break;
  case pdf::style::round: pdf_style = EPdfLineJoinStyle::ePdfLineJoinStyle_Round; break;
  case pdf::style::bevel: pdf_style = EPdfLineJoinStyle::ePdfLineJoinStyle_Bevel; break;
  }
  impl_->painter.SetLineJoinStyle(pdf_style);
}

void page::stroke_width(const units& width) {
  check(impl_);
  impl_->painter.SetStrokeWidth(width);
}

void page::move_to(const point& pt) {
  check(impl_);
  impl_->painter.MoveTo(pt.x, impl_->cy - pt.y);
}

void page::line_to(const point& pt) {
  check(impl_);
  impl_->painter.LineTo(pt.x, impl_->cy - pt.y);
}

void page::close(bool connect, bool stroke, bool fill) {
  check(impl_);
  if (connect) {
    impl_->painter.ClosePath();
  }
  if (stroke && fill) {
    impl_->painter.FillAndStroke();
  } else if (stroke) {
    impl_->painter.Stroke();
  } else if (fill) {
    impl_->painter.Fill();
  } else {
    impl_->painter.Close();
  }
}

void page::draw_line(const point& a, const point& b) {
  check(impl_);
  impl_->painter.DrawLine(a.x, impl_->cy - a.y, b.x, impl_->cy - b.y);
}

void page::draw_rect(const rect& rc, bool stroke, bool fill) {
  check(impl_);
  impl_->painter.MoveTo(rc.x, impl_->cy - rc.y);
  impl_->painter.LineTo(rc.x + rc.cx, impl_->cy - rc.y);
  impl_->painter.LineTo(rc.x + rc.cx, impl_->cy - rc.y - rc.cy);
  impl_->painter.LineTo(rc.x, impl_->cy - rc.y - rc.cy);
  close(true, stroke, fill);
}

void page::font(pdf::font font, units size) {
  check(impl_);
  impl_->font = font;
  impl_->font_size = size;
  impl_->font_ascent = impl_->font->GetFontMetrics()->GetAscent();
  impl_->font_descent = impl_->font->GetFontMetrics()->GetDescent();
  impl_->font->SetFontSize(static_cast<float>(size));
  impl_->painter.SetFont(impl_->font);
}

units page::draw_text(const point& pt, const std::string& text, units cx, alignment alignment) {
  check(impl_);
  if (impl_->font) {
    auto x = pt.x;
    auto y = impl_->cy - pt.y /* - impl_->font_size - impl_->font_descent*/;
    auto metrics = impl_->font->GetFontMetrics();
    auto pdf_text = encode(text);
    auto pdf_text_width = units(metrics->StringWidth(pdf_text));
    if (cx == 0_mm) {
      impl_->painter.DrawText(x, y, pdf_text);
      return pdf_text_width;
    }
    if (pdf_text_width > cx) {
      pdf_text_width = metrics->UnicodeCharWidth(L'\u2026');

      const auto data = reinterpret_cast<const utf8proc_uint8_t*>(text.data());
      const auto size = static_cast<utf8proc_ssize_t>(text.size());
      utf8proc_int32_t cp = 0;
      for (auto it = data; it < data + size;) {
        const auto result = utf8proc_iterate(it, data + size - it, &cp);
        if (result <= 0) {
          break;  // invalid UTF-8 string
        }
        if (cp > std::numeric_limits<unsigned short>::max()) {
          break;  // podofo does not support UTF-32
        }
        auto cw = units(metrics->UnicodeCharWidth(static_cast<unsigned short>(cp)));
        if (pdf_text_width + cw > cx) {
          pdf_text = encode(std::string(text.data(), static_cast<std::size_t>(it - data)) + "\xE2\x80\xA6");
          break;
        }
        pdf_text_width += cw;
        it += result;
      }
    }
    auto align = ePdfAlignment_Left;
    switch (alignment) {
    case alignment::left: align = ePdfAlignment_Left; break;
    case alignment::center: align = ePdfAlignment_Center; break;
    case alignment::right: align = ePdfAlignment_Right; break;
    }
    impl_->painter.DrawTextAligned(x, y, cx, pdf_text, align);
    return pdf_text_width;
  }
  return 0;
}

units page::draw_text_90(const double& angle,
  const units& cx,
  const units& cy,
  const std::string& text,
  units width,
  alignment align) {
  check(impl_);
  auto x = impl_->cx - cx;
  auto y = impl_->cy - cy;
  const double pi = std::acos(-1);
  auto rad = angle * pi / 180;
  impl_->painter.SetTransformationMatrix(std::cos(rad), std::sin(rad), -std::sin(rad), std::cos(rad), 88.5_mm, 0);
  auto x_rota = x * std::cos(rad) + y * std::sin(rad);
  auto y_rota = x * -std::sin(rad) + y * std::cos(rad);
  return draw_text({ x_rota, y_rota }, text, width, align);
}

units page::text_width(const std::string& text) const {
  check(impl_);
  return impl_->font->GetFontMetrics()->StringWidth(encode(text));
}

void page::draw_image(rect rc, const std::string& path, bool keep_aspect) {
  check(impl_);
  PdfImage image(impl_->doc.handle());
  image.LoadFromFile(path.c_str());
  auto cx = image.GetWidth();
  auto cy = image.GetHeight();
  double scale_x = rc.cx / cx;
  double scale_y = rc.cy / cy;
  if (keep_aspect) {
    scale_x = std::min(scale_x, scale_y);
    scale_y = std::min(scale_x, scale_y);
    rc.x += (rc.cx.pt() - cx * scale_x) / 2.0;
    rc.y -= (rc.cy.pt() - cy * scale_y) / 2.0;
  }
  impl_->painter.DrawImage(rc.x, impl_->cy - rc.y - rc.cy, &image, scale_x, scale_y);
}

units page::font_top() const {
  check(impl_);
  return impl_->font_ascent + impl_->font_descent;
}

units page::font_bottom() const {
  check(impl_);
  return -impl_->font_descent;
}

units page::font_height() const {
  check(impl_);
  return impl_->font_ascent;
}

units page::cx() const {
  check(impl_);
  return impl_->cx;
}

units page::cy() const {
  check(impl_);
  return impl_->cy;
}

void page::painter_save() {
  check(impl_);
  impl_->painter.Save();
}

void page::painter_restore() {
  check(impl_);
  impl_->painter.Restore();
}

PdfPage* page::handle() const {
  check(impl_);
  return impl_->page;
}

}  // namespace pdf
