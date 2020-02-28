#pragma once
#include "application.hpp"

class ProjectExample : public Project
{
public:
  ProjectExample();

  void draw() final override;
  void draw_editors() final override;
  void draw_menus() final override;

  void reset() final override;

  std::string name() final override;

private:
  int quality = 20; // 20 line segments between each point
  std::vector<ImVec2> polynomial;

  float circleRadius = 5.f;
  std::vector<ImVec2> controlPoints;

  bool showMousePosition = true;
  ImVec2 mouseScreenPos;
  ImVec2 mouseWorldPos;

  // A set of solid colors for drawing onto a dark gray background
  const ImVec4 colorSoftLightGray = { 0.9f, 0.9f, 0.9f, 0.415f };
  const ImVec4 colorSoftDarkGray = { 0.26f, 0.26f, 0.26f, 0.415f };
  const ImVec4 colorSoftLightGreen = { 0.74f, 0.98f, 0.74f, 0.415f };
  const ImVec4 colorSoftBlue = { 0.56f, 0.56f, 0.95f, 1.f };
  const ImVec4 colorSoftWhiteBlue = { 0.74f, 0.74f, 0.98f, 1.f };
};
