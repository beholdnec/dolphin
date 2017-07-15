// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

// This audio backend uses XAudio2 via XAudio2_7.dll
// This version of the library is included in the June 2010 DirectX SDK and
// works on all versions of Windows, however the SDK and/or redist must be
// separately installed.
// Therefore this backend is available iff:
//  * SDK is available at compile-time
//  * runtime dll is available at runtime
// Dolphin ships the relevant SDK headers in Externals, so it's always available.

#pragma once

#include <memory>
#include "AudioCommon/SoundStream.h"
#include "Common/Event.h"

#ifdef _WIN32

#include <Windows.h>

// Disable warning C4265 in wrl/client.h:
//   'Microsoft::WRL::Details::RemoveIUnknownBase<T>': class has virtual functions,
//   but destructor is not virtual
#pragma warning(push)
#pragma warning(disable : 4265)
#include <wrl/client.h>
#pragma warning(pop)

using Microsoft::WRL::ComPtr;

struct StreamingVoiceContext2_7;
struct IXAudio2;
struct IXAudio2MasteringVoice;

#endif

class XAudio2_7 final : public SoundStream
{
#ifdef _WIN32

private:
  ComPtr<IXAudio2> m_xaudio2;
  std::unique_ptr<StreamingVoiceContext2_7> m_voice_context;
  // all XAudio2 objects are released when m_xaudio2 is released
  IXAudio2MasteringVoice* m_mastering_voice;

  Common::Event m_sound_sync_event;
  float m_volume;

  const bool m_cleanup_com;

  static HMODULE m_xaudio2_dll;

  static bool InitLibrary();

public:
  XAudio2_7();
  virtual ~XAudio2_7();

  bool Start() override;
  void Stop() override;

  void Clear(bool mute) override;
  void SetVolume(int volume) override;

  static bool isValid() { return InitLibrary(); }
#endif
};
