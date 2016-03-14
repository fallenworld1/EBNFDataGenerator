#ifndef GENERATOR_H
#define GENERATOR_H
#include "parser.h"


/*!
 * \brief The Generator class used to generate results from parsed EBNF string
 */
class Generator
{
    /*!
     * \brief result_ list of generated results
     */
    StringList result_;
    /*!
     * \brief customTokenTrees_ map of custom token names to custom token trees
     */
    TreesContainer customTokenTrees_;
    /*!
     * \brief mainTokenName_ name of main token
     *
     * first custom token building tree starts from
     */
    std::string mainTokenName_;
    /*!
     * \brief size check function
     * \param checked tested size
     * \param passed set by user
     * \return true is \a checked is in range 10% of \a passed
     */
    bool checkSize(size_t checked, size_t passed);
public:
    Generator();
    /*!
      * \brief generate results
      * \param count
      * \param attemptCout
      * \return true if success, false otherwise
      *
      * try generate \a count of results in \a attemptCount of attempts
      * default generate without checking
      */
     void generate(size_t count = 0, int attemptCout = -1);
     /*!
      * \brief generate list of results(ResultType) for only \a customTokenName
      * \param customTokenName token result of which we want to generate
      * \param count results count
      * \param attemptCout count of attempt to tune generating variable to generate enougth results
      * \return true if success, false otherwise
      * \throws if token whith \a customTokenName doesnt exists
      */
     void generate(std::string customTokenName, size_t count = 0, int attemptCout = -1);
     /*!
      * \brief setMainTokenName
      * \param [in] s new name
      *
      * Sets main token name to \a s
      */
     void setMainTokenName(const std::string &s){if(s.size()) mainTokenName_ = s;}
     /*!
      * \brief getResults
      * \return list of generated results
      *
      * if no results was generated or error occured return empty list
      */
     const StringList&  getResults() const {return result_;}
     /*!
      * \brief setDictionary set dictionary for one of custom token
      * \param [in] customTokenName name of token to set dictionary
      * \param [in] d new dictionary
      * \throw DGException if token doesnt exists or dictionary set unsuccesfully
      *
      * for first custom token with name \a customTokenName from \a customTokenTrees_ set dictionary to \a d
      */
     void setDictionary(const std::string &customTokenName, const StringList &d);
     /*!
      * \brief setAddingPolicy sets user defined policy
      * \param [in] customTokenName name of token to set policy
      * \param [in] policy new policy
      *
      * for first custom token with name \a customTokenName from \a customTokenTrees_ set addingPolicy to \a policy
      */
     void setAddingPolicy(const std::string &customTokenName, PolicyPtr policy);
     /*!
      * \brief setAddingPolicy sets \a policy for all trees
      * \param [in] policy new policy
      */
     void setAddingPolicy(PolicyPtr policy);
     /*!
      * \brief setDictionary sets dictionary \a d for all trees
      * \param [in] d new dictionary
      */
     void setDictionary(const StringList &d);
     /*!
      * \brief getTokens parse \a expr whith \a parser and copy tokens
      * \param [in] expr string to parse
      * \param [in] parser
      */
     void getTokens(const std::string &expr,Parser &parser);

};

#endif // GENERATOR_H
