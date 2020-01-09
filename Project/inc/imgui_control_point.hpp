#pragma once
#include "enumerate.hpp"

enum ImGuiControlPointFlags_
{
  ImGuiControlPointFlags_None       = 0,
  ImGuiControlPointFlags_FixX       = 1 << 1,
  ImGuiControlPointFlags_FixY       = 1 << 2,

  ImGuiControlPointFlags_ClampX     = 1 << 3,
  ImGuiControlPointFlags_ClampY     = 1 << 4,

  ImGuiControlPointFlags_FixXY      = ImGuiControlPointFlags_FixX | ImGuiControlPointFlags_FixY,
  ImGuiControlPointFlags_ClampXY    = ImGuiControlPointFlags_ClampX | ImGuiControlPointFlags_ClampY
};

namespace ImGui
{
  // The only requirement for a ControlPoint object is the ability to modify an x and a y value as well as equivalence checking
  template <typename T>
  bool ControlPoints(std::vector<T>& control_points, float radius, ImU32 base_col, ImU32 selected_col, int control_point_flags = ImGuiControlPointFlags_None)
  {
    static T* selectedControlPoint = nullptr;
    static T selectedControlPointState;

    // draw control nodes
    bool noneSelected = true;
    bool moved = false;
    // run through every control node
    for (const auto& [i, controlPoint] : enumerate(control_points))
    {
      // calculate the world coordinates of each node
      const auto x = controlPoint.x;
      const auto y = controlPoint.y;

      // create vec2 for final location of node
      const auto controlPointLocation = ImVec2{ x, y };

      // set node color to lighter color if it's selected
      if (ImGui::PointCircle(ImGui::GetWorldPos(ImGui::GetMousePos()), controlPointLocation, radius) && !ImGui::IsMouseDragging())
      {
        noneSelected = false;
        if (!selectedControlPoint)
        {
          selectedControlPoint = &controlPoint;
          selectedControlPointState = controlPoint;
          RenderCircleFilled(controlPointLocation, radius, selected_col);
        }
        else if (selectedControlPoint == &controlPoint)
        {
          RenderCircleFilled(controlPointLocation, radius, selected_col);
        }
        else
        {
          RenderCircleFilled(controlPointLocation, radius, base_col);
        }
      }
      else
      {
        if (selectedControlPoint == &controlPoint)
        {
          RenderCircleFilled(controlPointLocation, radius, selected_col);
        }
        else
        {
          RenderCircleFilled(controlPointLocation, radius, base_col);
        }
      }

      if (selectedControlPoint == &controlPoint)
      {
        // if we have a selected node and are currently clicking and dragging
        if (ImGui::IsMouseDragging())
        {
          // move the node by the drag y amount
          const auto dragDelta = ImGui::GetWorldPos(ImGui::GetMouseDragDelta());

          // clamps value to fit node in screen
          if (control_point_flags & ImGuiControlPointFlags_ClampXY)
          {
            const auto [min, max] = GetViewRect();
            const auto clampMax = (max.y - min.y) / 2.f;
            selectedControlPoint->y = std::clamp(selectedControlPointState.y + dragDelta.y, -clampMax, clampMax);
          }
          else
          {
            if (control_point_flags & ImGuiControlPointFlags_FixY)
            {
              if (control_point_flags & ImGuiControlPointFlags_FixX)
              {
                // wait what why don't u wanna move the control points
              }
              else
              {
                selectedControlPoint->x = selectedControlPointState.x + dragDelta.x;
              }
            }
            else if (control_point_flags & ImGuiControlPointFlags_FixX)
            {
              selectedControlPoint->y = selectedControlPointState.y + dragDelta.y;
            }
            else
            {
              const auto mousePos = ImGui::GetWorldPos(ImGui::GetMousePos());
              selectedControlPoint->x = mousePos.x;
              selectedControlPoint->y = mousePos.y;
            }
          }

          moved = true;
          noneSelected = false;
        }
        else
        {
          selectedControlPointState = *selectedControlPoint;
        }
      }
    }

    // if node is no longer selected (not dragging or hovering) then unset selected control point
    if (noneSelected == true)
    {
      selectedControlPoint = nullptr;
    }

    return selectedControlPoint == nullptr;
  }
}
