#include <PPsat/antlrer.hpp>

PPsat::antlrer::antlrer(std::istream& input,
                        const factory_lexer_t& factory_lexer,
                        const factory_parser_t& factory_parser)
    : error_listener()
    , input_antlr(input)
    , lexer(
          [this, &factory_lexer]()
          {
              auto lexer = factory_lexer.create(&input_antlr);
              lexer->addErrorListener(&error_listener);
              return lexer;
          }())
    , token_stream(lexer.get())
    , parser(
          [this, &factory_parser]()
          {
              auto parser = factory_parser.create(&token_stream);
              lexer->addErrorListener(&error_listener);
              return parser;
          }())
{}

antlr4::ParserRuleContext* PPsat::antlrer::parse() const noexcept
{
    auto parsed_tree = parser->getParsedTree();
    return error_listener.error_encountered() ? nullptr : parsed_tree;
}
