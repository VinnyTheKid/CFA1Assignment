
#include <Game.h>

Game::Game()
{
    m_currentScore = 0;
    m_highscore_1 = 0;
    m_highscore_2 = 0;
    m_highscore_3 = 0;
    m_gameState = STARTSCREEN;
}

void Game::pauseIngame()
{
    m_gameState = PAUSED;
}

void Game::resumeIngame()
{
    m_gameState = INGAME;
}

void Game::gameRestart()
{
    m_gameState = INGAME;
    m_currentScore = 0;
    //tell ball and goal to generate new positions
}
