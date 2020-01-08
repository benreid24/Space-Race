#ifndef JSONLOADER_HPP
#define JSONLOADER_HPP

#include <sstream>

/**
 * Utility class to load json from files, streams, and strings
 */
class JsonLoader {
public:
    JsonLoader(const std::string& file);
    JsonLoader(std::istream& stream);

    /**
     * Returns if the input stream is still valid
     */
    bool isValid();

    /**
     * Returns the current line number
     */
    int currentLine() const;

    /**
     * Returns the filename loaded from
     */
    const std::string& getFilename() const;

    /**
     * Skips whitespace and peeks next symbol
     */
    char peekNextSymbol();

    /**
     * Skips the current symbol to load next one
     */
    void skipSymbol();

    /**
     * Loads a quote enclosed string value
     */
    std::string loadString();

    /**
     * Loads the next numeric value
     */
    float loadNumeric();

    /**
     * Helper function to determine if a character is numeric
     */
    bool isNumeric(char c);

    /**
     * Helper function to load a true or false. Returns true on success
     */
    bool loadBool(bool& value);

    /**
     * Stream to output parsing errors to
     */
    std::ostream& error();

private:
    bool valid;
    std::stringstream data;
    std::string filename;
    int cLine;

    void skipWhitespace();

    bool isNumber(char c);
    bool isWhitespace(char c);
};

#endif