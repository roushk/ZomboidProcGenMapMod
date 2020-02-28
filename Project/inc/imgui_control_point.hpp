#pragma once
#include "enumerate.hpp"
#include "imgui_distance.hpp"

enum ImGuiControlPointFlags_
{
  ImGuiControlPointFlags_None = 0,
  ImGuiControlPointFlags_FixX = 1 << 1,
  ImGuiControlPointFlags_FixY = 1 << 2,

  ImGuiControlPointFlags_ClampX = 1 << 3,
  ImGuiControlPointFlags_ClampY = 1 << 4,

  ImGuiControlPointFlags_AddAndRemoveWithMouse = 1 << 5,

  ImGuiControlPointFlags_FixXY = ImGuiControlPointFlags_FixX | ImGuiControlPointFlags_FixY,
  ImGuiControlPointFlags_ClampXY = ImGuiControlPointFlags_ClampX | ImGuiControlPointFlags_ClampY
};

namespace ImGui
{
  // There are several requirements for the T value that the vector of control points holds:
  //  * It must have publicly accessible 'x' and 'y' variables
  //  * It must be default contructable
  //  * It must be able to be pushed back to a std::vector
  template <typename T>
  bool ControlPoints(std::vector<T>& control_points, float radius, ImU32 base_col, ImU32 selected_col, int control_point_flags = ImGuiControlPointFlags_None)
  {
    static T* selectedControlPoint = nullptr;
    static T selectedControlPointState;

    // draw control nodes
    bool noneSelected = true;
    bool moved = false;
    bool markedForDeletion = false;
    // run through every control node
    for (const auto&[i, controlPoint] : enumerate(control_points))
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

          if (control_point_flags & ImGuiControlPointFlags_FixY)
          {
            if (control_point_flags & ImGuiControlPointFlags_FixX)
            {
              // wait what why don't u wanna move the control points
            }
            else
            {
              const auto mousePos = ImGui::GetWorldPos(ImGui::GetMousePos());
              selectedControlPoint->x = mousePos.x;
              if (const auto viewRect = ImGui::GetViewRect(); control_point_flags & ImGuiControlPointFlags_ClampX)
                selectedControlPoint->x = (viewRect.first.x < viewRect.second.x ? std::clamp(selectedControlPoint->x, viewRect.first.x, viewRect.second.x) : std::clamp(selectedControlPoint->x, viewRect.second.x, viewRect.first.x));
            }
          }
          else if (control_point_flags & ImGuiControlPointFlags_FixX)
          {
            const auto mousePos = ImGui::GetWorldPos(ImGui::GetMousePos());
            selectedControlPoint->y = mousePos.y;
            if (const auto viewRect = ImGui::GetViewRect(); control_point_flags & ImGuiControlPointFlags_ClampY)
              selectedControlPoint->y = (viewRect.first.y < viewRect.second.y ? std::clamp(selectedControlPoint->y, viewRect.first.y, viewRect.second.y) : std::clamp(selectedControlPoint->y, viewRect.second.y, viewRect.first.y));
          }
          else
          {
            const auto mousePos = ImGui::GetWorldPos(ImGui::GetMousePos());
            selectedControlPoint->x = mousePos.x;
            if (const auto viewRect = ImGui::GetViewRect(); control_point_flags & ImGuiControlPointFlags_ClampX)
              selectedControlPoint->x = (viewRect.first.x < viewRect.second.x ? std::clamp(selectedControlPoint->x, viewRect.first.x, viewRect.second.x) : std::clamp(selectedControlPoint->x, viewRect.second.x, viewRect.first.x));

            selectedControlPoint->y = mousePos.y;
            if (const auto viewRect = ImGui::GetViewRect(); control_point_flags & ImGuiControlPointFlags_ClampY)
              selectedControlPoint->y = (viewRect.first.y < viewRect.second.y ? std::clamp(selectedControlPoint->y, viewRect.first.y, viewRect.second.y) : std::clamp(selectedControlPoint->y, viewRect.second.y, viewRect.first.y));
          }

          moved = true;
          noneSelected = false;
        }
        else if (control_point_flags & ImGuiControlPointFlags_AddAndRemoveWithMouse && ImGui::IsMouseClicked(1)) // if we right click a node
        {
          markedForDeletion = true;
          moved = true;
          noneSelected = true;
        }
        else
        {
          selectedControlPointState = *selectedControlPoint;
        }
      }
    }

    if (markedForDeletion)
    {
      // somehow this should work
      control_points.erase(control_points.begin() + (selectedControlPoint - &*control_points.begin()));
    }

    // if node is no longer selected (not dragging or hovering) then unset selected control point
    if (noneSelected == true)
    {
      selectedControlPoint = nullptr;

      // if we left click not on a node
      if (control_point_flags & ImGuiControlPointFlags_AddAndRemoveWithMouse && ImGui::IsMouseClicked(0) && !ImGui::GetIO().WantCaptureMouse)
      {
        const auto mousePos = ImGui::GetWorldPos(ImGui::GetMousePos());

        T temp; temp.x = mousePos.x; temp.y = mousePos.y;
        control_points.push_back(temp);
        selectedControlPoint = &control_points.back();
        selectedControlPointState = *selectedControlPoint;
        moved = true;
      }
    }

    return moved;
  }
}
