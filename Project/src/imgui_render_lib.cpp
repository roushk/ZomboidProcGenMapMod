#include <pch.hpp>
#include "imgui_render_lib.hpp"

namespace ImGui
{
  static ImVec2 viewRectMin, viewRectMax;

  IMGUI_API void RenderLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness)
  {
    GetWindowDrawList()->AddLine(a, b, col, thickness);
  }

  IMGUI_API void RenderRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags, float thickness)
  {
    GetWindowDrawList()->AddRect(a, b, col, rounding, rounding_corners_flags, thickness);
  }

  IMGUI_API void RenderRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags)
  {
    GetWindowDrawList()->AddRectFilled(a, b, col, rounding, rounding_corners_flags);
  }

  IMGUI_API void RenderRectFilledMultiColor(const ImVec2& a, const ImVec2& b, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
  {
    GetWindowDrawList()->AddRectFilledMultiColor(a, b, col_upr_left, col_upr_right, col_bot_right, col_bot_left);
  }

  IMGUI_API void RenderQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness)
  {
    GetWindowDrawList()->AddQuad(a, b, c, d, col, thickness);
  }

  IMGUI_API void RenderQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col)
  {
    GetWindowDrawList()->AddQuadFilled(a, b, c, d, col);
  }

  IMGUI_API void RenderTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness)
  {
    GetWindowDrawList()->AddTriangle(a, b, c, col, thickness);
  }

  IMGUI_API void RenderTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col)
  {
    GetWindowDrawList()->AddTriangleFilled(a, b, c, col);
  }

  IMGUI_API void RenderCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments, float thickness)
  {
    GetWindowDrawList()->AddCircle(centre, radius, col, num_segments, thickness);
  }

  IMGUI_API void RenderCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments)
  {
    GetWindowDrawList()->AddCircleFilled(centre, radius, col, num_segments);
  }

  IMGUI_API void RenderText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
  {
    GetWindowDrawList()->AddText(pos, col, text_begin, text_end);
  }

  IMGUI_API void RenderText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
  {
    GetWindowDrawList()->AddText(font, font_size, pos, col, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
  }

  IMGUI_API void RenderImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col)
  {
    GetWindowDrawList()->AddImage(user_texture_id, a, b, uv_a, uv_b, col);
  }

  IMGUI_API void RenderImageQuad(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
  {
    GetWindowDrawList()->AddImageQuad(user_texture_id, a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);
  }

  IMGUI_API void RenderImageRounded(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col, float rounding, int rounding_corners)
  {
    GetWindowDrawList()->AddImageRounded(user_texture_id, a, b, uv_a, uv_b, col, rounding, rounding_corners);
  }

  IMGUI_API void RenderPolyline(const ImVec2* points, const int num_points, ImU32 col, bool closed, float thickness)
  {
    GetWindowDrawList()->AddPolyline(points, num_points, col, closed, thickness);
  }

  IMGUI_API void RenderConvexPolyFilled(const ImVec2* points, const int num_points, ImU32 col)
  {
    GetWindowDrawList()->AddConvexPolyFilled(points, num_points, col);
  }

  IMGUI_API void RenderBezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments)
  {
    GetWindowDrawList()->AddBezierCurve(pos0, cp0, cp1, pos1, col, thickness, num_segments);
  }


}

