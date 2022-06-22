#pragma once
#include "application.hpp"
#include <glm/glm.hpp>

//Making single tile data super small so arrays of it are also small
using TileData = byte;

//Raw data of cells. 
struct CellBackgroundRawData
{
  const int cellSize = 300; //width and height are both 300
  //X and Y pos of cell in world
  int xPos = 0;
  int yPos = 0;

  TileData data[300][300]{ 0 };  //90 kb
};

class SparseCellData
{
  //Figure out way to generate sparse
  //std::unordered_map<glm::ivec2, 
};

//Place for those constant zomboid constants such as cell size and pixel data info
namespace ZomboidConstants
{
  const int cellSize = 300;

  const int smallRoadLaneWidth = 2;
  const int regularRoadLaneWidth = 3;
  const int largeRoadLaneWidth = 4;

  //Occupies bits 5-8
  enum TileVegData : byte
  {
    Tile_VEG_None   = 0b00000000,
    OnlyTrees       = 0b00010000,
    DenseTrees      = 0b00100000,
    MedTrees        = 0b00110000,
    FirTrees        = 0b01000000,
    GrassAllTypes   = 0b01010000,
    LightLongGrass  = 0b01100000,
    GrassFewTrees   = 0b01110000,
    BushesGrassTree = 0b10000000, 
    VegBitwiseMask  = 0b11110000
  };

  //Occupies bits 1-4
  enum TileBackgroundData : byte
  {
    Tile_BG_None    = 0b00000000,
    DarkGrass       = 0b00000001,
    MediumGrass     = 0b00000010,
    LightGrass      = 0b00000011,
    Sand            = 0b00000100,
    LightAsphalt    = 0b00000101,
    DarkAsphalt     = 0b00000110,
    MediumAsphalt   = 0b00000111,
    GravelDirt      = 0b00001000,
    Dirt            = 0b00001001, 
    DirtGrass       = 0b00001010, 
    Water           = 0b00001011,
    BGBitwiseMask   = 0b00001111
  };

  const static std::unordered_map<TileBackgroundData, const glm::ivec3> TileColors
  {
    { Tile_BG_None,     glm::ivec3(0, 0, 0)},
    { DarkGrass,        glm::ivec3(90, 100, 35)},
    { MediumGrass,      glm::ivec3(117, 117, 47)},
    { LightGrass,       glm::ivec3(145, 135, 60)},
    { Sand,             glm::ivec3(210, 200, 160)},
    { LightAsphalt,     glm::ivec3(165, 160, 140)},
    { DarkAsphalt,      glm::ivec3(100,100, 100)},    //Main Roads
    { MediumAsphalt,    glm::ivec3(120,120, 120)},
    { GravelDirt,       glm::ivec3(140, 70, 15)},
    { Dirt,             glm::ivec3(120,70, 20)},
    { DirtGrass,        glm::ivec3(80, 55, 20)},
    { Water,            glm::ivec3(0, 138, 255)},  //Cannot walk on
  };

  //Static maps to easily get colors from data
  const static std::unordered_map<TileVegData, const glm::ivec3> VegColors
  {
    { Tile_VEG_None,    glm::ivec3(0, 0, 0)},
    { OnlyTrees,        glm::ivec3( 255, 0, 0 )},  //Basically never use
    { DenseTrees,       glm::ivec3( 200, 0, 0 )},
    { MedTrees,         glm::ivec3( 127, 0, 0 )},
    { FirTrees,         glm::ivec3( 64, 0, 0 )},
    { GrassAllTypes,    glm::ivec3( 0, 255, 0 )},
    { LightLongGrass,   glm::ivec3( 0, 255, 0 )},
    { GrassFewTrees,    glm::ivec3( 0, 128, 0 )},
    { BushesGrassTree,  glm::ivec3( 255, 0, 255 )},
  };
}


struct BackgroundRawDataView
{
  
  ID3D11Texture2D* debugTex;
  ID3D11ShaderResourceView* debugTexView;
  bool isDirty = true;

  CellBackgroundRawData& getData()
  {
    isDirty = true;
    return data;
  }
  const CellBackgroundRawData& getData() const
  {
    return data;
  }
private:
  CellBackgroundRawData data;
};

class MapGenerator
{
public:
  MapGenerator();
  void SetSize(Application& app, int numXCells, int numYCells);

  //IMGUI UI
  void DrawUI(Application& app);
  void DrawMenu(Application& app);
  void Perlin(float perlinNoiseScalar = 2.0f);

  void ExportToPng();

  std::vector<std::vector<BackgroundRawDataView>> mapRawData;

  std::string MapName = "TestMap";
  
private:
  //UI
  int currentDebugChoice = 0;
  glm::ivec2 currentMapChoice = { 0,0 };

  //Info
  int width;  //In cells
  int height;
};


class ProjectZombProcGen : public Project
{
public:
  ProjectZombProcGen(Application& _app);

  void draw() final override;
  void draw_editors() final override;
  void draw_menus() final override;

  void reset() final override;

  std::string name() final override;
  Application& app;

private:
  bool showMousePosition = true;
  ImVec2 mouseScreenPos;
  ImVec2 mouseWorldPos;

  MapGenerator mapGen;

  //ID3D11Texture2D* testBgTex;
  //ID3D11ShaderResourceView* testBgTexView;
  //ID3D11Texture2D* testVegTex;
  //ID3D11ShaderResourceView* testVegTexView;

  CellBackgroundRawData exRawData;

  //std::unordered_map<Cell&, > CellImages;

  // A set of solid colors for drawing onto a dark gray background
  const ImVec4 colorSoftLightGray = { 0.9f, 0.9f, 0.9f, 0.415f };
  const ImVec4 colorSoftDarkGray = { 0.26f, 0.26f, 0.26f, 0.415f };
  const ImVec4 colorSoftLightGreen = { 0.74f, 0.98f, 0.74f, 0.415f };
  const ImVec4 colorSoftBlue = { 0.56f, 0.56f, 0.95f, 1.f };
  const ImVec4 colorSoftWhiteBlue = { 0.74f, 0.74f, 0.98f, 1.f };
};
