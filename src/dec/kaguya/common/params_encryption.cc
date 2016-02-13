﻿#include "dec/kaguya/common/params_encryption.h"
#include "algo/binary.h"
#include "algo/locale.h"
#include "algo/range.h"
#include "err.h"

using namespace au;
using namespace au::dec::kaguya;

static common::Params parse_params_file_v2(io::BaseByteStream &input_stream)
{
    input_stream.skip(8);
    input_stream.skip(input_stream.read<u8>());
    const auto game_title
        = algo::sjis_to_utf8(input_stream.read(input_stream.read<u8>()));
    const auto producer
        = algo::sjis_to_utf8(input_stream.read(input_stream.read<u8>()));
    const auto copyright
        = algo::sjis_to_utf8(input_stream.read(input_stream.read<u8>()));
    input_stream.skip(input_stream.read<u8>());
    input_stream.skip(1);
    for (const auto i : algo::range(2))
        input_stream.skip(input_stream.read<u8>());
    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        const auto arc_name = input_stream.read(input_stream.read<u8>());
        const auto arc_type = input_stream.read(input_stream.read<u8>());
    }
    input_stream.skip(1);

    size_t key_size;

    if (game_title == "幼なじみと甘～くエッチに過ごす方法"_b
        || game_title == "艶女医"_b)
    {
        for (const auto i : algo::range(input_stream.read<u8>()))
        {
            input_stream.skip(1);
            input_stream.skip(input_stream.read<u8>());
            for (const auto j : algo::range(input_stream.read<u8>()))
                input_stream.skip(input_stream.read<u8>());
            input_stream.skip(input_stream.read<u8>());
        }

        for (const auto i : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read<u8>());

        for (const auto i : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read<u8>());

        key_size = input_stream.read_le<u32>();
        if (game_title == "幼なじみと甘～くエッチに過ごす方法"_b)
            key_size = 240000;
    }

    else if (game_title == "新妻イカせてミルク！"_b
        || game_title == "毎日がＭ！"_b
        || game_title == "ちゅぱしてあげる"_b)
    {
        for (const auto i : algo::range(input_stream.read<u8>()))
        {
            input_stream.skip(1);
            input_stream.skip(input_stream.read<u8>());
            for (const auto j : algo::range(input_stream.read<u8>()))
                input_stream.skip(input_stream.read<u8>());
            for (const auto j : algo::range(input_stream.read<u8>()))
                input_stream.skip(input_stream.read<u8>());
        }

        for (const auto i : algo::range(input_stream.read<u8>()))
        {
            input_stream.skip(input_stream.read<u8>());
            input_stream.skip(input_stream.read<u8>());
        }

        for (const auto i : algo::range(input_stream.read<u8>()))
        {
            input_stream.skip(input_stream.read<u8>());
            for (const auto j : algo::range(input_stream.read<u8>()))
                input_stream.skip(input_stream.read<u8>());
            for (const auto j : algo::range(input_stream.read<u8>()))
                input_stream.skip(input_stream.read<u8>());
        }

        key_size = input_stream.read_le<u32>();
    }
    else
    {
        throw err::CorruptDataError("Unknown game: " + game_title.str());
    }

    common::Params params;
    params.decrypt_anm = game_title != "幼なじみと甘～くエッチに過ごす方法"_b;
    params.game_title = game_title;
    params.key = input_stream.read(key_size);
    return params;
}

static common::Params parse_params_file_v3_or_4(
    io::BaseByteStream &input_stream)
{
    input_stream.skip(10);
    input_stream.skip(input_stream.read<u8>());
    const auto game_title
        = algo::sjis_to_utf8(input_stream.read(input_stream.read<u8>()));
    const auto producer
        = algo::sjis_to_utf8(input_stream.read(input_stream.read<u8>()));
    const auto copyright
        = algo::sjis_to_utf8(input_stream.read(input_stream.read<u8>()));
    input_stream.skip(1);
    for (const auto i : algo::range(2))
        input_stream.skip(input_stream.read<u8>());
    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        const auto arc_name = input_stream.read(input_stream.read<u8>());
        const auto arc_type = input_stream.read(input_stream.read<u8>());
    }
    input_stream.skip(12);
    input_stream.skip(1);

    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        input_stream.skip(1);
        input_stream.skip(input_stream.read<u8>());
        for (const auto j : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read<u8>());
        for (const auto j : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read<u8>());
    }

    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        input_stream.skip(input_stream.read<u8>());
        input_stream.skip(input_stream.read<u8>());
    }

    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        input_stream.skip(input_stream.read<u8>());
        for (const auto j : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read<u8>());
        for (const auto j : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read<u8>());
    }

    const auto key_size = input_stream.read_le<u32>();
    common::Params params;
    params.decrypt_anm = true;
    params.game_title = game_title;
    params.key = input_stream.read(key_size);
    return params;
}

