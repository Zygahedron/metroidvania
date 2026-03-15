#ifndef LAZ_ASSET_MANAGER_HPP
#define LAZ_ASSET_MANAGER_HPP

#include <string>
#include <unordered_map>

namespace laz
{

template <typename T>
class AssetManager
{
public:
  AssetManager(const std::string& directory, const std::string& fileExtension)
    : _directory(directory), _fileExtension(fileExtension) {}
  ~AssetManager() {}

  T& load(const std::string& name, const std::string& filepath = "")
  {
    if (this->_assets.contains(name))
      return *this->_assets[name];

    this->_assets[name] = this->loadFromFile(filepath == "" ? name : filepath);
    return *this->_assets[name];
  }

  T& get(const std::string& name) const
  {
    return *this->_assets.at(name);
  }

  void unload(const std::string& name)
  {
    if (!this->_assets.contains(name))
      return;

    this->_assets[name].unload();
    this->_assets[name] = nullptr;
  }

private:
  std::string _directory = "";
  std::string _fileExtension = "";

  std::unordered_map<std::string, T*> _assets = {};

  T* loadFromFile(const std::string& filepath);
};

}

#endif
