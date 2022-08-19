#include <pch.hpp>
#include "project_zomb_proc_gen.hpp"

#include "enumerate.hpp"
#include "imgui_control_point.hpp"

#include <glm/gtc/noise.hpp>

#include <filesystem>
#include <iostream>

#define STBCC_GRID_COUNT_X_LOG2    10
#define STBCC_GRID_COUNT_Y_LOG2    10
#define STB_CONNECTED_COMPONENTS_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_connected_components.h>
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
#include <stb/stb_image_write.h>

ProjectZombProcGen::ProjectZombProcGen(Application& _app) : app{ _app }
{
  std::time_t time = std::time(NULL);
  std::srand(time);

  std::cout << "Current Time for rand seed " << time << std::endl;

  reset();
  ImGui::SetViewRect({ 0.f, 0.f }, { 10.f, 10.f });
  std::cout << std::filesystem::current_path() << std::endl;
  //app.createArrayTexture(&exRawData, &testBgTexView, &testBgTex);
  //app.createArrayTexture(&exRawData, &testVegTexView, &testVegTex);

  mapGen.SetSize(app, 5, 5);
  mapGen.Perlin();
}

void ProjectZombProcGen::draw()
{
  // Basic colors that work well with the background
  const ImU32 lineColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftLightGray);
  const ImU32 circleColorPacked = ImGui::ColorConvertFloat4ToU32(colorSoftBlue);
  const ImU32 circleColorHighlightedPacked = ImGui::ColorConvertFloat4ToU32(colorSoftWhiteBlue);

  mouseScreenPos = ImGui::GetMousePos();
  mouseWorldPos = ImGui::GetWorldPos(mouseScreenPos);

  mapGen.DrawUI(app);

  //app.updateArrayTexture(&exRawData, testBgTex, Application::UpdateArraySetting::Background);
  //ImGui::Image((void*)testBgTexView, ImVec2(300, 300));
  //app.updateArrayTexture(&exRawData, testVegTex, Application::UpdateArraySetting::Veg);
  //ImGui::Image((void*)testVegTexView, ImVec2(300, 300));
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
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoMouseInputs))
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

  mapGen.DrawMenu(app);
  
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

MapGenerator::MapGenerator()
{

}

void MapGenerator::SetSize(Application& app, int numXCells, int numYCells)
{
  width = numXCells;
  height = numYCells;

  mapRawData = std::vector<std::vector<BackgroundRawDataView>>();
  mapRawData.resize(width);
  for (int i = 0; i < width; ++i)
  {
    mapRawData[i] = std::vector<BackgroundRawDataView>();
    mapRawData[i].resize(height);
  }

  for (int x = 0; x < width; ++x)
  {
    for (int y = 0; y < height; ++y)
    {
      app.createArrayTexture(&mapRawData[x][y].getData(), &mapRawData[x][y].debugTexView, &mapRawData[x][y].debugTex);
    }
  }
}

