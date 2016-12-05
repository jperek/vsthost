#ifndef PRESET_H
#define PRESET_H

class Preset {
public:
	Preset() {}
	virtual ~Preset() {}
	virtual bool SetState() = 0;
	virtual void LoadFromFile() = 0;
	virtual void GetState() = 0;
	virtual void SaveToFile() = 0;
};

#endif