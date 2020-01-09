#pragma once

namespace ImGui
{
  inline bool PointCircle(const ImVec2& point, const ImVec2& center, float radius)
  {
    const auto screenPoint = GetScreenPos(point);
    const auto centerPoint = GetScreenPos(center);
    const auto radSq = radius * radius;
    const auto xDiff = screenPoint.x - centerPoint.x;
    const auto yDiff = screenPoint.y - centerPoint.y;

    return xDiff * xDiff + yDiff * yDiff < radSq;
  }
}
