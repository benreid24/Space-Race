#include <Util/JsonFile.hpp>

#include <fstream>
#include <Util/JSON/JsonLoader.hpp>

JsonFile::JsonFile(const JsonGroup& root)
: root(root) {}

JsonFile::JsonFile(const std::string& filename) {
    JsonLoader file(filename);
    root = JsonGroup::load(file);
}

void JsonFile::save(const std::string& filename) const {
    std::ofstream file(filename.c_str());
    root.print(file, 4);
}

void JsonFile::print(std::ostream& stream) const {
    root.print(stream, 4);
}

const JsonGroup& JsonFile::getRoot() const {
    return root;
}