#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include "Ball.h"
#include "Bat.h"
#include "Goal.h"
#include "Box.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <cstdlib>



//----------------------------------------------------------------------------------------------------------------------

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Dink!");
  m_redrawTimer=startTimer(20);
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
    m_cam->setShape(45f,(float)w/h,0.05f,350.0f);
    renderLater();
  }
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::initialize()
{
  //INITIALIZATION
  ngl::NGLInit::instance();

  //create a plain white background
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  //GL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  //SHADER SETUP
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

  //use the setup shaders
  (*shader)["Phong"]->use();

  //CAMERA SETUP
  ngl::Vec3 from(0,-2,-40);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  //create camera instance
  m_cam= new ngl::Camera(from,to,up);
  //set camera attributes
  m_cam->setShape(45.0f,(float)720.0/576.0f,0.05f,350.0f);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

  //LIGHTING SETUP
  m_light = new ngl::Light(ngl::Vec3(-3,-3,4),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::DIRECTIONALLIGHT );
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  m_light->setTransform(iv);
  m_light->loadToShader("light");

  //SCENE SETUP
  //create text instance
  m_text = new ngl::Text (QFont("Arial", 16));
  //set text attributes
  m_text->setScreenSize(width(),height());
  m_text->setColour(0.2f,0.2f,0.2f);
  //create box instance
  m_box = new Box();

  //retrieve box attributes for use with other class instances
  ngl::Real boxWidth, boxHeight, boxDepth;
  boxWidth = m_box->getWidth();
  boxHeight = m_box->getHeight();
  boxDepth = m_box->getDepth();

  //create ball instance
  m_ball = new Ball();
  //generate ball attributes
  m_ball->generatePos(boxWidth);
  m_ball->generateVel();

  //create goal instance
  m_goal = new Goal();
  //generate goal attributes
  m_goal->generatePos(boxWidth,boxHeight,boxDepth);

  //create bat intance
  m_bat = new Bat ();
  //set bat's inital position
  m_bat->setMousePos(m_batPos);

  //VIEWPORT SETUP
  glViewport(0,0,width()*devicePixelRatio(),height()*devicePixelRatio());
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

  //create a shader instance to be used for drawing
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  //a single material will be used to draw each object in turn while having its properties set

  //draw the box
  ngl::Material m(ngl::CHROME);
  m.loadToShader("material");
  m_box->draw("Phong",m_cam);

  //draw the ball
  m.set(m_ball->getMaterial());
  m.loadToShader("material");
  m_ball->draw("Phong",m_cam);

  //draw the goal
  m.set(ngl::BRASS);
  m.loadToShader("material");
  m_goal->draw("Phong",m_cam);

  //draw the bat
  m.set(ngl::GOLD);
  m.loadToShader("material");
  m_bat->draw("Phong",m_cam);

  //draw the score
  (*shader)["nglTextShader"]->use();
  QString score = QString::number(m_currentScore);
  QString scoreText = "Score: "+score;
  m_text->renderText(10,10,scoreText);



}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  //use the cursor position to update m_batPos
  if(m_batMove && _event->buttons() == Qt::NoButton)
  {
    int w=1024;
    int h=720;
    int offsetX = _event->x() - (w/2);
    int offsetY = _event->y() - (h/2);

    ngl::Real x = offsetX*-s_INCREMENT;
    ngl::Real y = offsetY*-s_INCREMENT;
    ngl::Real z = -(15+s_batGap);
    m_batPos.set(x,y,z);

    renderLater();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
  //when LMB pressed start a push and disable movement
  if(m_batMove && _event->button() == Qt::LeftButton)
  {
    m_bat->pushStart();
    m_batPush = true;
    m_batMove = false;
  }
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::updateBat()
{
  //update the position based on mouse movement
  m_bat->setMousePos(m_batPos);
  //update the normal based on ball m_pos
  if (m_batPush == false)
  {
    //initialize variables
    ngl::Vec3 newNormal, batOrient;
    ngl::Vec3 ballPos = m_ball->getPosition();
    ngl::Real tanThetaX, thetaXr,thetaXd;
    ngl::Real tanThetaY, thetaYr,thetaYd;

    //calulate the bat's new normal vector
    newNormal.set(ballPos - m_batPos);
    newNormal.normalize();
    m_bat->setNormal(newNormal);

    //x rotation component
    tanThetaX = newNormal.m_y/newNormal.m_z;
    thetaXr = atan(tanThetaX);
    thetaXd = thetaXr*(180/M_PI);

    //y rotation component
    tanThetaY = newNormal.m_x/newNormal.m_z;
    thetaYr = atan(tanThetaY);
    thetaYd = thetaYr*(180/M_PI);

    //Use the components to set the bat orientation
    batOrient.set(ngl::Vec3(-thetaXd,thetaYd,0));
    m_bat->setOrient(batOrient);
  }
  else
  {
    //initialize variables
    ngl::Vec3 pushV, pushA, pushPos, newPushV,newPushPos;
    ngl::Real pushDist = m_bat->getPushDist();
    pushV.set(m_bat->getPushV());
    pushA.set(m_bat->getPushA());
    newPushV.set(ngl::Vec3(pushV+pushA));
    newPushPos.set(ngl::Vec3(pushPos+pushV));
    m_bat->setPushV(newPushV);
    m_bat->setPushPos(newPushPos);

    //determine which stage of push and use to define bat attriibutes through push methods
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
  //check for collisions
  wallCollision();
  batCollision();

  //this will change the balls velocity by an increment s_gravity
  ngl::Vec3 oldVel = m_ball->getVelocity();
  ngl::Vec3 newVel = oldVel - (ngl::Vec3(0,s_gravity,0));//0.02
  m_ball->setVelocity(newVel);

  //this will update the position dictated by the velocity
  ngl::Vec3 oldPos = m_ball->getPosition();
  ngl::Vec3 newPos = (oldPos + newVel);
  m_ball->setPosition(newPos);

  renderLater();
}

////----------------------------------------------------------------------------------------------------------------------

void NGLScene::wallCollision()
{
  //initialize variables
  ngl::Vec3 pos = m_ball->getPosition();
  ngl::Real x = pos.m_x, y = pos.m_y, z = pos.m_z, r = m_ball->getRadius();
  ngl::Real width = m_box->getWidth(),height = m_box->getHeight(),depth=m_box->getDepth();
  //check the different extents of the box
  //for left and right walls, deflect
  if(x >= ((width/2)-r) || x <= (r-(width/2)))
  {
    ngl::Vec3 oldVel = m_ball->getVelocity();
    ngl::Vec3 newVel = oldVel * ngl::Vec3(-1,1,1);
    m_ball->setVelocity(newVel);
  }
  //for the back wall, check for a goal collision. Otherwise deflect ball
  else if( z >= (depth/2)-r)
  {
    goalCollision();
  }
  //for the player missing the ball, reset score and restart
  else if( z <= -((depth/2)+s_batGap))
  {
    m_currentScore = 0;
    m_ball->generatePos(width);
    m_ball->generateVel();
    m_goal->generatePos(width,height,depth);
  }
  //for the ceiling, deflect
  else if( y >= (height/2)-r)
  {
    ngl::Vec3 oldVel = m_ball->getVelocity();
    ngl::Vec3 newVel = oldVel * ngl::Vec3(1,-1,1);
    m_ball->setVelocity(newVel);
  }
  //for the ground, reset score and restart
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
  //initalize varibles
  ngl::Real planeDistance,magPQ;
  ngl::Vec3 batPos, ballPos, vecPQ, batNormal;
  ngl::Real ballR = m_ball->getRadius();
  ngl::Real batR = m_bat->getRadius();

  // work out the distance from the bat's centre and the distance from the bat's plane
  batPos = m_batPos;
  ballPos = m_ball->getPosition();
  batNormal = m_bat->getNormal();
  vecPQ = batPos-ballPos;
  planeDistance = vecPQ.dot(batNormal);
  magPQ = vecPQ.length();

  //check for a collision. If so, deflect
  if(planeDistance <= ballR && magPQ <=batR)
  {
    ngl::Vec3 newVel, oldVel;
    oldVel = m_ball->getVelocity();
    newVel = oldVel.reflect(batNormal);
    m_ball->setVelocity(newVel);
  }

}
////----------------------------------------------------------------------------------------------------------------------

void NGLScene::goalCollision()
{
  //initialize varibles
  ngl::Vec3 PQ, goalPos, ballPos;
  ngl::Real distance, goalR;
  goalPos = m_goal->getPosition();
  ballPos = m_ball->getPosition();
  PQ = goalPos - ballPos;
  goalR = m_goal->getRadius();
  distance = PQ.length();
  //check for collisions
  if(distance <= goalR)
  {
    ngl::Real boxWidth, boxHeight, boxDepth;
    boxWidth = m_box->getWidth();
    boxHeight = m_box->getHeight();
    boxDepth = m_box->getDepth();
    //increase the score
    //increase the difficult every 5 points
    m_currentScore ++;
    m_ball->generatePos(boxWidth);
    m_ball->generateVel();
    m_goal->generatePos(boxWidth,boxHeight,boxDepth);
    m_goal->increaseDifficulty(m_currentScore);
  }
  else
  //deflect
  {
    ngl::Vec3 oldVel = m_ball->getVelocity();
    ngl::Vec3 newVel = oldVel * ngl::Vec3(1,1,-1);
    m_ball->setVelocity(newVel);
  }
}
////----------------------------------------------------------------------------------------------------------------------

void NGLScene::timerEvent( QTimerEvent *_event)
{
  //on each timer call update the objects before rendering the scene
  if (_event->timerId() == m_redrawTimer)
  {
    updateBall();
    updateBat();
    renderNow();
  }
}

