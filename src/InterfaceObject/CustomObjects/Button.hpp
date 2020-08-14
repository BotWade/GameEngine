#ifndef _BUTTON_
#define _BUTTON_

#include "../InterfaceObject.hpp"

#include <string>

using namespace std;

class Button : public InterfaceObject {
public:
    string Text;
    void (*OnClick)();

    virtual void Update() override;
    virtual void Render() override;

};

#endif