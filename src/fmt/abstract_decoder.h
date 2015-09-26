#pragma once

#include <memory>
#include <vector>
#include "arg_parser.h"
#include "file.h"
#include "file_saver.h"
#include "fmt/registry.h"

#include "types.h"

namespace au {
namespace fmt {

    enum class FileNamingStrategy : u8
    {
        Root = 1,
        Sibling = 2,
        Child = 3,
    };

    class FileNameDecorator final
    {
    public:
        static std::string decorate(
            const FileNamingStrategy &strategy,
            const std::string &parent_file_name,
            const std::string &current_file_name);
    };

    class AbstractDecoder
    {
    public:
        virtual ~AbstractDecoder();
        bool is_recognized(File &) const;
        virtual void register_cli_options(ArgParser &) const = 0;
        virtual void parse_cli_options(const ArgParser &) = 0;
        virtual FileNamingStrategy get_file_naming_strategy() const = 0;
        virtual void unpack(File &, FileSaver &, bool) const = 0;

    protected:
        virtual bool is_recognized_internal(File &) const = 0;
    };

} }