#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include "Ball.h"
#include "Bat.h"
#include "Goal.h"
//#include "Game.h"
#include "Box.h"
//#include "Menu.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

#include <cstdlib>
#include <iostream>
//#include <QString>
#include <ctime>


//----------------------------------------------------------------------------------------------------------------------

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    setTitle("Dink!");
    m_redrawTimer=startTimer(20);
    //srand(time(NULL));
    m_currentScore = 0;
    m_batMove = true;
    m_batPush = false;
    m_batPos = ngl::Vec3(0,0,(-(15+s_batGap)));

    // now clear the key set
    m_keysPressed.clear();

}

//----------------------------------------------------------------------------------------------------------------------

NGLScene::~NGLScene()
{
    ngl::NGLInit *Init = ngl::NGLInit::instance();
    std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
    delete m_light,m_ball, m_box, m_goal,m_bat;
    Init->NGLQuit();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::resizeEvent(QResizeEvent *_event )
{
  if(isExposed())
  {
  int w=_event->size().width();
  int h=_event->size().height();
  // set the viewport for openGL
  glViewport(0,0,w,h);
  // now set the camera size values as the screen size has changed
  m_cam->setShape(45,(float)w/h,0.05,350);
  renderLater();
  }
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::initialize()
{

    ngl::NGLInit::instance();

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);			   //Plain White Background

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //shader setup
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    shader->createShaderProgram("Phong");

    shader->attachShader("PhongVertex",ngl::VERTEX);
    shader->attachShader("PhongFragment",ngl::FRAGMENT);

    shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
    shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");

    shader->compileShader("PhongVertex");
    shader->compileShader("PhongFragment");

    shader->attachShaderToProgram("Phong","PhongVertex");
    shader->attachShaderToProgram("Phong","PhongFragment");

    shader->bindAttribute("Phong",0,"inVert");

    shader->bindAttribute("Phong",1,"inUV");

    shader->bindAttribute("Phong",2,"inNormal");

    shader->linkProgramObject("Phong");

    (*shader)["Phong"]->use();

    //camera setup
    ngl::Vec3 from(0,-2,-40);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);

    m_cam= new ngl::Camera(from,to,up);

    m_cam->setShape(45.0f,(float)720.0/576.0f,0.05f,350.0f);
    shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

    //directional white light setup
    m_light = new ngl::Light(ngl::Vec3(-3,-3,4),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::DIRECTIONALLIGHT );
    ngl::Mat4 iv=m_cam->getViewMatrix();
    iv.transpose();
    m_light->setTransform(iv);
    m_light->loadToShader("light");


    glViewport(0,0,width()*devicePixelRatio(),height()*devicePixelRatio());

    //m_title = new ngl::Text (QFont("Arial", 32, QFont::Bold));
    //m_title->setScreenSize(width(),height());
    //m_title->setColour(1,1,1);

    m_text = new ngl::Text (QFont("Arial", 16));
    m_text->setScreenSize(width(),height());
    m_text->setColour(0.2f,0.2f,0.2f);

    //set up the game
    //m_dink = new Game();
    //setupGameState(Game::STARTSCREEN);


    m_box = new Box();
    ngl::Real boxWidth, boxHeight, boxDepth;
    boxWidth = m_box->getWidth();
    boxHeight = m_box->getHeight();
    boxDepth = m_box->getDepth();

    m_ball = new Ball();
    //m_ball->generatePos();
    m_ball->generatePos(boxWidth);
   // m_ball->generateVel();
    m_ball->generateVel();

    m_goal = new Goal();
    //m_goal->generatePos();
    m_goal->generatePos(boxWidth,boxHeight,boxDepth);


    m_bat = new Bat ();
    m_bat->setMousePos(m_batPos);

      glViewport(0,0,width(),height());
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::loadMatricesToShader()
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Mat4 MVP=m_cam->getVPMatrix()  ;

    shader->setShaderParamFromMat4("MVP",MVP);
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::render()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //drawScene(m_dink->getGameState(),"Phong");


    ngl::Material m(ngl::CHROME);
    m.loadToShader("material");
    m_box->draw("Phong",m_cam);

    m.set(m_ball->getMaterial());
    m.loadToShader("material");
    m_ball->draw("Phong",m_cam);

    m.set(ngl::BRASS);
    m.loadToShader("material");
    m_goal->draw("Phong",m_cam);

    m.set(ngl::GOLD);
    m.loadToShader("material");
    m_bat->draw("Phong",m_cam);

//    (*shader)["nglTextShader"]->use();
    QString score = QString::number(m_currentScore);
    QString scoreText = "Score: "+score;
    m_text->renderText(10,10,scoreText);



}
//----------------------------------------------------------------------------------------------------------------------
//This changes the gameState and applies any changes to the data each effects, ready to be called in drawScene()

