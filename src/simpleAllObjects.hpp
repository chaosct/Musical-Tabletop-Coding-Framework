#ifndef SIMPLEALLOBJECTS_HPP_INCLUDED
#define SIMPLEALLOBJECTS_HPP_INCLUDED

#include "Graphic.hpp"
#include <map>
#include "InputGestureDirectObjects.hpp"

class SimpleAllObjects: public OnTable < tuio::CanDirectObjects < Graphic > >, public std::map<unsigned int,tuio::DirectObject *>
{
    public:
    void newObject(tuio::DirectObject * object)
    {
        (*this)[object->f_id]=object;
    }
    void removeObject(tuio::DirectObject * object)
    {
        erase(object->f_id);
    }
    bool isOnTable(unsigned int f)
    {
        return find(f) != end();
    }
};

#endif // SIMPLEALLOBJECTS_HPP_INCLUDED
