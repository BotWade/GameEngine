#ifndef _BUTTON_
#define _BUTTON_

#include "../InterfaceObject.hpp"

#include <string>

using namespace std;

class Button : public InterfaceObject {
public:
    string Text;
    
    virtual void Render() override;

};

#endif