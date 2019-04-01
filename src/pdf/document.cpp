#include <pdf/document.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "encode.hpp"
using namespace PoDoFo;

namespace pdf {

class document::impl {
public:
  PdfOutputDevice device;
  PdfStreamedDocument doc;

  impl(std::ostream& os) : device(&os), doc(&device) {
  }

  ~impl() {
    doc.Close();
  }
};

document::document(std::ostream& os) : impl_(std::make_unique<impl>(os)) {
}

document::~document() {
}

page document::create_page(rect ps) {
  return page(*this, ps);
}

page document::create_page(preset preset, bool landscape) {
  return page(*this, preset, landscape);
}

font document::create_font(const std::filesystem::path& filename) {
  const auto library = impl_->doc.GetFontLibrary();
  if (!library) {
    throw std::runtime_error("pdf: freetype error");
  }
  if (!std::filesystem::is_regular_file(filename)) {
    throw std::runtime_error("pdf: font file not found");
  }
  FT_Face face = nullptr;
  const auto filename_buffer = filename.u8string();
  if (FT_New_Face(library, filename_buffer.data(), 0, &face)) {
    throw std::runtime_error("pdf: freetype load font error");
  }
  const auto font = impl_->doc.CreateFont(face);
  if (!font) {
    throw std::runtime_error("pdf: create font error");
  }
  return font;
}

void document::set(const std::string& name, const std::string& value) {
  impl_->doc.GetInfo()->SetCustomKey(name, encode(value));
}

void document::author(const std::string& value) {
  impl_->doc.GetInfo()->SetAuthor(encode(value));
}

void document::creator(const std::string& value) {
  impl_->doc.GetInfo()->SetCreator(encode(value));
}

void document::keywords(const std::string& value) {
  impl_->doc.GetInfo()->SetKeywords(encode(value));
}

void document::subject(const std::string& value) {
  impl_->doc.GetInfo()->SetSubject(encode(value));
}

void document::title(const std::string& value) {
  impl_->doc.GetInfo()->SetTitle(encode(value));
}

void document::producer(const std::string& value) {
  impl_->doc.GetInfo()->SetProducer(encode(value));
}

const std::string& document::author() const {
  return impl_->doc.GetInfo()->GetAuthor().GetStringUtf8();
}

const std::string& document::creator() const {
  return impl_->doc.GetInfo()->GetCreator().GetStringUtf8();
}

const std::string& document::keywords() const {
  return impl_->doc.GetInfo()->GetKeywords().GetStringUtf8();
}

const std::string& document::subject() const {
  return impl_->doc.GetInfo()->GetSubject().GetStringUtf8();
}

const std::string& document::title() const {
  return impl_->doc.GetInfo()->GetTitle().GetStringUtf8();
}

const std::string& document::producer() const {
  return impl_->doc.GetInfo()->GetProducer().GetStringUtf8();
}

PdfStreamedDocument* document::handle() const {
  return &impl_->doc;
}

}  // namespace pdf
