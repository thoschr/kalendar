#include "pluginmanager.h"

PluginManager::PluginManager()
{
    for (experimental::filesystem::directory_entry e : experimental::filesystem::directory_iterator(TOOLS_FOLDER)) {
        experimental::filesystem::path p = e.path();
        if ((p.extension() == ".sh") || (p.extension() == ".bat"))
            this->tools.push_back(p.filename());
    }
}

void PluginManager::runTool(string name) {
    string cmd = string(TOOLS_FOLDER) + "/\"" + name +"\"";
    system(cmd.c_str());
}

vector<string> PluginManager::get_tools() {
    return this->tools;
}
