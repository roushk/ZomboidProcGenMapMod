#pragma once
#include "application.hpp"
#include <glm/glm.hpp>

//Making single tile data super small so arrays of it are also small
using TileData = byte;


/*
CellCoord cellX = worldX / ZomboidConstants::cellSize;
CellCoord cellY = worldY / ZomboidConstants::cellSize;
TileCoord tileX = worldX % ZomboidConstants::cellSize;
TileCoord tileY = worldY % ZomboidConstants::cellSize;

WorldCoord worldX = ZomboidConstants::cellSize * cellX + tileX
WorldCoord worldY = ZomboidConstants::cellSize * cellY + tileY
*/

//Sadly tiles are 300x300 and not 256x256 sooo we need to use 16 bits instead of 8
using TileCoord = uint16_t;
//No way engine can handle above 65536 * 65536 cells so capping at uint16
using CellCoord = uint16_t;
//World coords is cell num * 300 + tileCoord for global position so use 32 bits 
using WorldCoord = uint32_t;


namespace VoronoiDiagram
{
  enum VoronoiDiagramDistFuncs
  {
    EuclidianDist,  //
    ChebyshevDist,  //8 dir
    ManhattanDist,  //4 dir
  };
  
  //https://en.wikipedia.org/wiki/Euclidean_distance
  static double EuclidianDistFormula(glm::ivec2 start, glm::ivec2 end)
  {
    return sqrt((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y));
  }

  //https://en.wikipedia.org/wiki/Chebyshev_distance
  static double ChebyshevDistFormula(glm::ivec2 start, glm::ivec2 end)
  {
    return std::max(std::abs(end.x - start.x), std::abs(end.y - start.y));
  }

  //https://math.stackexchange.com/questions/139600/how-do-i-calculate-euclidean-and-manhattan-distance-by-hand
  static double ManhattanDistFormula(glm::ivec2 start, glm::ivec2 end)
  {
    return std::abs(end.x - start.x) + std::abs(end.y - start.y);
  }

  //take in X and Y and return double of dist
  const static std::unordered_map<VoronoiDiagramDistFuncs, std::function<double(glm::ivec2, glm::ivec2)>> VoronoiFuncs
  {
    {EuclidianDist, EuclidianDistFormula},
    {ChebyshevDist, ChebyshevDistFormula},
    {ManhattanDist, ManhattanDistFormula},
  };
}

//Place for those constant zomboid constants such as cell size and pixel data info
namespace ZConsts
{
  const int cellSize = 300;

  const int smallRoadLaneWidth = 2;
  const int regularRoadLaneWidth = 3;
  const int largeRoadLaneWidth = 4;

  //Occupies bits 5-8
  enum TileVegData : byte
  {
    Tile_VEG_None = 0b00000000,
    OnlyTrees = 0b00010000,
    DenseTrees = 0b00100000,
    MedTrees = 0b00110000,
    FirTrees = 0b01000000,
    GrassAllTypes = 0b01010000,
    LightLongGrass = 0b01100000,
    GrassFewTrees = 0b01110000,
    BushesGrassTree = 0b10000000,
    VegBitwiseMask = 0b11110000
  };

  //Occupies bits 1-4
  enum TileBackgroundData : byte
  {
    Tile_BG_None = 0b00000000,
    DarkGrass = 0b00000001,
    MediumGrass = 0b00000010,
    LightGrass = 0b00000011,
    Sand = 0b00000100,
    LightAsphalt = 0b00000101,
    DarkAsphalt = 0b00000110,
    MediumAsphalt = 0b00000111,
    GravelDirt = 0b00001000,
    Dirt = 0b00001001,
    DirtGrass = 0b00001010,
    Water = 0b00001011,
    BGBitwiseMask = 0b00001111
  };

