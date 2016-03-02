#ifndef PARSER_H
#define PARSER_H

#include "tree.h"
#include <map>


/**
 * @brief The Parser class parse EBNF grammar and generate token trees
 */
class Parser
{
    /*!
     * \brief tokenized_ result of last parsing and building tree operation
     */
    bool tokenized_;
    /*!
     * \brief result_ list of generated results
     */
    ResultType result_;
    /*!
     * \brief customTokenTrees_ map of custom token names to custom token trees
     */
    map<string,TreePtr> customTokenTrees_;
    /*!
     * \brief mainTokenName_ name of main token
     *
     * first custom token building tree starts from
     */
    string mainTokenName_;
    /**
     * @brief linker function
     *
     * links all customTokenTrees_ to their TreePtr by name
     */
    void link();
    /**
     * @brief size check function
     * @param checked tested size
     * @param passed set by user
     * @return true is @a checked is in range 10% of @a passed
     */
    bool checkSize(size_t checked, size_t passed);
    /*!
     * \brief Parser default constructor
     *
     * private because of singleton pattern
     */
    Parser();

public:
    /*!
     * \brief Parser copy constructor
     * \param other
     *
     * deleted!
     */
    Parser(Parser &other) = delete;
    /*!
     * \brief operator = copy assign
     * \param other
     * \return
     *
     * deleted!
     */
    Parser& operator=(Parser& other)=delete;
    /*!
    * \brief getParser
    * \return implementation of singleton
    *
    * singleton get function
    */
   static Parser& getParser();
   /**
     * @brief generate results
     * @param count
     * @param attemptCout
     * @return true if success, false otherwise
     *
     * try generate @a count of results in @a attemptCount of attempts
     * default generate without checking
     */
    bool generate(size_t count, int attemptCout = -1);
    /**
     * @brief setMainTokenName
     * @param s new name
     *
     * Sets main token name to @a s
     */
    void setMainTokenName(const string &s){if(s.size()) mainTokenName_ = s;}
    /**
     * @brief parse
     * @param expr
     *
     * parse @a expr, build AST's, and call @link()
     */
    void parse(const std::string &expr);
    /*!
     * \brief getResults
     * \return list of generated results
     *
     * if no results was generated or error occured return empty list
     */
    const ResultType&  getResults(){return result_;}
};

#endif // PARSER_H
