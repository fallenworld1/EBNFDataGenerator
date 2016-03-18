#ifndef DEFINES
#define DEFINES
#include <memory>
#include <map>
#include <deque>


using strIt      = std::string::iterator;
using ConstStrIt = std::string::const_iterator;

class BaseToken;
class Tree;
class AddingPolicy;

using BasePtr = std::shared_ptr<BaseToken>;

using TreePtr = std::shared_ptr<Tree> ;

using TreesContainer = std::map<std::string,TreePtr>;
using StringList =     std::deque<std::string>;
using PolicyPtr =      std::shared_ptr<AddingPolicy>;


#endif // DEFINES

