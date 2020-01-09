#include <pch.hpp>
#include "project_example.hpp"

#include "imgui_distance.hpp"
#include "imgui_disabled.hpp"
#include "imgui_control_point.hpp"

ProjectExample::ProjectExample()
{
  reset();
  ImGui::SetViewRect({ 0.f, 0.f }, { 10.f, 10.f });
}

void ProjectExample::draw()
{
  // Basic colors that work well with the background
  const ImU32 boxColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftLightGray);
  const ImU32 circleColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftBlue);
  const ImU32 circleColorHighlightedPacked = ImGui::ColorConvertFloat4ToU32(colorSoftWhiteBlue);

  if (drawBox)
  {
    ImGui::RenderRect({ 1.f, 1.f }, { 9.f, 9.f }, boxColorPacked, boxRounding, ImDrawCornerFlags_All, boxThickness);
  }

  if (drawCircle)
  {
    // Draw a sphere in the center
    const auto sphereCenter = ImVec2{ 5.f, 5.f };
    if (ImGui::PointCircle(ImGui::GetWorldPos(ImGui::GetMousePos()), sphereCenter, circleRadius))
      ImGui::RenderCircleFilled(sphereCenter, circleRadius, circleColorHighlightedPacked, circleDivisions);
    else 
      ImGui::RenderCircleFilled(sphereCenter, circleRadius, circleColorPacked, circleDivisions);
  }

  ImGui::ControlPoints(controlPoints, circleRadius, circleColorPacked, circleColorHighlightedPacked, ImGuiControlPointFlags_FixX);
}

void ProjectExample::draw_editors()
{
  static ImVec2 windowSize; // Default initializes to { 0, 0 }

  // Only show editor window if any editor buttons are active
  if (toggleDrawBox || toggleDrawCircle || showMousePosition)
  {
    // Create Editor window
    ImGui::SetNextWindowPos({ 5.f, Application::WindowHeight() - windowSize.y - 5.f });
    ImGui::SetNextWindowBgAlpha(0.2f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    if (ImGui::Begin("##Example_Editor", nullptr,
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysAutoResize))
    {
      if (toggleDrawBox)
      {
        ImGui::Checkbox("Toggle Box", &drawBox);
        if (IMGUI_DISABLED(drawBox))
        {
          ImGui::SliderFloat("Box Rounding", &boxRounding, 0.f, 25.f);
          ImGui::SliderFloat("Box Thickness", &boxThickness, 0.f, 25.f);
        }
      }

      if (toggleDrawCircle)
      {
        ImGui::Checkbox("Toggle Circle", &drawCircle);
        if (IMGUI_DISABLED(drawCircle))
        {
          ImGui::SliderFloat("Circle Radius", &circleRadius, 1.f, 250.f);
          ImGui::SliderInt("Circle Divisions", &circleDivisions, 8, 256);
        }
      }

      if (showMousePosition)
      {
        const auto mousePos = ImGui::GetMousePos();
        ImGui::Text(fmt::format("Mouse Coordinates: {0}, {1}", mousePos.x, mousePos.y).c_str());
      }

      // Get window size for next drawing
      windowSize = ImGui::GetWindowSize(); // A little hacky, but it works
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
  }
}

void ProjectExample::draw_menus()
{
  // Create drop-down menu button
  if (ImGui::BeginMenu("Project Example Options"))
  {
    if (ImGui::MenuItem("Box Toggler", nullptr, toggleDrawBox))
    {
      toggleDrawBox ^= 1;
    }

    if (ImGui::MenuItem("Circle Toggler", nullptr, toggleDrawCircle))
    {
      toggleDrawCircle ^= 1;
    }

    if (ImGui::MenuItem("Show Mouse Position", nullptr, showMousePosition))
    {
      showMousePosition ^= 1;
    }

    // Make sure to end the menu
    ImGui::EndMenu();
  }
  
  // Add more ImGui::BeginMenu(...) for additional menus
}

void ProjectExample::reset()
{
  drawBox = true;
  toggleDrawBox = true;
  drawCircle = true;
  toggleDrawCircle = true;
  circleRadius = 15.f;
  showMousePosition = true;
}

std::string ProjectExample::name()
{
  return "Project Example";
}
