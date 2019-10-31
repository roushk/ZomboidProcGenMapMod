#include "pch.hpp"

#include "application.hpp"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  Application app(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

  app.run();

  return EXIT_SUCCESS;
}
