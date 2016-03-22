#ifndef TREEBUILDER_H
#define TREEBUILDER_H
#include "defines.h"
#include "tokens.h"
/*!
 * \brief The TreeBuilder class builds Tree objects from string
 */
class TreeBuilder
{
    /*!
     * \brief The Literals enum
     *
     * just named literals
     */
    enum Literals
    {
        QUOTE               = '\"' ,
        CONCAT              = ','  ,
        OR                  = '|'  ,
        OPEN_ROUND_BRACE    = '('  ,
        OPEN_SQUARE_BRACE   = '['  ,
        OPEN_FIGURE_BRACE   = '{'  ,
        CRBRACE             = ')'  ,
        CSBRACE             = ']'  ,
        CFBRACE             = '}'  ,
        NAME_BODY_DELIMETR  = '='  ,
        TOKENS_DELIMETR1    = ';'  ,
        TOKENS_DELIMETR2    = '.'  ,
        COMMENT             = '#'  ,
        SPACE               = ' '  ,
        TAB                 = '\t' ,
        CARRIAGE_RETURN_WIN = '\r' ,
        CARRIAGE_RETURN     = '\n' ,
        UNDERSCORE          = '_'

    };
    /*!
     * \brief The TokenType enum
     */
    enum class TokenType
    {
        CBRACE      = 0,///closing brace } ] )
        NAME        = 1,///token name name = body;
        EQUAL       = 2,///equal sign
        TEXT_CUSTOM = 3,///text "..." or custom name
        OBRACE      = 4,///opening brace { [ (
        OPERATION   = 5,/// operation , or |
        END         = 6/// termination . or ;

    };
    using CTContainer  = std::deque<std::shared_ptr<CustomToken> >;     ///Custom Tokens Container
    using OTContainer  = std::deque<std::shared_ptr<OrToken> >;         ///Or     Tokens Container
    using SBTContainer = std::deque<std::shared_ptr<SquareBraceToken> >;///[]     Tokens Container

    CTContainer         customTokens_;  /// vector of all custom tokens in builded tree
    OTContainer         orTokens_;      /// vector of all Or tokens in builded tree
    SBTContainer        SBTokens_;      /// vector of all [] tokens in builded tree
    std::string         name_;          /// name of builded tree

    TokenType           lastTokenType_;
    std::list<BasePtr>  openedBraceStack_;
    ConstStrIt          begin_;
    ConstStrIt          it_;
    ConstStrIt          end_;
    BasePtr             current_;
    BasePtr             lastToken_;


private:
    void refresh();                                 /// set tree parametrs to default
    void checkType(const TokenType &current);       /// check last generated token type(throws)
    bool sequenceControl(const TokenType &current); /// check last generated token type(nothrows)
    void processQuote();                            /// process "..."
    void processConcat();
    void processOR();
    void processORB();
    void processOSB();
    void processOFB();
    void processOB();
    void processCB();
    void processName();
    void processEq();
    void processEnd();
    TreePtr makeTree();
public:
    /*!
     * \brief TreeBuilder creates builder object.(Tree parametrs required)
     * \param [out] customTokens  builded tree parametr
     * \param [out] orTokens      builded tree parametr
     * \param [out] SBTokens      builded tree parametr
     * \param [out] name          builded tree parametr
     * \param [out] valid         builded tree parametr
     * \param [out] top           builded tree parametr
     *
     * TreeBuilder object can exists without Tree object
     */
    //TreeBuilder();//(CTContainer &customTokens, OTContainer &orTokens,SBTContainer &SBTokens, std::string &name, bool &valid,BasePtr &top);
    /*!
     * \brief buildTree build tree object
     * \param [in]     expr  string to parse
     * \param [in/out] begin starts build from. and Set  it to end
     */
   TreePtr buildTree(const std::string &expr, ConstStrIt &begin);

};

#endif // TREEBUILDER_H
