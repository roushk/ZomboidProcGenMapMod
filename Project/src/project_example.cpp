#include <pch.hpp>
#include "project_example.hpp"

#include "imgui_distance.hpp"

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
    // Draw line on top of drawable screen
    drawList->AddLine({ windowLeftDrawBound, windowTopDrawBound }, { windowRightDrawBound, windowTopDrawBound }, boxColorPacked);

    // Draw line on bottom of drawable screen
    drawList->AddLine({ windowLeftDrawBound, windowBottomDrawBound }, { windowRightDrawBound, windowBottomDrawBound }, boxColorPacked);

    // Draw line on left of drawable screen
    drawList->AddLine({ windowLeftDrawBound, windowTopDrawBound }, { windowLeftDrawBound, windowBottomDrawBound }, boxColorPacked);

    // Draw line on right of drawable screen
    drawList->AddLine({ windowRightDrawBound, windowTopDrawBound }, { windowRightDrawBound, windowBottomDrawBound }, boxColorPacked);
  }

  if (drawCircle)
  {
    // Draw a sphere in the center
    if (const auto mousePos = ImGui::GetMousePos(); imgui_point_circle(mousePos, { windowHorizontalCenter, windowVerticalCenter }, circleRadius))
      drawList->AddCircleFilled({ windowHorizontalCenter, windowVerticalCenter }, circleRadius, circleColorHighlightedPacked, 128);
    else
      drawList->AddCircleFilled({ windowHorizontalCenter, windowVerticalCenter }, circleRadius, circleColorPacked, 128);
  }
}

void ProjectExample::draw_editors()
{
  static ImVec2 windowSize; // Default initializes to { 0, 0 }

  // Only show editor window if any editor buttons are active
  if (toggleDrawBox || toggleDrawCircle || showMousePosition)
  {
    // Create Editor window (Just an example editor window that scales with what is currently inside of it,
    // can definitely change to be whatever you want, this is just a hacky quick version that works).
    ImGui::SetNextWindowPos({ 5.f, Application::WindowHeight() - windowSize.y - 5.f }); //TODO: Change hard coded window position to scale with window size changes
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
      }

      if (toggleDrawCircle)
      {
        ImGui::Checkbox("Toggle Circle", &drawCircle);
        if (drawCircle)
          ImGui::SliderFloat("Circle Radius", &circleRadius, 1.f, 250.f);
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
