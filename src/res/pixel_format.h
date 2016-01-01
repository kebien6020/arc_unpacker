#pragma once

#include "io/istream.h"
#include "res/pixel.h"

namespace au {
namespace res {

    enum class PixelFormat : u8
    {
        Gray8,

        BGR555X,
        BGR565,
        BGR888,
        BGR888X,
        BGRA4444,
        BGRA5551,
        BGRA8888,
        BGRnA4444,
        BGRnA5551,
        BGRnA8888,

        RGB555X,
        RGB565,
        RGB888,
        RGB888X,
        RGBA4444,
        RGBA5551,
        RGBA8888,
        RGBnA4444,
        RGBnA5551,
        RGBnA8888,

        Count
    };

    inline size_t pixel_format_to_bpp(PixelFormat fmt)
    {
        switch (fmt)
        {
            case PixelFormat::Gray8: return 1;
            case PixelFormat::BGR555X: return 2;
            case PixelFormat::BGR565: return 2;
            case PixelFormat::BGR888: return 3;
            case PixelFormat::BGR888X: return 4;
            case PixelFormat::BGRA4444: return 2;
            case PixelFormat::BGRA5551: return 2;
            case PixelFormat::BGRA8888: return 4;
            case PixelFormat::BGRnA4444: return 2;
            case PixelFormat::BGRnA5551: return 2;
            case PixelFormat::BGRnA8888: return 4;
            case PixelFormat::RGB555X: return 2;
            case PixelFormat::RGB565: return 2;
            case PixelFormat::RGB888: return 3;
            case PixelFormat::RGB888X: return 4;
            case PixelFormat::RGBA4444: return 2;
            case PixelFormat::RGBA5551: return 2;
            case PixelFormat::RGBA8888: return 4;
            case PixelFormat::RGBnA4444: return 2;
            case PixelFormat::RGBnA5551: return 2;
            case PixelFormat::RGBnA8888: return 4;
            default: return 0;
        }
    }

    template<PixelFormat fmt> Pixel read_pixel(const u8 *&ptr);

    template<PixelFormat fmt> void read_pixels(
        const u8 *input_ptr, std::vector<Pixel> &output)
    {
        for (auto &c : output)
            c = read_pixel<fmt>(input_ptr);
    }

    void read_pixels(
        const u8 *input_ptr,
        std::vector<Pixel> &output,
        const PixelFormat fmt);

    template<PixelFormat fmt> inline Pixel read_pixel(
        io::IStream &input_stream)
    {
        const auto str = input_stream.read(pixel_format_to_bpp(fmt));
        const auto *str_ptr = str.get<const u8>();
        return read_pixel<fmt>(str_ptr);
    }

} }