  //Full byte debug colors
  enum TileDebugColors : byte
  {
    Color_Red,
    Color_Green,
    Color_Blue,
    Color_Orange,
    Color_Yellow,
    Color_Cyan,
    Color_Purple,
    Color_Magenta,
    Color_0,
    Color_1,
    Color_2,
    Color_3,
    Color_4,
    Color_5,
    Color_6,
    Color_7,
    Color_8,
    Color_9,
    Color_10,
    Color_11,
    Color_12,
    Color_13,
    Color_14,
    Color_15,
    Color_16,
    Color_17,
    Color_MAX
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

  const static std::unordered_map<TileVegData, const glm::ivec3> VegColors
  {
    { Tile_VEG_None,    glm::ivec3(0, 0, 0)},
    { OnlyTrees,        glm::ivec3(255, 0, 0)},  //Basically never use
    { DenseTrees,       glm::ivec3(200, 0, 0)},
    { MedTrees,         glm::ivec3(127, 0, 0)},
    { FirTrees,         glm::ivec3(64, 0, 0)},
    { GrassAllTypes,    glm::ivec3(0, 255, 0)},
    { LightLongGrass,   glm::ivec3(0, 255, 0)},
    { GrassFewTrees,    glm::ivec3(0, 128, 0)},
    { BushesGrassTree,  glm::ivec3(255, 0, 255)},
  };

  //Full byte debug colors
  const static std::unordered_map<byte, const glm::ivec3> DebugColorsMap
  {
    { Color_Red,        glm::ivec3(255, 0, 0)},
    { Color_Green,      glm::ivec3(0, 255, 0)},
    { Color_Blue,       glm::ivec3(0, 0, 255)},
    { Color_Orange,     glm::ivec3(255, 128, 0)},
    { Color_Yellow,     glm::ivec3(255, 255, 0)},
    { Color_Cyan,       glm::ivec3(0, 255, 255)},
    { Color_Purple,     glm::ivec3(255, 0, 255)},
    { Color_Magenta,    glm::ivec3(255, 0, 128)},
    { Color_0,          glm::ivec3(255, 128, 128)},
    { Color_1,          glm::ivec3(50, 100, 100)},
    { Color_2,          glm::ivec3(75, 200, 75)},
    { Color_3,          glm::ivec3(100, 75, 0)},
    { Color_4,          glm::ivec3(200, 150, 100)},
    { Color_5,          glm::ivec3(100, 200, 150)},
    { Color_6,          glm::ivec3(100, 150, 200)},
    { Color_7,          glm::ivec3(125, 100, 200)},
    { Color_8,          glm::ivec3(100, 50, 125)},
    { Color_9,          glm::ivec3(175, 175, 175)},
    { Color_10,         glm::ivec3(25, 50, 175)},
    { Color_11,         glm::ivec3(0, 125, 0)},
    { Color_12,         glm::ivec3(125, 25, 250)},
    { Color_13,         glm::ivec3(243, 165, 005)},
    { Color_14,         glm::ivec3(215, 045, 109)},
    { Color_15,         glm::ivec3(179, 036, 040)},
    { Color_16,         glm::ivec3(203, 208, 204)},
    { Color_17,         glm::ivec3(69, 050, 046)},
  };
}

//Raw data of cells. 
class CellBackgroundRawData
{
public:
  //X and Y pos of cell in world
  int xPos = 0;
  int yPos = 0;

  void SetTile(TileCoord x, TileCoord y, const ZConsts::TileVegData& vegData)
  {
    data[x][y] = vegData | (data[x][y] & ZConsts::BGBitwiseMask);
  }

  void SetTile(TileCoord x, TileCoord y, const ZConsts::TileBackgroundData& bgData)
  {
    data[x][y] = bgData | (data[x][y] & ZConsts::VegBitwiseMask);
  }
  
  TileData& GetTile(TileCoord x, TileCoord y)
  {
    return data[x][y];
  }

  const TileData& GetTile(TileCoord x, TileCoord y) const
  {
    return data[x][y];
  }

  void SetTileZombAmount(TileCoord x, TileCoord y, uint8_t zombAmount)
  {
    zombData[x][y] = zombAmount;
  }

  void SetTileDebugColor(TileCoord x, TileCoord y, ZConsts::TileDebugColors color)
  {
    data[x][y] = color;
  }

