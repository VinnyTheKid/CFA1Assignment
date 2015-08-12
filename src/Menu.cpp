#include <ngl/ShaderLib.h>
#include <QFont>
#include "Menu.h"
#include "Game.h"

Menu::Menu()
{
    m_highlighted = 1;
}


void Menu::moveUp()
{
    if (m_highlighted == 1)
        m_highlighted = 3;
    else
        m_highlighted --;
}

void Menu::moveDown()
{
    if (m_highlighted == 3)
        m_highlighted = 1;
    else
        m_highlighted ++;
}