//NOTES
//Is killTimer() the effective way of pausing the game?<-------------------
//

//void NGLScene::setupGameState(   const Game::GameState _gameState)
//{
//    switch(_gameState)
//    {
//    case Game::STARTSCREEN:
//    {
//        m_dink->setGameState(Game::STARTSCREEN);
//    }
//    case Game::MENU:
//    {
//        m_dink->setGameState(Game::MENU);
//        m_menu = new Menu();
//    }
//    case Game::INGAME:
//    {
//        m_dink->setGameState(Game::INGAME);
//        m_dink->setCurrentScore(0);

//        m_batUpdateTimer=startTimer(20);
//        m_ballUpdateTimer=startTimer(10);

//        m_box = new Box((0,0,0),6,5,0);//position needs calucating

//        m_ball = new Ball();
//        m_ball->setPosition(m_ball->generatePos(m_box->getWidth(),m_box->getHeight(),m_box->getDepth()));
//        m_ball->setVelocity(m_ball->generateVel());

//        m_bat = new Bat ((0,0,0));//position needs calucating

//    }
//    case Game::HOWTO:
//    {
//        m_dink->setGameState(Game::HOWTO);
//    }
//    case Game::HIGHSCORES:
//    {
//        m_dink->setGameState(Game::HIGHSCORES);
//    }
//    case Game::PAUSED:
//    {
//        m_dink->setGameState(Game::PAUSED);
//        //m_batUpdateTimer=killTimer(20);
//        //m_ballUpdateTimer=killTimer(10);
//    }
//    case Game::GAMEOVER:
//    {
//        m_dink->setGameState(Game::GAMEOVER);
//        //m_batUpdateTimer=killTimer(20);
//        //m_ballUpdateTimer=killTimer(10);
//        int newScore = m_dink->getCurrentScore();
//        if(newScore > m_dink->getHighscore3())
//        {
//            if(newScore >= m_dink->getHighscore2())
//            {
//                if(newScore >= m_dink->getHighscore1())
//                {
//                    m_dink->setHighscore3(m_dink->getHighscore2());
//                    m_dink->setHighscore2(m_dink->getHighscore1());
//                    m_dink->setHighscore1(newScore);
//                }
//                else
//                {
//                    m_dink->setHighscore3(m_dink->getHighscore2());
//                    m_dink->setHighscore2(newScore);
//                }
//            }
//            else m_dink->setHighscore3(newScore);
//        }
//    }
//    }
//}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)


