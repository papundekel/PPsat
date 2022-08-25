#include <PPsat-base/antlrer.hpp>

PPsat_base::antlrer::antlrer(const logger& logger_outer,
                             antlr4::CharStream& input,
                             const factory_lexer& factory_lexer,
                             const factory_parser& factory_parser,
                             bool lexer_error_is_fail)
    : listener(logger_outer)
    , lexer(
          [this, &input, &factory_lexer, lexer_error_is_fail]()
          {
              auto lexer = factory_lexer.create(&input);
              lexer->removeErrorListeners();
              if (lexer_error_is_fail)
              {
                  lexer->addErrorListener(&listener);
              }
              return lexer;
          }())
    , token_stream(lexer.get())
    , parser(
          [this, &factory_parser]()
          {
              auto parser = factory_parser.create(&token_stream);
              parser->removeErrorListeners();
              parser->addErrorListener(&listener);
              return parser;
          }())
{}

antlr4::ParserRuleContext* PPsat_base::antlrer::parse() const noexcept
{
    auto parsed_tree = parser->getParsedTree();
    return listener.error_encountered() ? nullptr : parsed_tree;
}
