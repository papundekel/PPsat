#pragma once
#include <PPsat-base/antlr_parser.hpp>
#include <PPsat-base/factory.hpp>

#include <antlr4-runtime.h>

namespace PPsat_base
{
using factory_parser = factory<PPsat_base::antlr_parser, antlr4::TokenStream*>;
}
