#include "treebuilder.h"

#include "tree.h"
#include "tokens.h"
#include <cctype>

void BuildingData::quotes()
{
    std::string temp;
    ++it_;
    routines::readLiteralName(it_,end_,temp);//thows or put it_ at closing "
    BasePtr tt = std::make_shared<TextToken>(temp);
    current_->setChild(tt);
}

void BuildingData::concat()
{
    BasePtr ct = std::make_shared<ConcatToken>();
    current_->resetChild(ct);
    current_   = ct;
    canChange_ = true;
}

void BuildingData::or_()
{
    BasePtr ct = std::make_shared<OrToken>();
    current_->resetChild(ct);
    current_ = ct;
}

void BuildingData::openFBrace()
{
    BasePtr fbt = std::make_shared<FigureBraceToken>();
    tokenStack_.push(current_);
    current_->setChild(fbt);
    current_ = fbt;
    canChange_=true;
}

void BuildingData::openRBrace()
{
    BasePtr bt = std::make_shared<RoundBraceToken>();
    tokenStack_.push(current_);
    current_->setChild(bt);
    current_ = bt;
}

void BuildingData::openSBrace()
{
    BasePtr sbt = std::make_shared<SquareBraceToken>();
    tokenStack_.push(current_);
    current_->setChild(sbt);
    current_ = sbt;
}

void BuildingData::defaultProc()
{
    if(isalnum(*it_) || *it_ == VISIBLE_SPACE)
    {//probably custom token name
        auto it = it_;
        do
        {
            if(++it == end_)    return;// throw DGException("Unexpected end of string.",*it_,it_-end_);
        }
        while(isalnum(*it) || *it == VISIBLE_SPACE);
        std::string name(it_,it);
        auto ct = std::make_shared<CustomToken>(name,nullptr);
        current_->setChild(ct);
        customTokens_.push_back(ct);
        it_=it-1;
        canChange_=true;//TODO add smart changing flag, checks for changing abilit_y of all custom tokens after full parsing
    }
    else throw DGException("Wrong lit_eral: ",*it_,it_-begin_);
}

void BuildingData::closeBrace()
{
    BasePtr last = tokenStack_.top();
    if(last->checkChildType(*it_))
    {
        current_ = last;
        tokenStack_.pop();
    }
    else throw DGException("Wrong closing brace: ",*it_,it_-begin_);
}

void BuildingData::nameBodyDelimetr()
{
    if(!name_.empty()) DGException("Unexpected occurence of: ",*it_,it_-begin_);
    name_ = customTokens_.back()->name();
    canChange_ = false;
}

void BuildingData::tokenDilemtr()
{
    finished_ = true;//TODO return here
}

void BuildingData::comment()
{
    while(++it_ != end_ && *it_ != COMMENT);
}

void BuildingData::insignificant()
{

}

void TreeBuilder::initFuncTable()
{
    for(int i=0;i<256;++i)
        operations[i] = &BuildingData::defaultProc;

    operations[BuildingData::QUOTES]             = &BuildingData::quotes;
    operations[BuildingData::CONCAT]             = &BuildingData::concat;
    operations[BuildingData::OR]                 = &BuildingData::or_;
    operations[BuildingData::OPEN_ROUND_BRACE]   = &BuildingData::openRBrace;
    operations[BuildingData::OPEN_SQUARE_BRACE]  = &BuildingData::openSBrace;
    operations[BuildingData::OPEN_FIGURE_BRACE ] = &BuildingData::openFBrace;
    operations[BuildingData::CRBRACE]            = &BuildingData::closeBrace;
    operations[BuildingData::CSBRACE]            = &BuildingData::closeBrace;
    operations[BuildingData::CFBRACE]            = &BuildingData::closeBrace;
    operations[BuildingData::NAME_BODY_DELIMETR] = &BuildingData::nameBodyDelimetr;
    operations[BuildingData::TOKENS_DELIMETR]    = &BuildingData::tokenDilemtr;
    operations[BuildingData::COMMENT]            = &BuildingData::comment;
    operations[BuildingData::SPACE]              = &BuildingData::insignificant;
    operations[BuildingData::TAB]                = &BuildingData::insignificant;
    operations[BuildingData::CARRIAGE_RETURN_WIN]= &BuildingData::insignificant;
    operations[BuildingData::CARRIAGE_RETURN]    = &BuildingData::insignificant;
    operations[BuildingData::VISIBLE_SPACE]       = &BuildingData::insignificant;
}

const TreeBuilder &TreeBuilder::getBuilder()
{
    static TreeBuilder builder;
    return builder;
}

void TreeBuilder::buildTree(Tree &tree, const std::string &str, ConstStrIt &begin) const
{

        BuildingData data(begin,tree.canChange_,tree.customTokens_,tree.name_);
        data.begin_ = std::begin(str);
        data.end_  =std::end(str);

        data.finished_  = false;
        data.current_ = std::make_shared<RoundBraceToken>();
        data.top_ = data.current_;

        while(!data.finished_)
        {
            if(data.it_ == data.end_) throw DGException("Unexpected end of string: ",*data.it_,data.it_-data.begin_);
            (data.*(operations[*data.it_]))();
            ++data.it_;
        }

        tree.treeValid_ = true;
}