//IMGUI UI
void MapGenerator::DrawMenu(Application& app)
{

  if (ImGui::BeginMenu("Map View"))
  {
    if (ImGui::MenuItem("Background Color", 0, currentDebugChoice == 0))
    {
      currentDebugChoice = 0;
    }
    if (ImGui::MenuItem("Vegetation Color", 0, currentDebugChoice == 1))
    {
      currentDebugChoice = 1;
    }
    if (ImGui::MenuItem("Debug Color", 0, currentDebugChoice == 2))
    {
      currentDebugChoice = 2;
    }
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Map Settings"))
  {
    if (ImGui::MenuItem("Regenerate Background and Veg via Perlin Noise"))
    {
      Perlin();
    }

    //TODO: number of points, min/max cell, select distance
    if (ImGui::MenuItem("Generate Voronoi Diagrams"))
    {
      voronoiPoints.clear();
      for (int i = 0; i < numCities; ++i)
      {
        int randX = (std::rand() % 60) * 5;
        int randY = (std::rand() % 60) * 5;
        voronoiPoints.push_back({ randX, randY });
      }

      VoronoiDiagram(0, 0, voronoiPoints, VoronoiDiagram::EuclidianDist);
      VoronoiDiagram(0, 1, voronoiPoints, VoronoiDiagram::ChebyshevDist);
      VoronoiDiagram(0, 2, voronoiPoints, VoronoiDiagram::ManhattanDist);
    }
    ImGui::DragInt("Num Cities", &numCities, 0.1f, 1, ZConsts::Color_MAX - 1);

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Export Settings"))
  {
    if (ImGui::MenuItem("Export BG and Veg to Images"))
    {
      ExportToPng();
    }

    ImGui::EndMenu();
  }
}

void MapGenerator::Perlin(float perlinNoiseScalar)
{
  float perlinScale = perlinNoiseScalar / std::max(width, height);
  perlinScale *= std::max(width, height);
  int randVal = std::rand() % 100000;
  glm::vec2 perlinRand = { randVal, randVal };

  for (int x = 0; x < width; ++x)
  {
    for (int y = 0; y < height; ++y)
    {
      std::cout << "Generating Perlin Noise for " << x << " x " << y << std::endl;
      mapRawData[x][y].getData().xPos = x;
      mapRawData[x][y].getData().yPos = y;

      for (int i = 0; i < 300; ++i)
      {
        for (int j = 0; j < 300; ++j)
        {
          //RESET CELL ENTIRELY TO ZERO
          mapRawData[x][y].getData().data[i][j] = 0;

          // exRawData.data[i][j] = ZomboidConstants::Sand;
          float perlin = 0.5f + (0.5f * glm::perlin(perlinRand + perlinScale * glm::vec2(mapRawData[x][y].getData().xPos + i / 300.0f, mapRawData[x][y].getData().yPos + j / 300.0f)));

          if (perlin > 0 && perlin < 0.15f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::Water);
          }
          else if (perlin >= 0.15f && perlin < 0.18f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::Sand);
          }
          else if (perlin >= 0.18f && perlin < 0.5f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::LightGrass);
          }
          else if (perlin >= 0.5f && perlin < 0.8f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::MediumGrass);
          }
          else if (perlin >= 0.8f && perlin < 1.f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::DarkGrass);
          }

          //Separate scale but same perlin noise map for trees
          if (perlin >= 0.2f && perlin < 0.4f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::GrassAllTypes);
          }
          if (perlin >= 0.4f && perlin < 0.6f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::GrassFewTrees);
          }
          else if (perlin >= 0.6f && perlin < 0.7f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::MedTrees);
          }
          else if (perlin >= 0.7f && perlin < 1.f)
          {
            mapRawData[x][y].getData().SetTile(i, j, ZConsts::DenseTrees);
          }
        }
      }
    }
  }
}


//https://en.wikipedia.org/wiki/Voronoi_diagram
//Inefficient naive implementation
//There is one called Fortunes but not sure if its faster for grid based

inline void MapGenerator::VoronoiDiagram(CellCoord x, CellCoord y, const std::vector<glm::ivec2>& points, VoronoiDiagram::VoronoiDiagramDistFuncs func)
{
  BackgroundRawDataView& cell = mapRawData[x][y];
  std::function<double(glm::ivec2, glm::ivec2)> distFunc = VoronoiDiagram::VoronoiFuncs.at(func);

  for (int i = 0; i < ZConsts::cellSize; ++i)
  {
    for (int j = 0; j < ZConsts::cellSize; ++j)
    {
      float minDist = std::numeric_limits<float>::max();

      for (unsigned k = 0; k < points.size(); ++k)
      {
        //Get distance function
        float dist = distFunc(glm::ivec2(i, j), points[k]);

        if (dist < minDist)
        {
          minDist = dist;
          cell.getData().data[i][j] = k;
        }
      }
    }
  }
}

//https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm
void MapGenerator::DelTri_BowyerWatson(CellCoord x, CellCoord y, const std::vector<glm::ivec2>& points)
{

  //function BowyerWatson (pointList)
  //// pointList is a set of coordinates defining the points to be triangulated
  //triangulation := empty triangle mesh data structure
  //add super-triangle to triangulation // must be large enough to completely contain all the points in pointList
  //for each point in pointList do // add all the points one at a time to the triangulation
  //   badTriangles := empty set
  //   for each triangle in triangulation do // first find all the triangles that are no longer valid due to the insertion
  //      if point is inside circumcircle of triangle
  //         add triangle to badTriangles
  //   polygon := empty set
  //   for each triangle in badTriangles do // find the boundary of the polygonal hole
  //      for each edge in triangle do
  //         if edge is not shared by any other triangles in badTriangles
  //            add edge to polygon
  //   for each triangle in badTriangles do // remove them from the data structure
  //      remove triangle from triangulation
  //   for each edge in polygon do // re-triangulate the polygonal hole
  //      newTri := form a triangle from edge to point
  //      add newTri to triangulation
  //for each triangle in triangulation // done inserting points, now clean up
  //   if triangle contains a vertex from original super-triangle
  //      remove triangle from triangulation
  //return triangulation

  for (int i = 0; i < ZConsts::cellSize; ++i)
  {
    for (int j = 0; j < ZConsts::cellSize; ++j)
    {
      float minDist = std::numeric_limits<float>::max();

      for (unsigned k = 0; k < points.size(); ++k)
      {
        
      }
    }
  }
}

