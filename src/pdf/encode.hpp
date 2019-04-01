#pragma once
#include <podofo/podofo.h>
#include <string>

namespace pdf {

inline PoDoFo::PdfString encode(const std::string& str) {
  return PoDoFo::PdfString(reinterpret_cast<const PoDoFo::pdf_utf8*>(str.c_str()));
}

}  // namespace pdf
