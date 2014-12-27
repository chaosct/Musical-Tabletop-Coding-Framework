/*

    Musical Tabletop Coding Framework

    Copyright (c) 2010 Carles F. Julià <carles.fernandez@upf.edu>
    Copyright (c) 2010 Daniel Gallardo Grassot <daniel.gallardo@upf.edu>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef SIMPLEALLOBJECTS_HPP_INCLUDED
#define SIMPLEALLOBJECTS_HPP_INCLUDED

#include "EventClient.hpp"
#include <map>
#include "InputGestureDirectObjects.hpp"

class SimpleAllObjects: public EventClient, public std::map<unsigned int,DirectObject *>{
    public:
    void newObject(InputGestureDirectObjects::newObjectArgs & a)
    {
        DirectObject * object = a.object;
        (*this)[object->f_id]=object;
    }
    void removeObject(InputGestureDirectObjects::removeObjectArgs & a)
    {
        DirectObject * object = a.object;
        erase(object->f_id);
    }
    bool isOnTable(unsigned int f)
    {
        return find(f) != end();
    }
    SimpleAllObjects()
    {
        //registerEvent( Event , &MyClass::method );
        this->registerEvent(InputGestureDirectObjects::newObject,&SimpleAllObjects::newObject);
        this->registerEvent(InputGestureDirectObjects::removeObject,&SimpleAllObjects::removeObject);
    }
};

#endif // SIMPLEALLOBJECTS_HPP_INCLUDED