static common::Params parse_params_file_v5(
    io::BaseByteStream &input_stream)
{
    input_stream.skip(10);
    input_stream.skip(input_stream.read<u8>());
    const auto game_title
        = algo::utf16_to_utf8(input_stream.read(input_stream.read_le<u16>()));
    const auto producer
        = algo::utf16_to_utf8(input_stream.read(input_stream.read_le<u16>()));
    const auto copyright
        = algo::utf16_to_utf8(input_stream.read(input_stream.read_le<u16>()));
    input_stream.skip(1);
    for (const auto i : algo::range(2))
        input_stream.skip(input_stream.read_le<u16>());
    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        const auto arc_name = algo::utf16_to_utf8(
            input_stream.read(input_stream.read_le<u16>()));
        const auto arc_type = algo::utf16_to_utf8(
            input_stream.read(input_stream.read_le<u16>()));
    }
    input_stream.skip(12);

    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        input_stream.skip(1);
        input_stream.skip(input_stream.read_le<u16>());
        for (const auto j : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read_le<u16>());
        for (const auto j : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read_le<u16>());
    }

    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        input_stream.skip(input_stream.read_le<u16>());
        input_stream.skip(input_stream.read_le<u16>());
    }

    for (const auto i : algo::range(input_stream.read<u8>()))
    {
        input_stream.skip(input_stream.read_le<u16>());
        for (const auto j : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read_le<u16>());
        for (const auto j : algo::range(input_stream.read<u8>()))
            input_stream.skip(input_stream.read_le<u16>());
    }

    const auto key_size = input_stream.read_le<u32>();
    common::Params params;
    params.decrypt_anm = true;
    params.game_title = game_title;
    params.key = input_stream.read(key_size);
    return params;
}

static bool verify_magic(io::BaseByteStream &input_stream, const bstr &magic)
{
    return input_stream.seek(0).read(magic.size()) == magic;
}

common::Params common::parse_params_file(io::BaseByteStream &input_stream)
{
    if (verify_magic(input_stream, "[SCR-PARAMS]v02"_b))
        return parse_params_file_v2(input_stream);

    if (verify_magic(input_stream, "[SCR-PARAMS]v03"_b)
        || verify_magic(input_stream, "[SCR-PARAMS]v04"_b))
    {
        return parse_params_file_v3_or_4(input_stream);
    }

    if (verify_magic(input_stream, "[SCR-PARAMS]v05.1"_b)
        || verify_magic(input_stream, "[SCR-PARAMS]v05"_b))
    {
        return parse_params_file_v5(input_stream);
    }

    throw err::RecognitionError();
}

static void decrypt(
    io::BaseByteStream &input_stream,
    const bstr &key,
    const size_t pos,
    const size_t size)
{
    const auto old_pos = input_stream.pos();
    input_stream.seek(pos);
    const auto data = algo::unxor(input_stream.read(size), key);
    input_stream.seek(pos);
    input_stream.write(data);
    input_stream.seek(old_pos);
}

static void decrypt(
    io::BaseByteStream &input_stream, const bstr &key, const size_t pos)
{
    decrypt(input_stream, key, pos, input_stream.size() - pos);
}

