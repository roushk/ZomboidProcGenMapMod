#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>
#pragma comment(lib, "d3d11.lib")

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#define FMT_STRING_ALIAS 1
#include "fmt/format.h"

#include <string>
#include <string_view>
#include <cstdlib>
#include <vector>
#include <functional>
