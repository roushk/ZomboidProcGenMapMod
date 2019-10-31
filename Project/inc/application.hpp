#pragma once
#include "project.hpp"

class Application
{
public:
  Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
  ~Application();

  void run();

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
  const int windowWidth = 800;
  const int windowHeight = 600;

  bool performanceController = false;
};
