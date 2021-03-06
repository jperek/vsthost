#ifndef HOST_H
#define HOST_H

#include <string>
#include <memory>
#include <cstdint>
#include <windows.h>

namespace VSTHost {
class SpeakerArrangement {
public:
	enum Types {
		Mono = 1,
		Stereo = 2,
		Surround_5_1 = 6,
		Surround_7_1 = 8
	} type;
	SpeakerArrangement(Types t);
	explicit SpeakerArrangement(uint32_t channels);
	operator std::uint64_t() const;
};

class IHostController;
class HostController;
class HostObserver;

class Host {
friend HostController;
public:
	static constexpr auto kPluginDirectory{ ".\\..\\..\\plugins" };
	static constexpr auto kPluginList{ ".\\..\\vsthost.ini" };

	Host(std::int64_t max_num_samples, double sample_rate, SpeakerArrangement sa = SpeakerArrangement::Stereo); // max_num_samples is maximum number of samples per channel
	~Host();
	void Process(float** input, float** output, std::int64_t num_samples); // num_samples - samples per channel
	void Process(char* input, char* output, std::int64_t num_samples);
	void Process(std::int16_t* input, std::int16_t* output, std::int64_t num_samples);
	void ProcessReplace(float** input_output, std::int64_t num_samples);
	void ProcessReplace(char* input_output, std::int64_t num_samples);
	void ProcessReplace(std::int16_t* input_output, std::int64_t num_samples);
	void SetSampleRate(double sr);
	void SetBlockSize(std::int64_t bs);
	void SetSpeakerArrangement(SpeakerArrangement sa);
	void CreateGUIThread();
	void CreateGUI();
	bool LoadPluginList(const std::string& path);
	bool SavePluginList(const std::string& path) const;
	bool LoadPluginList();
	bool SavePluginList() const;
	IHostController* GetController();
private:
	class HostImpl;
	std::shared_ptr<HostImpl> impl;
};

class IHostController {
public:
	virtual ~IHostController() {}
	virtual bool LoadPluginList(const std::string& path) = 0;
	virtual bool SavePluginList(const std::string& path) const = 0;
	virtual bool LoadPluginList() = 0;
	virtual bool SavePluginList() const = 0;
	virtual std::uint32_t GetPluginCount() const = 0;
	virtual bool AddPlugin(const std::string& path) = 0;
	virtual void DeletePlugin(std::uint32_t idx) = 0;
	virtual void MoveUp(std::uint32_t idx) = 0;
	virtual void MoveDown(std::uint32_t idx) = 0;
	virtual std::string GetPluginName(std::uint32_t idx) const = 0;
	virtual bool HasEditor(std::uint32_t idx) const = 0;
	virtual void CreateEditor(std::uint32_t idx, HWND hwnd) = 0;
	virtual std::uint32_t GetPluginEditorHeight(std::uint32_t idx) = 0;
	virtual std::uint32_t GetPluginEditorWidth(std::uint32_t idx) = 0;
	virtual void ShowEditor(std::uint32_t idx) = 0;
	virtual void HideEditor(std::uint32_t idx) = 0;
	virtual bool IsBypassed(std::uint32_t idx) const = 0;
	virtual void SetBypass(std::uint32_t idx, bool bypass) = 0;
	virtual bool IsActive(std::uint32_t idx) const = 0;
	virtual void SetActive(std::uint32_t idx, bool active) = 0;
	virtual std::uint32_t GetPluginPresetCount(std::uint32_t idx) const = 0;
	virtual std::string GetPluginPresetName(std::uint32_t plugin_idx, std::uint32_t preset_idx) = 0;
	virtual void SetPluginPreset(std::uint32_t plugin_idx, std::uint32_t preset_idx) = 0;
	virtual std::string GetPluginPresetExtension(std::uint32_t idx) = 0;
	virtual bool SavePreset(std::uint32_t idx) = 0;
	virtual bool LoadPreset(std::uint32_t idx) = 0;
	virtual bool SavePreset(std::uint32_t idx, const std::string& path) = 0;
	virtual bool LoadPreset(std::uint32_t idx, const std::string& path) = 0;
	virtual void RegisterObserver(HostObserver* o) = 0;
	virtual void UnregisterObserver(HostObserver* o) = 0;
};
} // namespace

#endif