void common::decrypt(
    io::BaseByteStream &input_stream, const common::Params &params)
{
    if (input_stream.seek(0).read(2) == "BM"_b)
        ::decrypt(input_stream, params.key, 54);

    else if (input_stream.seek(0).read(4) == "AP-0"_b)
         ::decrypt(input_stream, params.key, 12);

    else if (input_stream.seek(0).read(4) == "AP-1"_b)
         ::decrypt(input_stream, params.key, 12);

    else if (input_stream.seek(0).read(4) == "AP-2"_b)
         ::decrypt(input_stream, params.key, 24);

    else if (input_stream.seek(0).read(4) == "AP-3"_b)
         ::decrypt(input_stream, params.key, 24);

    else if (input_stream.seek(0).read(2) == "AP"_b)
        ::decrypt(input_stream, params.key, 12);

    else if (input_stream.seek(0).read(4) == "AN00"_b && params.decrypt_anm)
    {
        input_stream.seek(20);
        const auto frame_count = input_stream.read_le<u16>();
        input_stream.skip(2 + frame_count * 4);
        const auto file_count = input_stream.read_le<u16>();
        for (const auto i : algo::range(file_count))
        {
            input_stream.skip(8);
            const auto width = input_stream.read_le<u32>();
            const auto height = input_stream.read_le<u32>();
            const auto size = 4 * width * height;
            ::decrypt(input_stream, params.key, input_stream.pos(), size);
            input_stream.skip(size);
        }
    }

    else if (input_stream.seek(0).read(4) == "AN10"_b && params.decrypt_anm)
    {
        input_stream.seek(20);
        const auto frame_count = input_stream.read_le<u16>();
        input_stream.skip(2 + frame_count * 4);
        const auto file_count = input_stream.read_le<u16>();
        for (const auto i : algo::range(file_count))
        {
            input_stream.skip(8);
            const auto width = input_stream.read_le<u32>();
            const auto height = input_stream.read_le<u32>();
            const auto channels = input_stream.read_le<u32>();
            const auto size = channels * width * height;
            ::decrypt(input_stream, params.key, input_stream.pos(), size);
            input_stream.skip(size);
        }
    }

    else if (input_stream.seek(0).read(4) == "AN20"_b && params.decrypt_anm)
    {
        input_stream.seek(4);
        const auto unk_count = input_stream.read_le<u16>();
        input_stream.skip(2);
        for (const auto i : algo::range(unk_count))
        {
            const auto control = input_stream.read<u8>();
            if (control == 0) continue;
            else if (control == 1) input_stream.skip(8);
            else if (control == 2) input_stream.skip(4);
            else if (control == 3) input_stream.skip(4);
            else if (control == 4) input_stream.skip(4);
            else if (control == 5) input_stream.skip(4);
            else throw err::NotSupportedError("Unsupported control");
        }
        input_stream.skip(2);
        const auto file_count = input_stream.read_le<u16>();
        if (!file_count)
            return;
        input_stream.skip(16);
        for (const auto i : algo::range(file_count))
        {
            input_stream.skip(8);
            const auto width = input_stream.read_le<u32>();
            const auto height = input_stream.read_le<u32>();
            const auto channels = input_stream.read_le<u32>();
            const auto size = channels * width * height;
            ::decrypt(input_stream, params.key, input_stream.pos(), size);
            input_stream.skip(size);
        }
    }

    else if (input_stream.seek(0).read(4) == "AN21"_b && params.decrypt_anm)
    {
        input_stream.seek(4);
        const auto unk_count = input_stream.read_le<u16>();
        input_stream.skip(2);
        for (const auto i : algo::range(unk_count))
        {
            const auto control = input_stream.read<u8>();
            if (control == 0) continue;
            else if (control == 1) input_stream.skip(8);
            else if (control == 2) input_stream.skip(4);
            else if (control == 3) input_stream.skip(4);
            else if (control == 4) input_stream.skip(4);
            else if (control == 5) input_stream.skip(4);
            else throw err::NotSupportedError("Unsupported control");
        }
        const auto unk2_count = input_stream.read_le<u16>();
        input_stream.skip(unk2_count * 8);
        input_stream.skip(7);
        const auto file_count = input_stream.read_le<u16>();
        if (!file_count)
            return;
        input_stream.skip(24);
        const auto width = input_stream.read_le<u32>();
        const auto height = input_stream.read_le<u32>();
        const auto channels = input_stream.read_le<u32>();
        const auto size = channels * width * height;
        ::decrypt(input_stream, params.key, input_stream.pos(), size);
    }

    else if (input_stream.seek(0).read(4) == "PL00"_b)
    {
        input_stream.seek(4);
        const auto file_count = input_stream.read_le<u16>();
        input_stream.skip(16);
        for (const auto i : algo::range(file_count))
        {
            input_stream.skip(8);
            const auto width = input_stream.read_le<u32>();
            const auto height = input_stream.read_le<u32>();
            const auto channels = input_stream.read_le<u32>();
            const auto size = channels * width * height;
            ::decrypt(input_stream, params.key, input_stream.pos(), size);
            input_stream.skip(size);
        }
    }

    else if (input_stream.seek(0).read(4) == "PL10"_b)
    {
        input_stream.seek(4);
        const auto file_count = input_stream.read_le<u16>();
        input_stream.skip(16);
        input_stream.skip(8);
        const auto width = input_stream.read_le<u32>();
        const auto height = input_stream.read_le<u32>();
        const auto channels = input_stream.read_le<u32>();
        const auto size = channels * width * height;
        ::decrypt(input_stream, params.key, input_stream.pos(), size);
    }
}
