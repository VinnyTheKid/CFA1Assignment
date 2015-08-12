#ifndef GAME
#define GAME


#include <ngl/Vec3.h>
#include <ngl/text.h>


//A class to manipulate what needs to be rendered at what stage in the interface

//This is where the enum gamestate comes in.

class Game
{
public:
    Game();
    enum GameState {STARTSCREEN,MENU,INGAME,HOWTO,HIGHSCORES,PAUSED,GAMEOVER};
    inline void setCurrentScore(const int _currentScore){m_currentScore = _currentScore;}
    inline int getCurrentScore(){return m_currentScore;}
    inline void setGameState(const GameState _gameState){m_gameState = _gameState;}
    inline GameState getGameState(){return m_gameState;}
    inline void setHighscore1(const int _highscore1){m_highscore_1 = _highscore1;}
    inline int getHighscore1(){return m_highscore_1;}
    inline void setHighscore2(const int _highscore2){m_highscore_2 = _highscore2;}
    inline int getHighscore2(){return m_highscore_2;}
    inline void setHighscore3(const int _highscore3){m_highscore_3 = _highscore3;}
    inline int getHighscore3(){return m_highscore_3;}
    void pauseIngame();//depricated?
    void resumeIngame();//depricated?
    void gameRestart();//depricated?

private:
    int m_currentScore, m_highscore_1, m_highscore_2, m_highscore_3;
    GameState m_gameState;
};


#endif // GAME

