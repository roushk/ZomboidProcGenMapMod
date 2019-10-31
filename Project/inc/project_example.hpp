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
  bool drawBox = true, toggleDrawBox = true;
  bool drawCircle = true, toggleDrawCircle = true;
  float circleRadius = 15.f;
  int circleDivisions = 128;
  bool showMousePosition = true;

  // A set of solid colors for drawing onto a dark gray background
  const ImVec4 colorSoftLightGray = { 0.9f, 0.9f, 0.9f, 0.415f };
  const ImVec4 colorSoftDarkGray = { 0.26f, 0.26f, 0.26f, 0.415f };
  const ImVec4 colorSoftLightGreen = { 0.74f, 0.98f, 0.74f, 0.415f };
  const ImVec4 colorSoftBlue = { 0.56f, 0.56f, 0.95f, 1.f };
  const ImVec4 colorSoftWhiteBlue = { 0.74f, 0.74f, 0.98f, 1.f };
};
