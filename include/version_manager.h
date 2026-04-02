#ifndef VERSION_MANAGER_H
#define VERSION_MANAGER_H

#include <string>
#include <fstream>
#include <sstream>

class VersionManager {
private:
    static const std::string CONFIG_FILE;
    static const std::string BASE_VERSION;
    
public:
    static std::string getVersion() {
        std::string version = BASE_VERSION;
        
        std::ifstream configFile(CONFIG_FILE);
        if (configFile.is_open()) {
            std::string line;
            while (std::getline(configFile, line)) {
                if (line.substr(0, 8) == "version=") {
                    version = line.substr(8);
                    break;
                }
            }
            configFile.close();
        }
        
        return version;
    }
};

const std::string VersionManager::CONFIG_FILE = "version.ini";
const std::string VersionManager::BASE_VERSION = "v1.1";

#endif // VERSION_MANAGER_H