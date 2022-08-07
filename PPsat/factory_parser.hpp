#pragma once
#include <PPsat/factory.hpp>
#include <PPsat/antlr_parser.hpp>

#include <antlr4-runtime.h>

namespace PPsat
{
using factory_parser = factory<antlr_parser, antlr4::TokenStream*>;
}
