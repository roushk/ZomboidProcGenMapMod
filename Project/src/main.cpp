#include "pch.hpp"

#include "application.hpp"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  Application(hInstance, hPrevInstance, lpCmdLine, nCmdShow).run();

  return EXIT_SUCCESS;
}
