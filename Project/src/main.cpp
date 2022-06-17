#include "pch.hpp"
#include "application.hpp"
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
#ifdef _ENABLE_CONSOLE_
  //other code
  AllocConsole();
  freopen("CONIN$", "r", stdin);
  freopen("CONOUT$", "w", stdout);
  freopen("CONOUT$", "w", stderr);
#endif

  Application(hInstance, hPrevInstance, lpCmdLine, nCmdShow).run();

  return EXIT_SUCCESS;
}
