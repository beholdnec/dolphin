// Copyright 2010 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_2.h>
#include <vector>

// Disable warning C4265 in wrl/client.h:
//   'Microsoft::WRL::Details::RemoveIUnknownBase<T>': class has virtual functions,
//   but destructor is not virtual
#pragma warning(push)
#pragma warning(disable : 4265)
#include <wrl/client.h>
#pragma warning(pop)

#include "Common/Common.h"
#include "Common/CommonTypes.h"
#include "Common/MsgHandler.h"

namespace DX11
{
using Microsoft::WRL::ComPtr;

#define SAFE_DELETE(x)                                                                             \
  {                                                                                                \
    delete (x);                                                                                    \
    (x) = nullptr;                                                                                 \
  }
#define SAFE_DELETE_ARRAY(x)                                                                       \
  {                                                                                                \
    delete[](x);                                                                                   \
    (x) = nullptr;                                                                                 \
  }
#define CHECK(cond, Message, ...)                                                                  \
  if (!(cond))                                                                                     \
  {                                                                                                \
    PanicAlert(__FUNCTION__ " failed in %s at line %d: " Message, __FILE__, __LINE__,              \
               __VA_ARGS__);                                                                       \
  }

class D3DTexture2D;

namespace D3D
{
HRESULT LoadDXGI();
HRESULT LoadD3D();
HRESULT LoadD3DCompiler();
void UnloadDXGI();
void UnloadD3D();
void UnloadD3DCompiler();

D3D_FEATURE_LEVEL GetFeatureLevel(IDXGIAdapter* adapter);
std::vector<DXGI_SAMPLE_DESC> EnumAAModes(IDXGIAdapter* adapter);

HRESULT Create(HWND wnd);
void Close();

extern ComPtr<ID3D11Device> device;
extern ComPtr<ID3D11DeviceContext> context;
extern HWND hWnd;
extern bool bFrameInProgress;

void Reset();
bool BeginFrame();
void EndFrame();
void Present();

unsigned int GetBackBufferWidth();
unsigned int GetBackBufferHeight();
D3DTexture2D& GetBackBuffer();
const char* PixelShaderVersionString();
const char* GeometryShaderVersionString();
const char* VertexShaderVersionString();
bool BGRATexturesSupported();

u32 GetMaxTextureSize(D3D_FEATURE_LEVEL feature_level);

HRESULT SetFullscreenState(bool enable_fullscreen);
bool GetFullscreenState();

// This function will assign a name to the given resource.
// The DirectX debug layer will make it easier to identify resources that way,
// e.g. when listing up all resources who have unreleased references.
void SetDebugObjectName(ID3D11DeviceChild* resource, const char* name);
std::string GetDebugObjectName(ID3D11DeviceChild* resource);

// Convenience function for calling D3D::context->OMSetRenderTargets with 1 target
void SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv = nullptr);

}  // namespace D3D

typedef HRESULT(WINAPI* CREATEDXGIFACTORY)(REFIID, void**);
extern CREATEDXGIFACTORY PCreateDXGIFactory;
typedef HRESULT(WINAPI* D3D11CREATEDEVICE)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT,
                                           CONST D3D_FEATURE_LEVEL*, UINT, UINT, ID3D11Device**,
                                           D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

typedef HRESULT(WINAPI* D3DREFLECT)(LPCVOID, SIZE_T, REFIID, void**);
extern D3DREFLECT PD3DReflect;
extern pD3DCompile PD3DCompile;

}  // namespace DX11
