#ifndef PRESET_H
#define PRESET_H

namespace VSTHost {
class Preset {
public:
	virtual ~Preset() {}
	virtual void SetState() = 0;
	virtual void LoadFromFile() = 0;
	virtual void GetState() = 0;
	virtual void SaveToFile() = 0;
};
} // namespace

#endif