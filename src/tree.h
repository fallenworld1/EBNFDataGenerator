#ifndef TREE_H
#define TREE_H

#include "tokens.h"
#include "policy.h"



/*!
 * \brief The Tree class represents AST tree of EBNF grammar
 */
class Tree
{
    using CTContainer = std::vector<std::shared_ptr<CustomToken> >;//Custom Tokens Container


    BasePtr     top_;           /// top of this tree
    StringList  result_;        /// generated results
    CTContainer customTokens_;  /// map of all custom tokens linked name to ptr
    std::string name_;          /// current tree name, for linking
    bool        treeValid_;     /// true if this tree valid(can reproduce results)
    bool        canChange_ ;    /// indicates this tree had randomisation in generating
    StringList  dictionary_;    /// user defined values this tree can generate
    PolicyPtr   addingPolicy_;  /// what elements add to the results

    void        adjustResults();/// adjusts results adds dictionary
public:
    /*!
     * \brief Tree constructs empty tree
     */
    Tree():treeValid_(false),canChange_(false),addingPolicy_(std::make_shared<DefaultPolicy>()){}

    Tree(const Tree& other)          =delete;
    Tree operator=(const Tree& other)=delete;
    Tree(Tree&& other)               =delete;
    Tree operator=(Tree&& other)     =delete;
    /*!
     * \brief preCount
     * \return rough count of results that tree can produced
     */
    size_t preCount(){ return top_->preCount();}
    /*!
     * \brief buildTree
     * \param expr
     * \param begin
     *
     * builds tree from pos \a begin  of \a expr to end(\a expr) or ';'
     */
    void buildTree(const std::string &expr, constStrIt &begin);
    /*!
     * \brief generate
     * \param reGenerate
     * \return
     * generates results if \a reGenerate true
     */
    bool generate(bool reGenerate);

    /*!
     * \brief setDictionary sets current dictionary to \a d
     * \param d new dictionary
     */
    void setDictionary(const StringList &d)
    {
        dictionary_ = d;
        addingPolicy_->update(d);
    }
    /*!
     * \brief setPolicy sets \a addingPolicy_ to \a newPolicy
     * \param newPolicy user defined policy(derrived from Policy)
     */
    void setPolicy(PolicyPtr newPolicy){
        addingPolicy_ = newPolicy;
        addingPolicy_->update(dictionary_);
    }
    /*!
     * \brief getResults
     * \return list of generated results
     */
    const StringList&         getResults()          const {  return result_;      }
    /*!
     * \brief getCustomTokensList
     * \return all custom tokens in that tree
     */
    const CTContainer&        getCustomTokensList() const {  return customTokens_;}
    /*!
     * \brief name
     * \return string whith name of that tree
     */
    const std::string & name()                      const {  return name_;        }
    /*!
     * \brief canChange
     * \return copy canChange_ flag
     */
    bool canChange()                                const {  return canChange_;   }
};

#endif //TREE