{
    if(m_batMove && _event->buttons() == Qt::NoButton)//-------------< ISSUE #2

    {
        int w=1024;
        int h=720;
        int offsetX = _event->x() - (w/2);
        int offsetY = _event->y() - (h/2);

        ngl::Real x = offsetX*-0.03;
        ngl::Real y = offsetY*-0.03;
        ngl::Real z = -(15+s_batGap);
        m_batPos.set(x,y,z);
        m_bat->setMousePos(m_batPos);


        //jons
//        int diffX = (int)(_event->x() - m_origXPos);
//        int diffY = (int)(_event->y() - m_origYPos);
//        m_origXPos=_event->x();
//        m_origYPos=_event->y();
//        m_batPos.m_x += 0.03 * diffX;
//        m_batPos.m_y -= 0.03 * diffY;
//        m_bat->setMousePos(m_batPos);

        renderLater();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
//    if(_event->button() == Qt::LeftButton || m_dink->getGameState() == Game::INGAME)
//    {
//        m_bat->pushStart();
//        m_batPush=true;
//        renderLater();
//    }
//    if(_event->button() == Qt::LeftButton)
//    {
//        m_bat->pushStart();
//        m_batPush=true;
//        renderLater();
//    }
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
//    switch (m_dink->getGameState())
//    {
//    case Game::STARTSCREEN :
//    {
//        //press Space key to start
//        if (_event->key() == Qt::Key_Space)
//            setupGameState(Game::MENU); break;
//        if (_event->key() == Qt::Key_Escape)
//            QGuiApplication::exit(EXIT_SUCCESS); break;
//    }


//    case Game::MENU :
//    {
//        switch (_event->key())
//        {
//        // escape key to quit
//        case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
//        case Qt::Key_Up : m_menu->moveUp(); break;
//        case Qt::Key_Down : m_menu->moveDown(); break;
//        case Qt::Key_Space :
//            if(m_menu->getHighlighted() == 1)
//            {
//                setupGameState(Game::INGAME);
//            }
//            else if(m_menu->getHighlighted() == 2)
//            {
//                setupGameState(Game::HOWTO);
//            }
//            else
//            {
//                setupGameState(Game::HIGHSCORES);
//            }
//            break;
//        default : break;
//        }
//        renderLater();
//    }
//    case Game::INGAME :
//    {
//        switch (_event->key())
//        {
//        case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
//        case Qt::Key_M : setupGameState(Game::MENU); break;
//        case Qt::Key_P :
//        {
//            setupGameState(Game::PAUSED);
//            //m_batUpdateTimer=killTimer(20);<-- i know im using these wrong
//            //m_ballUpdateTimer=killTimer(10);
//        }
//            break;
//        default : break;
//        }
//    }
//    case Game::PAUSED:
//    {
//        switch (_event->key())
//        {
//        case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
//        case Qt::Key_M : setupGameState(Game::MENU); break;
//        case Qt::Key_P : m_dink->resumeIngame(); break;// needs checking
//        default : break;
//        }
//    }
//    case Game::HOWTO:
//    {
//        switch (_event->key())
//        {
//        case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
//        case Qt::Key_Backspace : setupGameState(Game::MENU); break;
//        default : break;
//        }
//    }
//    case Game::HIGHSCORES:
//    {
//        switch (_event->key())
//        {
//        case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
//        case Qt::Key_Backspace :setupGameState(Game::MENU); break;
//        default : break;
//        }
//    }
//    case Game::GAMEOVER:
//    {
//        switch (_event->key())
//        {
//        case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
//        case Qt::Key_Backspace :setupGameState(Game::MENU); break;
//        default : break;
//        }
//    }
//    }
}

//----------------------------------------------------------------------------------------------------------------------

//void NGLScene::drawScene(   const Game::GameState _gameState, const std::string &_shader)
//{
//    //Am I using this shader parameter correctly?<-------------------
//    //
//    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
//    (*shader)[_shader]->use();
//    switch(_gameState)
//    {
//   case Game::STARTSCREEN:
//    {
//        m_text->renderText(400,400,"Press [Spacebar] to begin");
//        m_title->renderText(360,180,"Dink!");
//        //draw Dink! logo
//    }
//    case Game::MENU:
//    {
//        m_title->renderText(20,60,"Dink!");
//        m_text->renderText(20,40,"Play Game");
//        m_text->renderText(20,80,"How To Play");
//        m_text->renderText(20,100,"Highscores");
//        //create a cursor
//    }
//    case Game::INGAME:
//    {
//        QString score = QString::number(m_dink->getCurrentScore());
//        QString scoreText = "Score: "+score;
//        m_text->renderText(10,10,scoreText);
//        m_text->renderText(60,10,"press [P] to pause         press [M] to return to menu");
//       ngl::Material m(ngl::CHROME);
//        //m.loadToShader("material");
//       // m_box->draw("Phong",m_cam);<---------not drawing box, under construction

//        m.set(m_ball->getMaterial());
//        m.loadToShader("material");
//        m_ball->draw("Phong",m_cam);

//        m_bat->setMousePos(m_batPos);
//        m.set(m_bat->getMaterial());
//        m.loadToShader("material");
//        m_bat->draw("Phong",m_cam);
//    }
//    case Game::HOWTO:
//    {
//        m_text->renderText(60,50, "how to notes TO DO");
//        m_text->renderText(60,10,"press [Backspace] to return to menu");

//    }

//    case Game::HIGHSCORES:
//    {
//        QString score1 = QString::number(m_dink->getHighscore1());
//        QString score2 = QString::number(m_dink->getHighscore2());
//        QString score3 = QString::number(m_dink->getHighscore3());
//        QString score1Text = "1st:     "+score1;
//        QString score2Text = "2nd:     "+score2;
//        QString score3Text = "3rd:     "+score3;

//        m_title->renderText(20,60, "Highscores");
//        m_text->renderText(20,40, score1Text);
//        m_text->renderText(20,30, score2Text);
//        m_text->renderText(20,20, score3Text);
//        m_text->renderText(60,10,"press [Backspace] to return to menu");
//        //show text for highscores and avaiable keys
//    }
//    case Game::GAMEOVER:
//    {
//        QString score1 = QString::number(m_dink->getHighscore1());
//        QString score2 = QString::number(m_dink->getHighscore2());
//        QString score3 = QString::number(m_dink->getHighscore3());
//        QString score1Text = "1st:     "+score1;
//        QString score2Text = "2nd:     "+score2;
//        QString score3Text = "3rd:     "+score3;

//        m_title->renderText(20,60, "Game Over!");
//        m_text->renderText(20,40, score1Text);
//        m_text->renderText(20,30, score2Text);
//        m_text->renderText(20,20, score3Text);
//        m_text->renderText(60,10,"press [P] to pause         press [M] to return to menu");
//        m_text->renderText(60,10,"press [R] to restart");
//    }
//    }

//}


////----------------------------------------------------------------------------------------------------------------------

void NGLScene::updateBat()
{
    //update the position based on mouse movement
    //update the normal based on ball m_pos
    if (m_batPush == false)
    {
        //initialize for bat normal maths
        ngl::Vec3 defaultNormal, newNormal;
        ngl::Vec3 ballPos = m_ball->getPosition();
        //calulate the bat's new normal vector
        defaultNormal = m_bat->getDefaultNormal();
        newNormal.set(ballPos - m_batPos);
        newNormal.normalize();
        m_bat->setNormal(newNormal);
        //initialize for bat orientation maths
        ngl::Vec3 xVec, yVec, zVec;
        xVec = ngl::Vec3(newNormal.m_x,0,0);
        yVec = ngl::Vec3(0,newNormal.m_y,0);
        zVec = ngl::Vec3(0,0,newNormal.m_z);
        //use the dot product to find the angles of each component
        //for x rotation
        ngl::Real dotX, thetaXr,thetaXd;
        dotX = defaultNormal.dot(xVec);
        thetaXr = acos(dotX);//since both vectors are normalized
        thetaXd = thetaXr*(180/M_PI);//convert to degrees
        //for y rotation
        ngl::Real dotY, thetaYr,thetaYd;
        dotY = defaultNormal.dot(yVec);
        thetaYr = acos(dotY);
        thetaYd = thetaYr*(180/M_PI);
        //for z rotation
        ngl::Real dotZ, thetaZr,thetaZd;
        dotZ = defaultNormal.dot(zVec);
        thetaZr = acos(dotZ);
        thetaZd = thetaZr*(180/M_PI);
        //Use the components to set the bat orientation
        ngl::Vec3 batOrient;
        batOrient.set(ngl::Vec3(0, 0, thetaZd));
        m_bat->setOrient(batOrient);
    }
    else
    {
        ngl::Vec3 pushV, pushA, pushPos, newPushV,newPushPos;
        ngl::Real pushDist = m_bat->getPushDist();
        pushV.set(m_bat->getPushV());
        pushA.set(m_bat->getPushA());
        newPushV = pushV+pushA;
        newPushPos = pushPos+pushV;
        m_bat->setPushV(newPushV);
        m_bat->setPushPos(newPushPos);

        if (pushPos.m_z >= pushDist)
        {
            m_bat->pushPeak();
        }
        if (pushPos.m_z <= 0)
        {
            m_bat->pushStop();
        }
    }
    renderLater();
}
////----------------------------------------------------------------------------------------------------------------------

void NGLScene::updateBall()
{
    //update the ball position

    //this will change pos by an increment dictated by the velocity
    //batCollision();
    wallCollision();

    ngl::Vec3 oldVel = m_ball->getVelocity();
    ngl::Vec3 newVel = oldVel - (ngl::Vec3(0,(ngl::Real)0.02,0));
    m_ball->setVelocity(newVel);


    ngl::Vec3 oldPos = m_ball->getPosition();
    ngl::Vec3 newPos = (oldPos + newVel);
    m_ball->setPosition(newPos);

    renderLater();
}

////----------------------------------------------------------------------------------------------------------------------

void NGLScene::wallCollision()
{
    ngl::Vec3 pos = m_ball->getPosition();
    ngl::Real x = pos.m_x, y = pos.m_y, z = pos.m_z, r = m_ball->getRadius();
    ngl::Real width = m_box->getWidth(),height = m_box->getHeight(),depth=m_box->getDepth();
    if(x >= ((width/2)-r) || x <= (r-(width/2)))
    {
        ngl::Vec3 oldVel = m_ball->getVelocity();
        ngl::Vec3 newVel = oldVel * ngl::Vec3(-1,1,1);
        m_ball->setVelocity(newVel);//SIDE
    }
    else if( z >= (depth/2)-r)
    {
        goalCollision();//BACK
    }
    else if( z <= -((depth/2)+s_batGap))
    {
        m_currentScore = 0;
        m_ball->generatePos(width);
        m_ball->generateVel();
        m_goal->generatePos(width,height,depth);

    }
    else if( y >= (height/2)-r)
    {
        ngl::Vec3 oldVel = m_ball->getVelocity();
        ngl::Vec3 newVel = oldVel * ngl::Vec3(1,-1,1);
        m_ball->setVelocity(newVel);//TOP
    }
    else if( y <= r-(height/2))
    {
        m_currentScore = 0;
        m_ball->generatePos(width);
        m_ball->generateVel();
        m_goal->generatePos(width,height,depth);

    }
}
////----------------------------------------------------------------------------------------------------------------------

void NGLScene::batCollision()
{
    ngl::Real planeDistance,centreDistance,magN;
    ngl::Vec3 batPos, ballPos, vecPQ, batNormal;
    ngl::Real ballR = m_ball->getRadius();
    ngl::Real batR = m_bat->getRadius();

    batNormal = m_bat->getNormal();//HERE WE ARE
    vecPQ = batPos-ballPos;
    magN = batNormal.length();
    planeDistance = (vecPQ.dot(batNormal))/magN;
    centreDistance = vecPQ.length();


    if(planeDistance <= ballR || centreDistance <=batR)
    {
        ngl::Vec3 newVel, oldVel;
        oldVel = m_ball->getVelocity();
        newVel = oldVel.reflect(batNormal);
        m_ball->setVelocity(newVel);
        //m_ball->batDeflect(batNormal);
    }

}
////----------------------------------------------------------------------------------------------------------------------

void NGLScene::goalCollision()
{
    ngl::Vec3 PQ, goalPos, ballPos;
    ngl::Real distance, goalR;

    goalPos = m_goal->getPosition();
    ballPos = m_ball->getPosition();

    PQ = goalPos - ballPos;
    goalR = m_goal->getRadius();
    distance = PQ.length();

    if(distance <= goalR)
    {
        ngl::Real boxWidth, boxHeight, boxDepth;
        boxWidth = m_box->getWidth();
        boxHeight = m_box->getHeight();
        boxDepth = m_box->getDepth();

        m_currentScore ++;
        //m_dink->setCurrentScore(score + 1);

        m_ball->generatePos(boxWidth);
        m_ball->generateVel();

        m_goal->generatePos(boxWidth,boxHeight,boxDepth);
        //m_goal->setPosition(ngl::Vec3(0,0,15),ngl::Vec3(0,180,0));
    }
    else
    {
        ngl::Vec3 oldVel = m_ball->getVelocity();
        ngl::Vec3 newVel = oldVel * ngl::Vec3(1,1,-1);
        m_ball->setVelocity(newVel);//TOP
    }
}
////----------------------------------------------------------------------------------------------------------------------

void NGLScene::timerEvent( QTimerEvent *_event)
{
    if (_event->timerId() == m_redrawTimer)
    {

        updateBall();
        updateBat();
        renderNow();
    }
}

