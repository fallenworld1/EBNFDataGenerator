#include "policy.h"
#include <time.h>


void NearAveragePolicy::add(double size)
{
    averageSize_ = (averageSize_ * elemCount + size)/(elemCount+1);
    ++elemCount;
}

NearAveragePolicy::NearAveragePolicy(double defaultASize,double range):defaultAverageSize_(defaultASize),averageSize_(defaultASize),range_(range)
{
    srand(clock());
}

void NearAveragePolicy::update(const StringList &d)
{
    for(auto &elem:d) add(static_cast<double>(elem.size()));
    defaultAverageSize_ = averageSize_;
}

bool NearAveragePolicy::check(const std::string &elem)
{
    double elemSize = static_cast<double>(elem.size());
    if(elemSize < averageSize_*range_)
    {
        add(elemSize);
        return true;
    }
    else
    {
        if((rand()%100)*averageSize_/elemSize > 80.0)
        {
            add(elemSize);
            return true;
        }
        else return false;

    }

}

void NearAveragePolicy::refresh()
{
    averageSize_ = defaultAverageSize_;
    elemCount   = 1;
}

void MinMaxDPolicy::update(const StringList &dictionary)
{
    if(dictionary.empty())
    {
        min_ =  0;
        max_ = -1;
        return;
    }
    else
    {
        min_ = dictionary.front().size();
        max_ = min_;
    }
    for(const auto &elem:dictionary)
    {
        if(elem.size() < min_) min_ = elem.size();
        else if(elem.size() > max_) max_ = elem.size();
    }
}

bool MinMaxDPolicy::check(const std::string &elem)
{
    return (elem.size() > max_) || (elem.size() < min_);
}
