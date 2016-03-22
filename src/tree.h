#ifndef TREE_H
#define TREE_H

#include "tokens.h"
#include "policy.h"


/*!
 * \brief The Tree class represents AST tree of EBNF grammar
 */
class Tree
{
    friend class TreeBuilder;

    using CTContainer =  std::deque<std::shared_ptr<CustomToken> >;     ///Custom Tokens Container
    using OTContainer =  std::deque<std::shared_ptr<OrToken> >;         ///Or     Tokens Container
    using SBTContainer = std::deque<std::shared_ptr<SquareBraceToken> >;///[]     Tokens Container

    BasePtr      top_;           /// top of this tree
    StringList   result_;        /// generated results
    CTContainer  customTokens_;  /// vector of all custom tokens in this tree
    OTContainer  orTokens_;      /// vector of all Or tokens in this tree
    SBTContainer SBTokens_;      /// vector of all [] tokens in this tree
    std::string  name_;          /// current tree name, for linking
    bool         treeValid_;     /// true if this tree valid(can reproduce results)
  //bool         canChange_;     /// indicates this tree had randomisation in generating
    StringList   dictionary_;    /// user defined values this tree can generate
    PolicyPtr    addingPolicy_;  /// what elements add to the results

    /*!
     * \brief adjustResults \a rt
     * \param [in/out] rt adjustable results
     *
     * add dictionary_ and aply addingPolicy_ to rt;
     */
    void adjustResults(StringList &rt);/// adjusts results \a rt adds dictionary
public:
    /*!
     * \brief Tree constructs empty tree
     */
    Tree()
    {
        refresh();
    }

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
    // void buildTree(const std::string &expr, ConstStrIt &begin);
    // void buildTreeWithBuilder(const std::string &expr, ConstStrIt &begin);
    /*!
     * \brief generate
     * \param reGenerate
     * \throw routines::DGException if errors occur
     * generates results if \a reGenerate true
     */
    void generate(bool reGenerate);
    /*!
        * \brief generateAndGet for async generation
        * \param [out] rt container to store results
        *
        * generates results in \a rt using only on stack memory. Can be used to asynk using;
        */
    void generateAndGet(StringList &rt);
    /*!
     * \brief setDictionary sets current dictionary to \a d
     * \param d new dictionary
     */
    void setDictionary(const StringList &d)
    {
        dictionary_ = d;
        addingPolicy_->update(d);
        result_.clear();
    }
    /*!
     * \brief setPolicy sets \a addingPolicy_ to \a newPolicy
     * \param newPolicy user defined policy(derrived from Policy)
     */
    void setPolicy(PolicyPtr newPolicy){
        if(newPolicy)//if pointer to null was passed
        {
            addingPolicy_ = newPolicy;
            addingPolicy_->update(dictionary_);
        }
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
    //bool canChange()                                const {  return canChange_;   }
    /*!
     * \brief isValid getter func to Tree::treeValid_
     * \return Tree::treeValid_ true - if this tree can generate results
     */
    bool isValid()                                  const {  return treeValid_;   }
    /*!
     * \brief refresh this tree
     */
    void refresh();
    /*!
     * \brief set Probabilities of elements adding in OrToken
     * \param [in] tokenNumber number of element in orTokens_
     * \param [in] probabilities probabilities to set up
     *
     * sets up \a probabilities in orTokens_[tokenNumber]
     */
    void setProbabilities(size_t tokenNumber,const std::list<int>& probabilities);
    /*!
     * \brief setProbability of elements adding in SquareBraceToken
     * \param [in] tokenNumber number of element in orTokens_
     * \param [in] probabiliy probability to set up
     *
     * sets up \a probability in SBTokens_[tokenNumber]
     */
    void setProbability(size_t tokenNumber,int probability);
};



#endif //TREE



