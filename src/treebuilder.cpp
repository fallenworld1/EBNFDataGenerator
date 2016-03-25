#include "treebuilder.h"
#include "tree.h"
#include <cctype>

const bool TOKENS_MAP[7][7] =
        //CBRACE,NAME,EQUAL,T orC,OBRACE,OPER,end_
{{true ,false,false,false,false,true ,true }, //CBRACE
{false,false,true ,false,false,false,false}, //NAME
{false,false,false,true ,true ,false,false}, //EQUAL
{true ,false,true ,false,false,true ,true }, //T orC
{false,false,false,true ,true ,false,false}, //OBRACE
{false,false,false,true ,true ,false,false}, //OPER
{false,true ,false,false,false,false,false}};//end_


void TreeBuilder::refresh()
{
    customTokens_.clear();
    orTokens_.clear();
    SBTokens_.clear();
    name_.clear();
}

void TreeBuilder::checkType(const TreeBuilder::TokenType &current_)
{
    if(!sequenceControl(current_))
        throw DGException("Tree::buildTree error. Unexpected occurence of: ",*it_,it_-begin_);
}

bool TreeBuilder::sequenceControl(const TreeBuilder::TokenType &current_)
{
    bool result = TOKENS_MAP[(int)lastTokenType_][(int)current_];
    lastTokenType_ = current_;
    return result;
}

void TreeBuilder::processQuote()
{
    std::string temp;
    ++it_;
    routines::readLiteralName(it_,end_,temp);//thows or put begin at closing "
    checkType(TokenType::TEXT_CUSTOM);
    lastToken_ = std::make_shared<TextToken>(temp);
    current_->setChild(lastToken_);
}

void TreeBuilder::processConcat()
{
    checkType(TokenType::OPERATION);
    lastToken_ = std::make_shared<ConcatToken>();
    if(current_->checkType(',') || current_->checkType('|')) lastToken_->setChild(current_);
    else current_->resetChild(lastToken_);
    current_ = lastToken_;
}

void TreeBuilder::processOR()
{
    checkType(TokenType::OPERATION);
    if(!current_->checkType('|'))
    {
        orTokens_.push_back(std::make_shared<OrToken>());
        lastToken_ = orTokens_.back();
        if(current_->checkType(','))lastToken_->setChild(current_);
        else current_->resetChild(lastToken_);
        current_ = lastToken_;
    }
}

void TreeBuilder::processORB()
{

    lastToken_ = std::make_shared<RoundBraceToken>();
    processOB();

}

void TreeBuilder::processOSB()
{

    SBTokens_.push_back(std::make_shared<SquareBraceToken>());
    lastToken_ = SBTokens_.back();
    processOB();
}

void TreeBuilder::processOFB()
{

    lastToken_ = std::make_shared<FigureBraceToken>();
    processOB();
}

void TreeBuilder::processOB()
{
    checkType(TokenType::OBRACE);
    openedBraceStack_.push_back(current_);
    current_->setChild(lastToken_);
    current_ = lastToken_;
}

void TreeBuilder::processCB()
{
    checkType(TokenType::CBRACE);
    if(openedBraceStack_.empty()) throw DGException("Tree::buildTree error. Unexpected closing brace: ",*it_,it_-begin_);
    lastToken_ = openedBraceStack_.back();
    if(lastToken_->checkChildType(*it_))
    {
        current_ = lastToken_;
        openedBraceStack_.pop_back();
    }
    else throw DGException("Tree::buildTree error. Wrong closing brace: ",*it_,it_-begin_);
}

void TreeBuilder::processName()
{
    auto it = it_;
    checkType(TokenType::TEXT_CUSTOM);
    while(it != end_ && (isalnum(*it) || (Literals)*it == Literals::UNDERSCORE)) ++it;
    std::string name(it_,it);

    if(name.empty()) throw DGException("Tree::buildTree error. Wrong literal in token name: ",*it_,it_-begin_);

    auto ct = std::make_shared<CustomToken>(name,nullptr);
    current_->setChild(ct);
    customTokens_.push_back(ct);
    it_=it-1;
}

void TreeBuilder::processEq()
{
    if(!name_.empty()) throw DGException("Tree::buildTree error. Unexpected occurence of: ",*it_,it_-begin_);

    if(customTokens_.size()>1) throw DGException("Tree::buildTree error. Wrong syntax near "+customTokens_.back()->name(),'-',it_-begin_);

    checkType(TokenType::EQUAL);
    name_ = customTokens_.back()->name();
}
void TreeBuilder::processEnd()
{
    checkType(TokenType::END);
    if(!openedBraceStack_.empty()){
        lastToken_ = openedBraceStack_.front();
        char c='(';
        if(!lastToken_->checkType(c))
        {
            c='[';
            if(!lastToken_->checkType(c))c='{';
        }

        throw DGException("Tree::buildTree error. Unterminated brace in string",c,it_-begin_);
    }
    ++it_;
}

void TreeBuilder::processComment()
{
    if(++it_ != end_  &&(Literals)*it_ != Literals::COMMENT)
        throw DGException("Tree::buildTree error. Unexpected symbol: ",'#',it_-begin_);
    auto twice = false;
    while(++it_ != end_ )
    {
        if((Literals)*it_ == Literals::COMMENT)
        {
            if(twice) break;
            else twice = true;
        }
        else twice = false;
    }

}

TreePtr TreeBuilder::makeTree()
{
     TreePtr result = std::make_shared<Tree>();
     result->top_.swap(current_);
     result->customTokens_.swap(customTokens_);
     result->orTokens_.swap(orTokens_);
     result->SBTokens_.swap(SBTokens_);
     result->name_.swap(name_);
     result->treeValid_ = true;
     return result;
}



std::shared_ptr<Tree> TreeBuilder::buildTree(const std::string &expr, ConstStrIt &begin)
{
    if(expr.empty()) throw DGException("Tree::buildTree error. Empty token string.");
    begin_         = begin;
    it_            = begin;
    end_           = std::end(expr);
    lastTokenType_ = TokenType::OBRACE;
    current_       = std::make_shared<RoundBraceToken>();
    openedBraceStack_.clear();
    refresh();

    while(1)
    {
        switch(*it_)
        {
        case Literals::TOKENS_DELIMETR1:
        case Literals::TOKENS_DELIMETR2:
        {
            processEnd();
            begin  = it_;
            return makeTree();
        }
        case Literals::QUOTE              : processQuote();  break;
        case Literals::CONCAT             : processConcat(); break;
        case Literals::OR                 : processOR();     break;
        case Literals::OPEN_ROUND_BRACE   : processORB();    break;
        case Literals::OPEN_SQUARE_BRACE  : processOSB();    break;
        case Literals::OPEN_FIGURE_BRACE  : processOFB();    break;
        case Literals::NAME_BODY_DELIMETR : processEq();     break;
        case Literals::CRBRACE            :
        case Literals::CSBRACE            :
        case Literals::CFBRACE            : processCB();     break;
        case Literals::SPACE              :
        case Literals::TAB                :
        case Literals::CARRIAGE_RETURN_WIN:
        case Literals::CARRIAGE_RETURN    :                  break;
        case Literals::COMMENT            : processComment();break;
        default                           : processName();
        }
        if(++it_ == end_)
        {//we cant get here in normal mode, except situations like ...;...<-spaces
            if(openedBraceStack_.size() || name_.size() || customTokens_.size())
                throw DGException("Tree::buildTree error. Unexpected end of string ",' ',it_-begin_);
            else
            {
                begin  = it_;
                return nullptr;
            }
        }
    }

}
