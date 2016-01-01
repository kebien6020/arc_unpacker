#pragma once

#include <memory>
#include "io/ibit_reader.h"
#include "io/istream.h"
#include "types.h"

namespace au {
namespace fmt {
namespace bgi {
namespace cbg {

    bstr read_decrypted_data(io::IStream &input_stream);

    using FreqTable = std::vector<u32>;

    struct NodeInfo final
    {
        bool valid;
        u32 frequency;
        u32 children[2];
    };

    struct Tree final
    {
        u32 get_leaf(io::IBitReader &bit_reader) const;

        NodeInfo &operator[](size_t);

        u32 size;
        std::vector<std::shared_ptr<NodeInfo>> nodes;
    };

    u32 read_variable_data(io::IStream &input_stream);
    FreqTable read_freq_table(io::IStream &input_stream, size_t tree_size);
    Tree build_tree(const FreqTable &freq_table, bool greedy);

} } } }