void MapGenerator::ExportToPng()
{
  uint8_t* pngData = new uint8_t[300 * 300 * 3];
  std::string directory = "..\\Assets\\test\\" + MapName;
  std::filesystem::create_directory(directory);

  //Export Background Colors
  for (int x = 0; x < width; ++x)
  {
    for (int y = 0; y < height; ++y)
    {
      //new image
      //X and Y are inverted on purpose
      std::string imageName = directory + "\\" + MapName + "_" + std::to_string(y) + "_" + std::to_string(x) + ".bmp";

      for (int i = 0; i < 300; ++i)
      {
        for (int j = 0; j < 300; ++j)
        {
          //and the important side to ignore non important values and get the color and put into data for png
          glm::ivec3 color = ZConsts::TileColors.at(static_cast<ZConsts::TileBackgroundData>(GetTile(x, y, i, j) & ZConsts::BGBitwiseMask));
          pngData[i * 300 * 3 + j * 3] = color.x;
          pngData[i * 300 * 3 + j * 3 + 1] = color.y;
          pngData[i * 300 * 3 + j * 3 + 2] = color.z;
        }
      }
      int result = stbi_write_bmp(imageName.c_str(), 300,300, 3, pngData);
    }
  }

  //Export Veg Colors
  for (int x = 0; x < width; ++x)
  {
    for (int y = 0; y < height; ++y)
    {
      //new image
      //X and Y are inverted on purpose
      std::string imageName = directory + "\\" + MapName + "_" + std::to_string(y) + "_" + std::to_string(x) + "_veg.bmp";

      for (int i = 0; i < 300; ++i)
      {
        for (int j = 0; j < 300; ++j)
        {
          glm::ivec3 color = ZConsts::VegColors.at(static_cast<ZConsts::TileVegData>(GetTile(x,y,i,j) & ZConsts::VegBitwiseMask));
          pngData[i * 300 * 3 + j * 3] = color.x;
          pngData[i * 300 * 3 + j * 3 + 1] = color.y;
          pngData[i * 300 * 3 + j * 3 + 2] = color.z;
        }
      }
      int result = stbi_write_bmp(imageName.c_str(), 300, 300, 3, pngData);
    }
  }
  delete pngData;
}

void MapGenerator::DrawUI(Application& app)
{
  
  ImGui::BeginChild("##VisibleMap", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
  
  //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.01f, 1.01f)); //1.0f = no spacing

  static int lastDebugChoice = 0;

  for (int i = 0; i < width; ++i)
  {
    for (int j = 0; j < height; ++j)
    {
      if (mapRawData[i][j].isDirty || lastDebugChoice != currentDebugChoice)
      {
        app.updateArrayTexture(&mapRawData[i][j].getData(), mapRawData[i][j].debugTex, Application::UpdateArraySetting(currentDebugChoice));
        mapRawData[i][j].isDirty = false;
      }

      ImGui::Image((void*)mapRawData[i][j].debugTexView, ImVec2(300, 300));
  
      if(j != height - 1)
        ImGui::SameLine();
    }
  }
  ImGui::PopStyleVar(1);

  //00, 01, 02
  for (int i = 0; i < voronoiPoints.size(); ++i)
  {
    ImGui::RenderCircleFilled(ImGui::GetWorldPos(glm::ivec2(voronoiPoints[i].y, voronoiPoints[i].x) + glm::ivec2(10, 25)), 10.0f, ImGui::ColorConvertFloat4ToU32(colorSoftLightGray));
  }

  for (int i = 0; i < voronoiPoints.size(); ++i)
  {
    ImGui::RenderCircleFilled(ImGui::GetWorldPos(glm::ivec2(voronoiPoints[i].y, voronoiPoints[i].x) + glm::ivec2(300, 0) + glm::ivec2(10,25)), 10.0f, ImGui::ColorConvertFloat4ToU32(colorSoftBlue));
  }

  for (int i = 0; i < voronoiPoints.size(); ++i)
  {
    ImGui::RenderCircleFilled(ImGui::GetWorldPos(glm::ivec2(voronoiPoints[i].y, voronoiPoints[i].x) + glm::ivec2(600, 0) + glm::ivec2(10, 25)), 10.0f, ImGui::ColorConvertFloat4ToU32(colorSoftWhiteBlue));
  }

  ImGui::EndChild();

  lastDebugChoice = currentDebugChoice;
}

DelTri::Circle DelTri::Tri::GetCircumcircle()
{
  glm::vec2 AC = c - a;

  glm::vec2 normalAC
}
