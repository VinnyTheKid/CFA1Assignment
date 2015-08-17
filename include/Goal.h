#ifndef GOAL
#define GOAL

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>
#include <cstdlib>

class Goal
{
public:
  /// @brief ctor
  Goal();
  /// @brief returns the scale of the goal
  inline ngl::Real getRadius(){return m_r;}
  /// @brief returns the position of the goal
  inline ngl::Vec3 getPosition(){return m_pos;}
  /// @brief draws the mesh using the transform stack
  /// @param &_shader the shader used to draw
  /// @param *_cam the viewing camera
  void draw(const std::string &_shader, ngl::Camera *_cam );
  /// @brief Uses a random float generator to give the goal a new position
  /// @param _boxWidth width of the box
  /// @param _boxHeight height of the box
  /// @param _boxDepth depth of the box
  void generatePos(ngl::Real _boxWidth, ngl::Real _boxHeight, ngl::Real _boxDepth);
  /// @brief changes the scale of the goal
  /// @param the current score
  void increaseDifficulty(const int _currentScore);



private:
  /// @brief the goal's position
  ngl::Vec3 m_pos;
  /// @brief the goal's orientation
  ngl::Vec3 m_orientation;
  /// @brief the goal's scale
  ngl::Real m_scale;
  /// @brief the diffculty level of the goal
  int m_difficultyLevel;
  /// @brief the goal's radius
  ngl::Real m_r;
  /// @brief a pointer to the goal's mesh
  ngl::Obj *m_mesh;
  /// @brief the goal's transform stack for drawing
  ngl::Transformation m_transform;

};

#endif // GOAL

