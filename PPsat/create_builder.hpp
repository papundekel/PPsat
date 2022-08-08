#pragma once
#include <PPsat/builder.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/option/format.hpp>
#include <PPsat/unique_ref.hpp>

#include <memory>

namespace PPsat
{
unique_ref<builder> create_builder(const PPsat::cli::option::format& options,
                                   const PPsat::cli::argument::file_in& file,
                                   bool nnf);
}
