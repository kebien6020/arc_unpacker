#pragma once

#include "fmt/image_decoder.h"

namespace au {
namespace fmt {
namespace leaf {

    class GrpImageDecoder final : public ImageDecoder
    {
    public:
        using ImageDecoder::decode;
        pix::Grid decode(
            File &file,
            std::shared_ptr<File> palette_file,
            std::shared_ptr<File> mask_file) const;
    protected:
        bool is_recognized_impl(File &) const override;
        pix::Grid decode_impl(File &) const override;
    };

} } }