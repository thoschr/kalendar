#include "pluginmanager.h"

PluginManager::PluginManager()
{
    std::filesystem::path dir(TOOLS_FOLDER);
    std::filesystem::create_directory(dir); // Create the directory if it doesn't exist
    for (const auto& e : std::filesystem::directory_iterator(TOOLS_FOLDER)) {
        std::filesystem::path p = e.path();
        if ((p.extension() == ".sh") || (p.extension() == ".bat"))
            this->tools.push_back(p.filename().string()); // Convert filename to string
    }
}

void PluginManager::runTool(const std::string &name) {
    std::string cmd = std::string(TOOLS_FOLDER) + "/\"" + name + "\"";
    system(cmd.c_str()); // Execute the tool
}

std::vector<std::string> PluginManager::get_tools() {
    return this->tools;
}