  TileData data[ZConsts::cellSize][ZConsts::cellSize]{ 0 };  //90 kb
  uint8_t zombData[30][30]{ 0 };
};

class SparseCellData
{
  //Figure out way to generate sparse
  //std::unordered_map<glm::ivec2, 
};

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

//Entire map class, contains interfaces for directly editing cells and tiles.
//Also includes general functions for manipulating the map and exporting it
class MapGenerator
{
public:
  MapGenerator();
  void SetSize(Application& app, int numXCells, int numYCells);

  //IMGUI UI
  void DrawUI(Application& app);
  void DrawMenu(Application& app);
  void Perlin(float perlinNoiseScalar = 2.0f);

  //https://en.wikipedia.org/wiki/Voronoi_diagram
  //Inefficient naive implementation
  //There is one called Fortunes but not sure if its faster for grid based
  void VoronoiDiagram(CellCoord x, CellCoord y, std::vector<glm::ivec2> points, VoronoiDiagram::VoronoiDiagramDistFuncs func);

  void ExportToPng();

  std::vector<std::vector<BackgroundRawDataView>> mapRawData;

  std::string MapName = "TestMap";

  //Both const and non-const tile data's to call const and non const BackgroundData::getTile funcs
  const TileData& GetTile(CellCoord cellX, CellCoord cellY, TileCoord tileX, TileCoord tileY) const
  {
    return mapRawData[cellX][cellY].getData().GetTile(tileX, tileY);
  }

  TileData& GetTile(CellCoord cellX, CellCoord cellY, TileCoord tileX, TileCoord tileY)
  {
    return mapRawData[cellX][cellY].getData().GetTile(tileX, tileY);
  }

  //Just non-const as zomb amount isnt really ever read
  uint8_t& GetZombTile(CellCoord cellX, CellCoord cellY, TileCoord tileX, TileCoord tileY)
  {
    return mapRawData[cellX][cellY].getData().zombData[tileX][tileY];
  }


  void SetTile(WorldCoord x, WorldCoord y, const ZConsts::TileVegData& vegData)
  {
    TileData& tileData = GetTile(x / ZConsts::cellSize, y / ZConsts::cellSize, x % ZConsts::cellSize, y % ZConsts::cellSize);
    tileData = vegData | (tileData & ZConsts::BGBitwiseMask);
  }

  void SetTile(WorldCoord x, WorldCoord y, const ZConsts::TileBackgroundData& bgData)
  {
    TileData& tileData = GetTile(x / ZConsts::cellSize, y / ZConsts::cellSize, x % ZConsts::cellSize, y % ZConsts::cellSize);
    tileData = bgData | (tileData & ZConsts::VegBitwiseMask);
  }

  void SetTileZombAmount(WorldCoord x, WorldCoord y, uint8_t zombAmount)
  {
    uint8_t& tileData = GetZombTile(x / ZConsts::cellSize, y / ZConsts::cellSize, x % ZConsts::cellSize, y % ZConsts::cellSize);
    tileData = zombAmount;
  }

  void SetTileDebugColor(WorldCoord x, WorldCoord y, ZConsts::TileDebugColors color)
  {
    TileData& tileData = GetTile(x / ZConsts::cellSize, y / ZConsts::cellSize, x % ZConsts::cellSize, y % ZConsts::cellSize);
    tileData = color;
  }
  
  std::vector<glm::ivec2> voronoiPoints;
  int numCities = 1;
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

};

// A set of solid colors for drawing onto a dark gray background
static const ImVec4 colorSoftLightGray = { 0.9f, 0.9f, 0.9f, 0.415f };
static const ImVec4 colorSoftDarkGray = { 0.26f, 0.26f, 0.26f, 0.415f };
static const ImVec4 colorSoftLightGreen = { 0.74f, 0.98f, 0.74f, 0.415f };
static const ImVec4 colorSoftBlue = { 0.56f, 0.56f, 0.95f, 1.f };
static const ImVec4 colorSoftWhiteBlue = { 0.74f, 0.74f, 0.98f, 1.f };
