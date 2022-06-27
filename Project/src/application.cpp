#include "pch.hpp"
#include "application.hpp"

#include "project_example.hpp"
#include "project_zomb_proc_gen.hpp"


Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
  : window_instance(hInstance)
{
  WNDCLASSEX wc = {};

  // set window struct variables
  wc.cbSize = sizeof WNDCLASSEX;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = win_proc_setup;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = window_instance;
  wc.hIcon = nullptr;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.lpszClassName = "Application";
  wc.hIconSm = nullptr;
  RegisterClassEx(&wc);

  // store info 1280×720
  RECT rect = { 350, 100, 350 + windowWidth, 100 + windowHeight };
  AdjustWindowRect(&rect, WS_THICKFRAME ^ WS_CAPTION, FALSE);
  window_handle = CreateWindow("Application",  // Window to create (matches wc window name string)
                               "ImGui 2D Renderer Framework", // Title bar text
                               WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME, // Window buttons
                               100, // window left position
                               50, // window top position
                               windowWidth, // window width
                               windowHeight, // window height
                               nullptr, // no parent window
                               nullptr, // no menu
                               window_instance,   // handle to the instance
                               this);   // no params passed

  if (window_handle == nullptr)
  {
    running = false;
    return;
  }
  
  if (create_device() < 0)
  {
    running = false;
    UnregisterClass("Application", window_instance);
    return;
  }

  ShowWindow(window_handle, nCmdShow);
  UpdateWindow(window_handle);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.IniFilename = nullptr; // Disable automatic imgui ini file saving/loading of window locations and data
  io.MouseDragThreshold = 1.f; // set mouse drag threshold (for dragging the control points)

  // Setup Platform/Renderer bindings
  ImGui_ImplWin32_Init(window_handle);
  ImGui_ImplDX11_Init(d3d_device.Get(), d3d_device_context.Get());

  // Setup Style
  ImGui::StyleColorsDark();

  init_projects();
}

Application::~Application()
{
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  UnregisterClass("Application", window_instance);
}

void Application::run()
{
  // A nice, clean gray
  const auto clearColor = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

  while(running)
  {
    handle_messages();

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    RECT rect;
    GetClientRect(window_handle, &rect);
    const auto clientHeight = rect.bottom - rect.top;
    const auto clientWidth = rect.right - rect.left;

    const float menuHeight = draw_menu_bar();

    ImGui::SetNextWindowSize({ static_cast<float>(clientWidth), static_cast<float>(clientHeight - menuHeight) });
    ImGui::SetNextWindowPos({ 0.f, menuHeight });
    ImGui::SetNextWindowBgAlpha( 0.0f );
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    if(ImGui::Begin("Screen", nullptr, 
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | 
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
      if (currentProject)
        currentProject->draw();
    }
    ImGui::End();
    ImGui::PopStyleVar();

    if (currentProject)
      currentProject->draw_editors();

    if(performanceController)
    {
      static auto fpsWindowPos = ImVec2{ clientWidth * 0.85f, (clientHeight - menuHeight) * 0.95f };

      // place counter bottom right of screen
      ImGui::SetNextWindowPos(fpsWindowPos);
      ImGui::SetNextWindowBgAlpha(0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
      if (ImGui::Begin("##Performance", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus))
      {
        const auto& io = ImGui::GetIO();
        const auto fpsString = fmt::format("{:.2f} FPS", io.Framerate);
        ImGui::Text(fpsString.c_str());

        const auto windowSize = ImGui::GetWindowSize();
        fpsWindowPos = { clientWidth - windowSize.x - 5.f, clientHeight - windowSize.y - 5.f };
      }
      ImGui::End();
      ImGui::PopStyleVar();
    }

    ImGui::Render();
    d3d_device_context->OMSetRenderTargets(1, main_render_target_view.GetAddressOf(), nullptr);
    d3d_device_context->ClearRenderTargetView(main_render_target_view.Get(), (float*)&clearColor);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    //swap_chain->Present(1, 0); // Present with vsync
    swap_chain->Present(0, 0); // Present without vsync
  }
}

float Application::draw_menu_bar()
{
  float menuHeight = 0.f;

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Clear"))
      {
        if (currentProject)
          currentProject->reset();
      }

      if (ImGui::MenuItem("Exit", "Alt+F4"))
      {
        running = false;
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Project"))
    {
      for(const auto& projectPtr : projects)
      {
        if(ImGui::MenuItem(projectPtr->name().c_str(), nullptr, currentProject == projectPtr.get()))
        {
          if(currentProject)
            currentProject->reset();
          currentProject = projectPtr.get();
        }
      }

      ImGui::EndMenu();
    }

    if (currentProject)
      currentProject->draw_menus();

    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("FPS", "F3", performanceController))
      {
        performanceController ^= 1;
      }

      ImGui::EndMenu();
    }

    menuHeight = ImGui::GetWindowSize().y;

    ImGui::EndMainMenuBar();
  }

  return menuHeight;
}

