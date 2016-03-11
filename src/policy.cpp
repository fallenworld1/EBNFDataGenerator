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

void MinMaxPolicy::update(const StringList &dictionary)
{
    if(dictionary.empty())
    {
        min =  0;
        max = -1;
        return;
    }
    else
    {
        min = dictionary.front().size();
        max = min;
    }
    for(const auto &elem:dictionary)
    {
        if(elem.size() < min) min = elem.size();
        else if(elem.size() > max) max = elem.size();
    }
}

bool MinMaxPolicy::check(const std::string &elem)
{
    return (elem.size() > max) || (elem.size() < min);
}
