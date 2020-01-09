#pragma once

namespace ImGui
{
  IMGUI_API void SetViewRect(const ImVec2& min, const ImVec2& max);
  IMGUI_API std::pair<ImVec2, ImVec2> GetViewRect();
  IMGUI_API ImVec2 GetScreenPos(const ImVec2& world_pos);
  IMGUI_API ImVec2 GetWorldPos(const ImVec2& screen_pos);
  IMGUI_API void RenderLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.0f);
  IMGUI_API void RenderRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ImDrawCornerFlags_All, float thickness = 1.0f);   // a: upper-left, b: lower-right, rounding_corners_flags: 4-bits corresponding to which corner to round
  IMGUI_API void RenderRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ImDrawCornerFlags_All);                     // a: upper-left, b: lower-right
  IMGUI_API void RenderRectFilledMultiColor(const ImVec2& a, const ImVec2& b, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
  IMGUI_API void RenderQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness = 1.0f);
  IMGUI_API void RenderQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col);
  IMGUI_API void RenderTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness = 1.0f);
  IMGUI_API void RenderTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col);
  IMGUI_API void RenderCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments = 48, float thickness = 1.0f);
  IMGUI_API void RenderCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments = 48);
  IMGUI_API void RenderText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
  IMGUI_API void RenderText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4 * cpu_fine_clip_rect = NULL);
  IMGUI_API void RenderImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a = ImVec2(0, 0), const ImVec2& uv_b = ImVec2(1, 1), ImU32 col = 0xFFFFFFFF);
  IMGUI_API void RenderImageQuad(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a = ImVec2(0, 0), const ImVec2& uv_b = ImVec2(1, 0), const ImVec2& uv_c = ImVec2(1, 1), const ImVec2& uv_d = ImVec2(0, 1), ImU32 col = 0xFFFFFFFF);
  IMGUI_API void RenderImageRounded(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col, float rounding, int rounding_corners = ImDrawCornerFlags_All);
  IMGUI_API void RenderPolyline(const ImVec2* points, const int num_points, ImU32 col, bool closed, float thickness);
  IMGUI_API void RenderConvexPolyFilled(const ImVec2* points, const int num_points, ImU32 col); // Note: Anti-aliased filling requires points to be in clockwise order.
  IMGUI_API void RenderBezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments = 0);
}
