#ifndef JSONFILE_HPP
#define JSONFILE_HPP

#include <Util/JSON/JsonTypes.hpp>
#include <ostream>

/**
 * Utility class to load and save json files. Can optionally verify a schema
 */
class JsonFile {
public:
    /**
     * Creates a file from the root data
     */
    JsonFile(const JsonGroup& root);

    /**
     * Loads the json data from the given file
     */
    JsonFile(const std::string& file);

    /**
     * Saves the json data to the given file
     */
    void save(const std::string& file) const;

    /**
     * Prints the json to the given stream
     */
    void print(std::ostream& stream) const;

    /**
     * Returns the root data object
     */
    const JsonGroup& getRoot() const;

private:
    JsonGroup root;
};

#endif