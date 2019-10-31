#pragma once

class Project
{
public:
  virtual void draw() = 0;
  virtual void draw_editors() = 0;
  virtual void draw_menus() = 0;

  virtual void reset() = 0;

  virtual std::string name()
  {
    return "Project";
  }
};
