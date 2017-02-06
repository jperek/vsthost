#include "PluginManager.h"

#include <iostream>
#include <fstream>
#include <windows.h>
//#include "Shlwapi.h"

#include "Plugin.h"
#include "PluginLoader.h"

namespace VSTHost {
const std::string PluginManager::kPluginList{ "vsthost.ini" };

PluginManager::PluginIterator::PluginIterator(std::vector<std::unique_ptr<Plugin>>::iterator i) : it(i) {}
bool PluginManager::PluginIterator::operator!=(PluginIterator& i) { return it != i.it; }
PluginManager::PluginIterator PluginManager::PluginIterator::operator++() { return PluginIterator(++it); }
PluginManager::PluginIterator PluginManager::PluginIterator::operator++(int) { return PluginIterator(it++); }
Plugin& PluginManager::PluginIterator::operator*() { return *(*it); }

PluginManager::PluginManager(Steinberg::Vst::TSamples bs, Steinberg::Vst::SampleRate sr, Steinberg::FUnknown* context)
	: def_block_size(bs), def_sample_rate(sr), vst3_context(context) {

}

PluginManager::IndexType PluginManager::Size() const {
	return plugins.size();
}

Plugin& PluginManager::GetAt(PluginManager::IndexType i) const {
	return *plugins[i];
}

Plugin& PluginManager::operator[](IndexType i) const {
	return GetAt(i);
}

Plugin& PluginManager::Front() const {
	return *plugins[0]; // checking whether there is at least 1 plugin is on the user
}

Plugin& PluginManager::Back() const {
	return *plugins[Size() - 1]; // same
}

std::mutex& PluginManager::GetLock() {
	return manager_lock;
}

PluginManager::PluginIterator PluginManager::Begin() {
	return PluginIterator(plugins.begin());
}

PluginManager::PluginIterator PluginManager::End() {
	return PluginIterator(plugins.end());
}

bool PluginManager::Add(const std::string& path) {
	auto plugin = PluginLoader::Load(path, vst3_context);
	if (plugin) { // host now owns what plugin points at
		std::cout << "Loaded " << path << "." << std::endl;
		plugin->Initialize(def_block_size, def_sample_rate);
		plugins.push_back(std::move(plugin));
		return true;
	}
	return false;
}

void PluginManager::Delete(IndexType i) {
	if (i < Size()) {
		std::lock_guard<std::mutex> lock(manager_lock);
		plugins.erase(plugins.begin() + i);
	}
}

void PluginManager::Swap(IndexType i, IndexType j) {
	if (i < Size() && j < Size())
		std::swap(plugins[i], plugins[j]);
}

const std::string& PluginManager::GetDefaultPluginListPath() {
	return kPluginList;
}

bool PluginManager::LoadPluginList(const std::string& path) {
	std::string line;
	std::ifstream list(path);
	if (list.is_open()) {
		while (getline(list, line))
			if (!line.empty())
				Add(line);
		for (decltype(Size()) i = 0; i < Size(); ++i)
			GetAt(i).LoadStateFromFile();
		list.close();
		return true;
	}
	else
		std::cout << " Could not open " << path << '.' << std::endl;
	return false;
}

bool PluginManager::SavePluginList(const std::string& path) const {
	char abs[MAX_PATH]{};
	_fullpath(abs, ".", MAX_PATH);
	std::string absolute(abs);
	std::ofstream list(path, std::ofstream::out | std::ofstream::trunc);
	if (list.is_open()) {
		for (decltype(Size()) i = 0; i < Size(); ++i) {
			/*
			char buf[MAX_PATH]{};
			if (PathRelativePathToA(buf, absolute.c_str(), FILE_ATTRIBUTE_DIRECTORY, GetAt(i).GetPluginPath().c_str(), 0) == TRUE)
				list << std::string(buf) << std::endl; // todo: write own function for this
			else
				list << GetAt(i).GetPluginPath();
			*/
			list << GetAt(i).GetPluginPath();
		}
		list.close();
		return true;
	}
	return false;
}

void PluginManager::SetBlockSize(Steinberg::Vst::TSamples bs) {
	def_block_size = bs;
}

void PluginManager::SetSampleRate(Steinberg::Vst::SampleRate sr) {
	def_sample_rate = sr;
}
} // namespace