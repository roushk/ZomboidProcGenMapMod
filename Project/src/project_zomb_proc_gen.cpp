#include <pch.hpp>
#include "project_zomb_proc_gen.hpp"

#include "enumerate.hpp"
#include "imgui_control_point.hpp"

#include <glm/gtc/noise.hpp>

#include <filesystem>
#include <iostream>

ProjectZombProcGen::ProjectZombProcGen(Application& _app) : app{ _app }
{
  reset();
  ImGui::SetViewRect({ 0.f, 0.f }, { 10.f, 10.f });
  std::cout << std::filesystem::current_path();
  app.createArrayTexture(&exRawData, &testBgTexView, &testBgTex);
  app.createArrayTexture(&exRawData, &testVegTexView, &testVegTex);

  for (int i = 0; i < 300; ++i)
  {
    for (int j = 0; j < 300; ++j)
    {
      float perlinScale = 2.0f;

      // exRawData.data[i][j] = ZomboidConstants::Sand;
      float perlin = 0.5f + (0.5f * glm::perlin(perlinScale * glm::vec2( exRawData.xPos + i / 300.0f, exRawData.yPos + j / 300.0f)));

      if (perlin > 0 && perlin < 0.15f)
      {
        exRawData.data[i][j] = ZomboidConstants::Water | (exRawData.data[i][j] & ZomboidConstants::VegBitwiseMask);
      }
      else if (perlin >= 0.15f && perlin < 0.2f)
      {
        exRawData.data[i][j] = ZomboidConstants::Sand | (exRawData.data[i][j] & ZomboidConstants::VegBitwiseMask);
      }
      else if (perlin >= 0.2f && perlin < 0.5f)
      {
        exRawData.data[i][j] = ZomboidConstants::LightGrass | (exRawData.data[i][j] & ZomboidConstants::VegBitwiseMask);
      }
      else if (perlin >= 0.5f && perlin < 0.8f)
      {
        exRawData.data[i][j] = ZomboidConstants::MediumGrass | (exRawData.data[i][j] & ZomboidConstants::VegBitwiseMask);
      }
      else if (perlin >= 0.8f && perlin < 1.f)
      {
        exRawData.data[i][j] = ZomboidConstants::DarkGrass | (exRawData.data[i][j] & ZomboidConstants::VegBitwiseMask);
      }
    }
  }

}

void ProjectZombProcGen::draw()
{
  // Basic colors that work well with the background
  const ImU32 lineColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftLightGray);
  const ImU32 circleColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftBlue);
  const ImU32 circleColorHighlightedPacked = ImGui::ColorConvertFloat4ToU32(colorSoftWhiteBlue);

  mouseScreenPos = ImGui::GetMousePos();
  mouseWorldPos = ImGui::GetWorldPos(mouseScreenPos);

  static float currDt = (1.0f / 60.0f);

  for (int i = 1; i < 10; ++i)
  {
    for (int j = 1; j < 10; ++j)
    {
      byte leftSide = exRawData.data[i][j] & 0b11110000;
      byte rightSide = exRawData.data[i][j] & 0b00001111;

      exRawData.data[i][j] = rightSide | 0b00100000;
    }
  }

  app.updateArrayTexture(&exRawData, testBgTex, Application::UpdateArraySetting::Background);
  ImGui::Image((void*)testBgTexView, ImVec2(300, 300));

  app.updateArrayTexture(&exRawData, testVegTex, Application::UpdateArraySetting::Veg);
  ImGui::Image((void*)testVegTexView, ImVec2(300, 300));
}

void ProjectZombProcGen::draw_editors()
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

void ProjectZombProcGen::draw_menus()
{
  // Create drop-down menu button
  if (ImGui::BeginMenu("Zomboid Proc Gen Options"))
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

void ProjectZombProcGen::reset()
{
  showMousePosition = true;
}

std::string ProjectZombProcGen::name()
{
  return "Zomboid Proc Gen";
}
