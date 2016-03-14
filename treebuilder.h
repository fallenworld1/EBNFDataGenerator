#ifndef TREEBUILDER_H
#define TREEBUILDER_H

#include <stack>

//#include "defines.h"
#include "tokens.h"
struct BuildingData
{
    enum Literals
    {
        QUOTES              = '\"',
        CONCAT              = ',' ,
        OR                  = '|' ,
        OPEN_ROUND_BRACE    = '(' ,
        OPEN_SQUARE_BRACE   = '[' ,
        OPEN_FIGURE_BRACE   = '{' ,
        CRBRACE             = ')' ,
        CSBRACE             = ']' ,
        CFBRACE             = '}' ,
        NAME_BODY_DELIMETR  = '=' ,
        TOKENS_DELIMETR     = ';' ,
        COMMENT             = '#' ,
        SPACE               = ' ' ,
        TAB                 = '\t',
        CARRIAGE_RETURN_WIN = '\r',
        CARRIAGE_RETURN     = '\n',
        VISIBLE_SPACE        = '_'


    };

    using CTContainer = std::vector<std::shared_ptr<CustomToken> >;//Custom Tokens Container

    std::stack<BasePtr> tokenStack_;
    ConstStrIt          begin_;
    ConstStrIt          &it_;
    ConstStrIt          end_;
    BasePtr             current_;
    BasePtr             top_;
    bool                &canChange_;
    bool                finished_;
    CTContainer         &customTokens_;  /// map of all custom tokens linked name to ptr
    std::string         &name_;

    BuildingData(ConstStrIt  &it, bool &canChange,CTContainer &customTokens,std::string &name):
        it_(it),
        canChange_(canChange),
        customTokens_(customTokens),
        name_(name)
    {}

    void quotes          ();
    void concat          ();
    void or_             ();
    void openFBrace      ();
    void openRBrace      ();
    void openSBrace      ();
    void closeBrace      ();
    void nameBodyDelimetr();
    void tokenDilemtr    ();
    void comment         ();
    void insignificant   ();
    void defaultProc     ();
};

class TreeBuilder
{


    using Operation =  void ((BuildingData::*)());
    Operation operations[256];



    void initFuncTable();
    TreeBuilder()
    {
        initFuncTable();
    }
  public:
    static const TreeBuilder& getBuilder();
    TreeBuilder(const TreeBuilder&)                  = delete;
    TreeBuilder(TreeBuilder&&)                       = delete;
    const TreeBuilder& operator=(const TreeBuilder&) = delete;
    const TreeBuilder& operator=(TreeBuilder&&)      = delete;

    void buildTree(Tree &tree,const std::string &str, ConstStrIt &begin) const;
};

#endif // TREEBUILDER_H
