#pragma once
#include <PPsat/factory.hpp>

#include <antlr4-runtime.h>

namespace PPsat
{
using factory_lexer = factory<antlr4::Lexer, antlr4::CharStream*>;
}
