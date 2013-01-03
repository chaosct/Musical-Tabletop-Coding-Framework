#ifndef OSC_TEXT_HPP_INCLUDED
#define OSC_TEXT_HPP_INCLUDED

#include "osc_common_draw.hpp"
#include "FigureGraphic.hpp"



class OSCTextObject : public Graphic, public OSCCommonDrawObject
{
    protected:
        //float x,y,angle;
        int r,g,b;
        bool hidden;
    public:

        OSCTextObject():r(255),g(255),b(255),hidden(false)
        {
        }
        void SetColor(int _r,int _g, int _b){ r = _r; g = _g; b = _b;}

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
            if(hidden)return;
            ofPushMatrix();
            ofSetColor(r,g,b);
            ofMultMatrix(total_matrix);
            ofScale(0.0005f,0.0005f,1);
            font().drawString(data,0,0);

            ofPopMatrix();
        }

public:
int id;
    void cmd_color(int r,int g,int b)
    {
        SetColor(r,g,b);
    }
    void cmd_hidden(bool ishidden)
    {
         hidden = ishidden;
    }
    void cmd_bring_top()
    {
        BringTop();
    }
    void cmd_set_layer(int _layer)
    {
        SetLayer(_layer);
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
        else
        {
            std::cout << "Text: command " << cmd << " not found" << std::endl;
        }
    }
};

class OscTextDraw : public Singleton<OscTextDraw>
{
public:
    OSCCommonDraw<OSCTextObject> o;
    OscTextDraw():o("/text"){}
};


#endif // OSC_TEXT_HPP_INCLUDED
