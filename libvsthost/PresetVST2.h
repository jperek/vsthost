#ifndef PRESETVST2_H
#define PRESETVST2_H

#include <memory>

#include "pluginterfaces\base\fplatform.h"
#include "pluginterfaces\vst2.x\aeffectx.h"
#include "pluginterfaces\vst2.x\vstfxstore.h"

#include "Preset.h"

namespace VSTHost {
class PluginVST2;
class PresetVST2 : public Preset {
public:
	static const std::string kExtension;
	PresetVST2(PluginVST2& p);
	~PresetVST2();
	bool Load() override;
	bool Load(const std::string& path) override;
	bool Save() override;
	bool Save(const std::string& path) override;
private:
	void SetState() override;
	void GetState() override;
	void SwapProgram();
	bool ProgramChunks() const;
	static bool SwapNeeded();
	static const size_t kProgramUnionSize;	// sizeof(fxProgram::content)
	fxProgram* program;
	std::unique_ptr<char[]> program_data;
	size_t fxprogram_size; // size of fxprogram in this particular instance, without 2 first values
	bool program_chunks;
	PluginVST2& plugin;
};
} // namespace

#endif