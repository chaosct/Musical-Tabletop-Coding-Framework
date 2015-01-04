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

#ifndef OSCREPORTER_HPP_INCLUDED
#define OSCREPORTER_HPP_INCLUDED

#include "dispatcher.hpp"
#include <map>
#include "InputGestureDirectObjects.hpp"
#include "InputGestureDirectFingers.hpp"
#include "EventClient.hpp"
#include "objectGraphics.hpp"

class OscObjectReporter: public OSCCMD, public EventClient{
    public:
        OscObjectReporter();
        void run(ofxOscMessage & m);
        void update();

        void report (DirectObject * obj);
        void reporton (int fid);
        void reportoff (int fid);

        void newObject(InputGestureDirectObjects::newObjectArgs & a);
        void updateObject(InputGestureDirectObjects::updateObjectArgs & a);
        void removeObject(InputGestureDirectObjects::removeObjectArgs & a);

        void newCursor(InputGestureDirectFingers::newCursorArgs & a);
        void updateCursor(InputGestureDirectFingers::updateCursorArgs & a);

    private:
        struct object_data{
            ObjectGraphs graphics;
            bool can_cursors, can_angle;
            float angle;
	    bool onTable;
	    DirectObject* object;
            object_data() : can_cursors(false), can_angle(false),
			    angle(0),
			    onTable(false),
			    object(NULL) {}
        };

        std::map<int, object_data> fiducials;
        std::vector<int> fiducialstoupdate;

        float computeAngle(DirectFinger * finger, ofVec2f &figure);
};

#endif // OSCREPORTER_HPP_INCLUDED
