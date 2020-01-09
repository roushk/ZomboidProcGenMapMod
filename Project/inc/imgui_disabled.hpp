#pragma once

// Not a fan of this syntax, but it works and isn't too ugly I guess
#define IMGUI_DISABLED(x) ImGui::Disabled d(x); d

namespace ImGui
{
  class Disabled
  {
  public:
    Disabled(bool input)
      : isDisabled(!input)
    {
      if (isDisabled)
      {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      }
    }

    ~Disabled()
    {
      if (isDisabled)
      {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      }
    }

    operator bool() const
    {
      return true;
    }

  private:
    bool isDisabled;
  };
}
