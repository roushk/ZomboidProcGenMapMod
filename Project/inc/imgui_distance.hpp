#pragma once

inline bool imgui_point_circle(const ImVec2& point, const ImVec2& center, float radius)
{
  const auto radSq = radius * radius;
  const auto xDiff = point.x - center.x;
  const auto yDiff = point.y - center.y;

  return xDiff * xDiff + yDiff * yDiff < radSq;
}
