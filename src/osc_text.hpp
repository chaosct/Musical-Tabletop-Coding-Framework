#ifndef OSC_TEXT_HPP_INCLUDED
#define OSC_TEXT_HPP_INCLUDED

#include <algorithm>
#include "osc_common_draw.hpp"
#include "GenericManager.hpp"
#include "GraphicDispatcher.hpp"
#include "Text.hpp"

class OSCTextObject : public OSCCommonDrawObject
{
    private:
        tableGraphics::Text* text;
        float & textsize;

    public:
        OSCTextObject() : textsize(ofxGlobalConfig::getRef("MTCF:TEXTSIZE",1.0f)){
            GenericManager::get<GraphicDispatcher>()->createGraphic(text);
            text->loadFont(17);
        }

        virtual ~OSCTextObject() {
            GenericManager::get<GraphicDispatcher>()->removeGraphic(text);
        }

        void cmd_report_matrix()
        {
            text->setMatrix(total_matrix);
        }
        void cmd_color(int r,int g,int b,int a)
        {
            text->setColor(ofColor(r, g, b, a));
        }
        void cmd_hidden(bool hide)
        {
            text->setVisible(!hide);
        }
        void cmd_bring_top()
        {
            GenericManager::get<GraphicDispatcher>()->bringTop(text);
        }
        void cmd_set_layer(int _layer)
        {
            GenericManager::get<GraphicDispatcher>()->setLayer(_layer, text);
        }
        void cmd_load_shader(std::string path)
        {
            bool success = text->loadShader(path);
            if(!success){
                ofLogWarning("OSCTextObject::cmd_load_shader()") << "Failed, check shader name: " << path;
            }
        }
        void run_extra(const std::string & cmd, OscOptionalUnpacker & msg)
        {
            if(cmd == "write")
            {
                text->clear();
                std::string data;
                while (!msg.Eos()) {
                    msg >> data;

                    // Replace old-format HTML-formatted spaces
                    for (size_t spacePos = data.find("%20");
			    spacePos != std::string::npos;
			    spacePos = data.find("%20")){
                        data.replace(spacePos, 3, " ");
                    }

                    *text << data << " ";
                }
            }
            else if(cmd == "clear")
            {
                text->clear();
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
