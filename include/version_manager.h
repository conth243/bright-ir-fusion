#ifndef VERSION_MANAGER_H
#define VERSION_MANAGER_H

#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

class VersionManager {
private:
    static const std::string CONFIG_FILE;
    static const std::string BASE_VERSION;
    
public:
    static std::string getVersion() {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        
        char dateTimeBuffer[20];
        std::strftime(dateTimeBuffer, sizeof(dateTimeBuffer), "%Y%m%d%H%M", localTime);
        std::string dateTimeStr = dateTimeBuffer;
        
        int serialNumber = getAndIncrementSerialNumber(dateTimeStr.substr(0, 8));
        
        std::ostringstream versionStream;
        versionStream << BASE_VERSION << "_" << dateTimeStr << "_" << std::setw(3) << std::setfill('0') << serialNumber;
        
        return versionStream.str();
    }
    
private:
    static int getAndIncrementSerialNumber(const std::string& date) {
        int serialNumber = 1;
        
        std::ifstream configFile(CONFIG_FILE);
        if (configFile.is_open()) {
            std::string line;
            while (std::getline(configFile, line)) {
                if (line.substr(0, 8) == date) {
                    serialNumber = std::stoi(line.substr(9)) + 1;
                    break;
                }
            }
            configFile.close();
        }
        
        std::ofstream outFile(CONFIG_FILE);
        if (outFile.is_open()) {
            std::ifstream inFile(CONFIG_FILE);
            if (inFile.is_open()) {
                std::string line;
                bool updated = false;
                while (std::getline(inFile, line)) {
                    if (line.substr(0, 8) == date) {
                        outFile << date << " " << serialNumber << std::endl;
                        updated = true;
                    } else {
                        outFile << line << std::endl;
                    }
                }
                inFile.close();
                
                if (!updated) {
                    outFile << date << " " << serialNumber << std::endl;
                }
            } else {
                outFile << date << " " << serialNumber << std::endl;
            }
            outFile.close();
        }
        
        return serialNumber;
    }
};

const std::string VersionManager::CONFIG_FILE = "version.ini";
const std::string VersionManager::BASE_VERSION = "v1.1";

#endif // VERSION_MANAGER_H