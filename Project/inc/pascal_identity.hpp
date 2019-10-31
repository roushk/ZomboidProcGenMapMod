#pragma once

inline int x_get_y(int x, int y)
{
  static std::vector<std::vector<int>> bernsteinPascalTriangle;

  if (y == 0 && y == x)
    return 1;

  if (y > x || x == 0 || y < 0)
    return 0;

  if (bernsteinPascalTriangle.size() > x && bernsteinPascalTriangle[x].size() == x + 1 && bernsteinPascalTriangle[x][y] != -1)
    return bernsteinPascalTriangle[x][y];

  if (bernsteinPascalTriangle.size() <= x) bernsteinPascalTriangle.resize(x + 1);
  if (bernsteinPascalTriangle[x].size() != x + 1) bernsteinPascalTriangle[x].resize(x + 1, -1);

  // pascal recursion identity
  return bernsteinPascalTriangle[x][y] = x_get_y(x - 1, y - 1) + x_get_y(x - 1, y);
}
