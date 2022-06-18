#pragma once
#include "project.hpp"

struct CellBackgroundRawData;

class Application
{
public:
  Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
  ~Application();

  void run();

  static int WindowWidth()
  {
    return windowWidth;
  }

  static int WindowHeight()
  {
    return windowHeight;
  }
  
  enum class UpdateArraySetting
  {
    Background,
    Veg
  };

  HRESULT createArrayTexture(CellBackgroundRawData* cellData, ID3D11ShaderResourceView** m_textureView, ID3D11Texture2D** pTexture);
  void updateArrayTexture(CellBackgroundRawData* cellData, ID3D11Texture2D* pTexture, const UpdateArraySetting setting);
  //TODO Cleanup createArrayTexture Func

private:
  float draw_menu_bar();

  void init_projects();

  HRESULT create_device();

  void handle_messages();

  static LRESULT WINAPI win_proc_setup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  static LRESULT WINAPI win_proc_thunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  LRESULT win_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  Microsoft::WRL::ComPtr<ID3D11Device>            d3d_device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3d_device_context;
  Microsoft::WRL::ComPtr<IDXGISwapChain>          swap_chain;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  main_render_target_view;
  HWND window_handle;
  HINSTANCE window_instance;

  std::vector<std::unique_ptr<Project>> projects;
  Project* currentProject = nullptr;

  bool running = true;
  static inline int windowWidth = 800;
  static inline int windowHeight = 600;

  bool performanceController = false;
};
