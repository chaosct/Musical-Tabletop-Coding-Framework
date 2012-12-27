#ifndef OSC_TEXT_HPP_INCLUDED
#define OSC_TEXT_HPP_INCLUDED

#include "osc_common_draw.hpp"


class GraphicObjectData
{
    protected:
        float x,y,angle;
        int r,g,b;
    public:

        GraphicObjectData():x(0),y(0),angle(0),r(255),g(255),b(255)
        {
        }
        void SetColor(int _r,int _g, int _b){ r = _r; g = _g; b = _b;}
        void SetPosition(float _x, float _y){ x = _x; y = _y;}
        void SetAngle(float _angle){angle = _angle;}
};

class text : public GraphicObjectData, public Graphic
{
    protected:
        string data;
        static ofTrueTypeFont & font()
        {
            static bool init = true;
            static ofTrueTypeFont	verdana;
            if (init)
            {
                verdana.loadFont(ofxGlobalConfig::getRef<std::string>("PROGRAM:HELPFONT","verdana.ttf"),32);
            }
            return verdana;
        }
    public:

        text(){}
        void SetText(std::string text)
        {
            int find = text.find("%20");
            while(find != string::npos)
            {
                text.replace(find,3," ");
                find = text.find("%20");
            }
            data = text;
        }

        void draw()
        {
            ofPushMatrix();
            ofSetColor(r,g,b);

            ofTranslate(x,y);
            ofRotate(angle);
            ofScale(0.0005f,0.0005f,1);
            font().drawString(data,0,0);

            ofPopMatrix();
            //std::cout << x << " " << y << " " << data <<std::endl;
        }
};


class OSCTextObject: public OSCCommonDrawObject, public text
{
public:
int id;
    void cmd_color(int r,int g,int b)
    {
        SetColor(r,g,b);
    }
    void cmd_position(float x,float y,float angle)
    {
            SetPosition(x,y);
            SetAngle(angle);
    }
    void cmd_hidden(bool ishidden)
    {
         std::cout << "TODO: hidden not implemented" << std::endl;
    }
    void run_extra(const std::string & cmd, OscOptionalUnpacker & msg)
    {
        std::cout << cmd << std::endl;
        if(cmd == "write")
        {
            std::string data;
            msg >> data;
            SetText(data);
        }
        else if(cmd == "clear")
        {
            SetText("");
        }
    }
};

class OscTextDraw
{
public:
    OSCCommonDraw<OSCTextObject> o;
    OscTextDraw():o("/text"){}
};


#endif // OSC_TEXT_HPP_INCLUDED
