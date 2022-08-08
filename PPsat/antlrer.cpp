#include <PPsat/antlrer.hpp>

PPsat::antlrer::antlrer(const logger& logger_outer,
                        std::istream& input,
                        const factory_lexer& factory_lexer,
                        const factory_parser& factory_parser)
    : listener(logger_outer)
    , input_antlr(input)
    , lexer(
          [this, &factory_lexer]()
          {
              auto lexer = factory_lexer.create(&input_antlr);
              // lexer->removeErrorListeners();
              lexer->addErrorListener(&listener);
              return lexer;
          }())
    , token_stream(lexer.get())
    , parser(
          [this, &factory_parser]()
          {
              auto parser = factory_parser.create(&token_stream);
              // parser->removeErrorListeners();
              parser->addErrorListener(&listener);
              return parser;
          }())
{}

antlr4::ParserRuleContext* PPsat::antlrer::parse() const noexcept
{
    auto parsed_tree = parser->getParsedTree();
    return listener.error_encountered() ? nullptr : parsed_tree;
}
