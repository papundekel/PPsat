#pragma once
#include <PPsat-base/factory.hpp>

#include <antlr4-runtime.h>

namespace PPsat_base
{
using factory_lexer = factory<antlr4::Lexer, antlr4::CharStream*>;
}
