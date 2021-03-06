#ifndef PLUGIN_H
#define PLUGIN_H

#define NOMINMAX
#include <Windows.h>
#include <memory>
#include <string>
#include <mutex>

#include "pluginterfaces/vst/vsttypes.h"

namespace VSTHost {
class PluginWindow;
class Preset;
class Plugin {
public:
	enum IsValidCodes {
		kValid,
		kInvalid = 5,
		kWrongInOutNum,
		kIsNotEffect
	};
	// basic plugin interface
	Plugin(HMODULE m, Steinberg::Vst::SpeakerArrangement sa);
	virtual ~Plugin();
	virtual IsValidCodes IsValid() const = 0;
	virtual void Initialize(Steinberg::Vst::TSamples bs, Steinberg::Vst::SampleRate sr) = 0;
	virtual std::basic_string<TCHAR> GetPluginName() const = 0;
	virtual std::string GetPluginNameA() const = 0;
	virtual void Process(Steinberg::Vst::Sample32** input, Steinberg::Vst::Sample32** output, Steinberg::Vst::TSamples block_size) = 0;
	virtual void ProcessReplace(Steinberg::Vst::Sample32** input_output, Steinberg::Vst::TSamples block_size) = 0;
	virtual void SetBlockSize(Steinberg::Vst::TSamples bs) = 0;
	virtual void SetSampleRate(Steinberg::Vst::SampleRate sr) = 0;
	virtual bool SetSpeakerArrangement(Steinberg::Vst::SpeakerArrangement sa) = 0;
	std::string GetPluginFileName() const;
	std::string GetPluginDirectory() const;
	std::string GetPluginPath() const;
	// presets
	virtual Steinberg::uint32 GetProgramCount() const = 0;
	virtual void SetProgram(Steinberg::uint32 id) = 0;
	virtual std::basic_string<TCHAR> GetProgramName(Steinberg::uint32 id) = 0;
	virtual std::string GetProgramNameA(Steinberg::uint32 id) = 0;
	virtual std::string GetPresetExtension() = 0;
	// parameters
	virtual Steinberg::uint32 GetParameterCount() const = 0;
	virtual Steinberg::Vst::ParamValue GetParameter(Steinberg::Vst::ParamID id) const = 0;
	virtual void SetParameter(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue value) = 0;
	// active and bypass flags
	void SetActive(bool active_);
	bool IsActive() const;
	bool IsBypassed() const;
	virtual void SetBypass(bool bypass_) = 0;
	virtual bool BypassProcess() const = 0;
	// editor
	virtual bool HasEditor() const;
	virtual void CreateEditor(HWND hwnd = NULL) = 0;
	virtual Steinberg::uint32 GetEditorHeight() = 0;
	virtual Steinberg::uint32 GetEditorWidth() = 0;
	virtual void ShowEditor() = 0;
	virtual void HideEditor() = 0;
	// state
	bool SaveState();
	bool LoadState();
	bool SaveState(const std::string& path);
	bool LoadState(const std::string& path);
protected:
	virtual void Resume() = 0;
	virtual void Suspend() = 0;
	virtual void StartProcessing() = 0;
	virtual void StopProcessing() = 0;
	Steinberg::uint32 GetChannelCount();

	HMODULE module;
	std::mutex plugin_lock; // locked when plugin is processing or setting itself (in)active
	bool active{ false };
	bool bypass{ false };
	// can stereo
	bool proper_in_out_num{ false };
	Steinberg::Vst::TSamples block_size;
	Steinberg::Vst::SampleRate sample_rate;
	Steinberg::Vst::SpeakerArrangement speaker_arrangement{ Steinberg::Vst::SpeakerArr::kStereo };
	std::unique_ptr<Preset> state;
	bool has_editor{ false };
};
} // namespace

#endif