#ifndef PARSER_H
#define PARSER_H

#include "tree.h"
#include <map>



class Parser
{
    bool tokenized_;
    ResultType result_;
    map<string,TreePtr> customTokenTrees_;
    string mainTokenName_;
    /**
     * @brief link
     *
     * links all customTokenTrees_ to their TreePtr by name
     */
    void link();
    /**
     * @brief checkSize
     * @param checked tested size
     * @param passed set by user
     * @return true is @a checked is in range 10% of @a passed
     */
    bool checkSize(size_t checked, size_t passed);

    Parser();
    Parser(Parser &other);
    Parser& operator=(Parser& other);
public:
   static Parser& getParser();
   /**
     * @brief generate results
     * @param count
     * @param attemptCout
     * @return true if success, false otherwise
     *
     * try generate @a count of results in @a attemptCount of attempts
     */
    bool generate(size_t count, int attemptCout = 100);
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
     * parse @a expr for tokens
     */
    void parse(const std::string &expr);

    const ResultType&  getResults(){return result_;}
};

#endif // PARSER_H