void Application::init_projects()
{
  // Load projects here
  projects.emplace_back(std::make_unique<ProjectZombProcGen>(ProjectZombProcGen{*this}));
  // projects.emplace_back(std::make_unique<Project1>(Project1{}));
  // projects.emplace_back(std::make_unique<Project2>(Project2{}));
  // ... etc

  // set startup project
  currentProject = projects[0].get();
}

HRESULT Application::create_device()
{    
  // Setup swap chain
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = window_handle;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  UINT createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_FEATURE_LEVEL featureLevel;
  const D3D_FEATURE_LEVEL featureLevelArray[4] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, };
  if (D3D11CreateDeviceAndSwapChain(nullptr, 
    D3D_DRIVER_TYPE_HARDWARE, 
    nullptr, 
    createDeviceFlags, 
    featureLevelArray, 
    4, 
    D3D11_SDK_VERSION, 
    &sd, 
    swap_chain.GetAddressOf(), 
    d3d_device.GetAddressOf(), 
    &featureLevel, 
    d3d_device_context.GetAddressOf()) != S_OK)
      return E_FAIL;

  {
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    d3d_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, main_render_target_view.GetAddressOf());
  }

  return S_OK;
}

void Application::handle_messages()
{
  MSG msg;
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
    {
      running = false;
    }
  }
}

LRESULT Application::win_proc_setup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
  if (msg == WM_NCCREATE)
  {
    // extract ptr to window class from creation data
    const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
    const auto windowPtr = reinterpret_cast<Application*>(pCreate->lpCreateParams);
    // sanity check
    if (windowPtr == nullptr) return 0;
    // set WinAPI-managed user data to store ptr to window class
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
    // set message proc to normal (non-setup) handler now that setup is finished
    SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Application::win_proc_thunk));
    // forward message to window class handler
    return windowPtr->win_proc(hWnd, msg, wParam, lParam);
  }
  // if we get a message before the WM_NCCREATE message, handle with default handler
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::win_proc_thunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	    
  // retrieve ptr to window class
  const auto windowPtr = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  // forward message to window class handler
  return windowPtr->win_proc(hWnd, msg, wParam, lParam);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Application::win_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    return true;
  
  switch (msg)
  {
  case WM_CLOSE: { // On click red X or alt+f4
    running = false;
    return 0;
  } break;
  case WM_KEYDOWN: {
    if(wParam == VK_F3)
    {
      performanceController ^= 1;
    }
    return 0;
  } break;
  case WM_SIZE: { // On Resize
    d3d_device_context->OMSetRenderTargets(0, 0, 0);
    main_render_target_view->Release();

    HRESULT hr = swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    {
      Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
      swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
      d3d_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, main_render_target_view.GetAddressOf());
    }

    d3d_device_context->OMSetRenderTargets(1, main_render_target_view.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = LOWORD(lParam);
    vp.Height = HIWORD(lParam);
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    d3d_device_context->RSSetViewports(1, &vp);

    windowWidth = static_cast<int>(LOWORD(lParam));
    windowHeight = static_cast<int>(HIWORD(lParam));
  }
  default:
    break; // do nothing
  }

  // if we don't handle it, have the default win proc handle it
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Specifying a dynamic texture to be used to draw to
HRESULT Application::createArrayTexture(CellBackgroundRawData* cellData, ID3D11ShaderResourceView** m_textureView, ID3D11Texture2D** pTexture)
{
  *pTexture = NULL;
  *m_textureView = NULL;

  D3D11_TEXTURE2D_DESC desc;
  desc.Width = ZConsts::cellSize;
  desc.Height = ZConsts::cellSize;
  desc.MipLevels = desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  desc.MiscFlags = NULL;
  HRESULT result = d3d_device->CreateTexture2D(&desc, NULL, pTexture);

  if (result != S_OK)
    return result;

  // Create texture view
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = desc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;

  result = d3d_device->CreateShaderResourceView(*pTexture, &srvDesc, m_textureView);

  return result;
}

