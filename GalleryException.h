// GalleryException.h
// Custom exception types used for validation and business-rule errors.
#ifndef GALLERY_EXCEPTION_H
#define GALLERY_EXCEPTION_H

#include <stdexcept>
#include <string>

// Thrown for any validation / business-rule / file error in the system.
class GalleryException : public std::runtime_error {
public:
    explicit GalleryException(const std::string& message)
        : std::runtime_error(message) {}
};

// Thrown when the input stream is closed (Ctrl+D / Ctrl+Z / end of piped input).
class InputEndedException : public GalleryException {
public:
    InputEndedException() : GalleryException("Input stream closed") {}
};

#endif
