#ifndef PARSER_H
#define PARSER_H

#include "tree.h"



/*!
 * \brief The Parser class parse EBNF grammar and generate token trees
 */
class Parser
{
    TreesContainer customTokenTrees_;///\brief customTokenTrees_ map of custom token names to custom token trees
    /*!
     * \brief linker function
     *
     * links all customTokenTrees_ to their TreePtr by name
     */
    void link();

public:
    /*!
     * \brief Parser default constructor
     */
    Parser() = default;
    Parser(const Parser &other) = delete;
    Parser operator=(const Parser& other)=delete;
    Parser(Parser &&other) = delete;
    Parser operator=(Parser&& other)=delete;
    /*!
     * \brief parse
     * \param [in] expr   string to parse
     *
     * parse \a expr, build AST's, and call \link()
     */
    void parse(const std::string &expr);
    /*!
     * \brief resultOfParsing
     * \return results of parsing
     */
    const TreesContainer & resultsOfParsing(){return customTokenTrees_;}
};

#endif // PARSER_H