//Update the texture and sync CPU and GPU
void Application::updateArrayTexture(CellBackgroundRawData* cellData, ID3D11Texture2D* pTexture, const UpdateArraySetting setting)
{
  //Update subresource good for rarer operations, map and unmap good for often
  //d3d_device_context->UpdateSubresource(pTexture,D3D11CalcSubresource(0, 0, 1),nullptr, data, rowPitch, depthPitch);

  D3D11_MAPPED_SUBRESOURCE resource;
  HRESULT result = d3d_device_context->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

  UCHAR* pTexels = (UCHAR*)resource.pData;

  for (UINT row = 0; row < ZConsts::cellSize; row++)
  {
    UINT rowStart = row * resource.RowPitch;
    for (UINT col = 0; col < ZConsts::cellSize; col++)
    {
      glm::ivec3 color = { 255, 255, 255 };

      if (setting == UpdateArraySetting::Background)
      {
        //BG Data
        //Take cell data at row,col and get that byte, & it with the relevent bits and cast to TileBackgroundData
        ZConsts::TileBackgroundData value = static_cast<ZConsts::TileBackgroundData>(cellData->data[row][col] & 0b00001111);

        if (ZConsts::TileColors.find(value) == ZConsts::TileColors.end())
          color = ZConsts::TileColors.at(ZConsts::Tile_BG_None);
        else
          color = ZConsts::TileColors.at(value);
      }
      else if (setting == UpdateArraySetting::Veg)
      {
        //veg Data
        ZConsts::TileVegData value = static_cast<ZConsts::TileVegData>(cellData->data[row][col] & 0b11110000);

        if (ZConsts::VegColors.find(value) == ZConsts::VegColors.end())
          color = ZConsts::VegColors.at(ZConsts::Tile_VEG_None);
        else
          color = ZConsts::VegColors.at(value);
      }
      else if (setting == UpdateArraySetting::Debug)
      {
        //debug Data
        byte value = static_cast<ZConsts::TileDebugColors>(cellData->data[row][col]);
        if (ZConsts::DebugColorsMap.find(value) == ZConsts::DebugColorsMap.end())
          color = ZConsts::DebugColorsMap.at(0);
        else
          color = ZConsts::DebugColorsMap.at(value);
      }

      UINT colStart = col * 4;
      pTexels[rowStart + colStart + 0] = color.x; // Red
      pTexels[rowStart + colStart + 1] = color.y; // Green
      pTexels[rowStart + colStart + 2] = color.z;  // Blue
      pTexels[rowStart + colStart + 3] = 255;  // Alpha
    }
  }

  d3d_device_context->Unmap(pTexture, 0);
}


//TODO Cleanup createArrayTexture Func

