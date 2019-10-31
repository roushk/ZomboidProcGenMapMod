#include <pch.hpp>
#include "project_example.hpp"

#include "imgui_distance.hpp"
#include "imgui_disabled.hpp"

ProjectExample::ProjectExample()
{
  reset();
}

void ProjectExample::draw()
{
  auto drawList = ImGui::GetWindowDrawList();     // Get draw list for current window
  const auto windowSize = ImGui::GetWindowSize(); // Get current window dimensions
  const auto windowPos = ImGui::GetWindowPos();   // Get current window position

  // Basic colors that work well with the background
  const ImU32 boxColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftLightGray);
  const ImU32 circleColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftBlue);
  const ImU32 circleColorHighlightedPacked = ImGui::ColorConvertFloat4ToU32(colorSoftWhiteBlue);

  const auto windowHorizontalCenter = windowSize.x * 0.5f + windowPos.x; // 50%, middle of screen
  const auto windowLeftDrawBound = windowSize.x * 0.1f + windowPos.x; // 10% on left side of screen
  const auto windowRightDrawBound = windowSize.x * 0.9f + windowPos.x; // 90% on right side of screen

  const auto windowVerticalCenter = windowSize.y * 0.5f + windowPos.y; // 50%, middle of screen
  const auto windowTopDrawBound = windowSize.y * 0.1f + windowPos.y; // 10% on left side of screen
  const auto windowBottomDrawBound = windowSize.y * 0.9f + windowPos.y; // 90% on right side of screen

  if (drawBox)
  {
    // Draw rect
    drawList->AddRect({ windowLeftDrawBound, windowTopDrawBound }, { windowRightDrawBound, windowBottomDrawBound }, boxColorPacked, boxRounding, ImDrawCornerFlags_::ImDrawCornerFlags_All, boxThickness);
  }

  if (drawCircle)
  {
    // Draw a sphere in the center
    if (const auto mousePos = ImGui::GetMousePos(); imgui_point_circle(mousePos, { windowHorizontalCenter, windowVerticalCenter }, circleRadius))
      drawList->AddCircleFilled({ windowHorizontalCenter, windowVerticalCenter }, circleRadius, circleColorHighlightedPacked, circleDivisions);
    else 
      drawList->AddCircleFilled({ windowHorizontalCenter, windowVerticalCenter }, circleRadius, circleColorPacked, circleDivisions);
  }
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
