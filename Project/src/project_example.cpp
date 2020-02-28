#include <pch.hpp>
#include "project_example.hpp"

#include "enumerate.hpp"
#include "imgui_control_point.hpp"

// Pulled from https://www.lighthouse3d.com/tutorials/maths/catmull-rom-spline/
float catmullRomSpline(float x, float v0, float v1,
                        float v2, float v3) {

  float c1, c2, c3, c4;

  c1 = 1.0f * v1;
  c2 = -0.5f * v0 + 0.5f * v2;
  c3 = 1.0f * v0 + -2.5f * v1 + 2.0f * v2 + -0.5f * v3;
  c4 = -0.5f * v0 + 1.5f * v1 + -1.5f * v2 + 0.5f * v3;

  return(((c4*x + c3)*x + c2)*x + c1);
}

// returns a value on the spline between v1 and v2, at value t in range [0, 1]
ImVec2 catmullRomSpline(float t, ImVec2 v0, ImVec2 v1, ImVec2 v2, ImVec2 v3)
{
  return { catmullRomSpline(t, v0.x, v1.x, v2.x, v3.x), catmullRomSpline(t, v0.y, v1.y, v2.y, v3.y) };
}

ProjectExample::ProjectExample()
{
  reset();
  ImGui::SetViewRect({ 0.f, 0.f }, { 10.f, 10.f });
}

void ProjectExample::draw()
{
  // Basic colors that work well with the background
  const ImU32 lineColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftLightGray);
  const ImU32 circleColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftBlue);
  const ImU32 circleColorHighlightedPacked = ImGui::ColorConvertFloat4ToU32(colorSoftWhiteBlue);

  // catmull rom spline for showing line drawing
  for(auto i = 1u; i < polynomial.size(); ++i)
  {
    ImGui::RenderLine(polynomial[i - 1], polynomial[i], lineColorPacked);
  }

  // ControlPoints(...) returns true when a control point is actively being clicked and dragged around, and false otherwise
  if(ImGui::ControlPoints(controlPoints, circleRadius, circleColorPacked, circleColorHighlightedPacked, ImGuiControlPointFlags_AddAndRemoveWithMouse | ImGuiControlPointFlags_ClampXY))
  {
    polynomial.resize((controlPoints.size() - 1) * quality);

    for(auto n = 0; n < controlPoints.size() - 1; ++n)
    {
      for(auto i = n * quality; i < (n + 1) * quality; ++i)
      {
        const float t = static_cast<float>(i - n * quality) / (quality - 1);
        polynomial[i] = catmullRomSpline(t, controlPoints[std::max(n - 1, 0)], controlPoints[n], controlPoints[n + 1], controlPoints[std::min(n + 2, static_cast<int>(controlPoints.size()) - 1)]);
      }
    }
  }

  mouseScreenPos = ImGui::GetMousePos();
  mouseWorldPos = ImGui::GetWorldPos(mouseScreenPos);
}

void ProjectExample::draw_editors()
{
  static ImVec2 windowSize; // Default initializes to { 0, 0 }

  // Only show editor window if any editor buttons are active
  if (showMousePosition)
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
      if (showMousePosition)
      {
        ImGui::Text(fmt::format("Mouse Screen Space Coordinates: {0}, {1}", mouseScreenPos.x, mouseScreenPos.y).c_str());
        ImGui::Text(fmt::format("Mouse World Space Coordinates:  {0}, {1}", mouseWorldPos.x, mouseWorldPos.y).c_str());
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
  circleRadius = 15.f;
  showMousePosition = true;

  controlPoints.clear();
  polynomial.clear();
}

std::string ProjectExample::name()
{
  return "Project Example";
}
