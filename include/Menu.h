#ifndef MAINMENU
#define MAINMENU

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/text.h>
#include <ngl/Transformation.h>

//a simple interface for the menu. Ive yet to write code for a cursor to highlight what is selected.

class Menu
    {
public:
    Menu();
    void moveUp();
    void moveDown();
    inline int getHighlighted(){return m_highlighted;}
private:
    int m_highlighted;
};

#endif // MAINMENU

