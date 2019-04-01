#pragma once
#include <pdf/config.hpp>
#include <pdf/font.hpp>
#include <pdf/page.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <ostream>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

namespace PoDoFo {

class PdfStreamedDocument;

}  // namespace PoDoFo

namespace pdf {

class PDF_API document {
public:
  document(std::ostream& os);

  document(document&& other) = delete;
  document(const document& other) = delete;

  document& operator=(document&& other) = delete;
  document& operator=(const document& other) = delete;

  virtual ~document();

  page create_page(rect ps);
  page create_page(preset preset = preset::a4, bool landscape = false);

  font create_font(const std::filesystem::path& filename);

  void set(const std::string& name, const std::string& value);

  void author(const std::string& value);
  void creator(const std::string& value);
  void keywords(const std::string& value);
  void subject(const std::string& value);
  void title(const std::string& value);
  void producer(const std::string& value);

  const std::string& author() const;
  const std::string& creator() const;
  const std::string& keywords() const;
  const std::string& subject() const;
  const std::string& title() const;
  const std::string& producer() const;

  PoDoFo::PdfStreamedDocument* handle() const;

private:
  class impl;
  std::unique_ptr<impl> impl_;
};

}  // namespace pdf

#ifdef _MSC_VER
#pragma warning(pop)
#endif
