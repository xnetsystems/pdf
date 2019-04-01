#pragma once
#ifndef PDF_API
#ifdef _MSC_VER
#ifdef pdf_EXPORTS
#define PDF_API __declspec(dllexport)
#else
#define PDF_API __declspec(dllimport)
#endif
#else
#define PDF_API
#endif
#endif
