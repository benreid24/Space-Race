#include <Util/JSON/JsonLoader.hpp>

#include <iostream>
#include <fstream>

JsonLoader::JsonLoader(const std::string& file)
: valid(true), filename(file), cLine(1) {
    std::ifstream input(file.c_str());
    std::string buffer;

    input.seekg(0, std::ios::end);
    buffer.reserve(input.tellg());
    input.seekg(0, std::ios::beg);
    buffer.assign((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    data.str(buffer);
    skipWhitespace();
}

JsonLoader::JsonLoader(std::istream& input) : valid(true), cLine(1) {
    std::string buffer;

    input.seekg(0, std::ios::end);
    buffer.reserve(input.tellg());
    input.seekg(0, std::ios::beg);
    buffer.assign((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    data.str(buffer);
    skipWhitespace();
}

int JsonLoader::currentLine() const {
    return cLine;
}

const std::string& JsonLoader::getFilename() const {
    return filename;
}

std::ostream& JsonLoader::error() {
    valid = false;
    std::cerr << "Error: file " << filename << " line " << cLine << ": ";
    return std::cerr;
}

bool JsonLoader::isValid() {
    return valid && data.good();
}

char JsonLoader::peekNextSymbol() {
    return data.peek();
}

void JsonLoader::skipSymbol() {
    if (isValid()) {
        data.get();
        skipWhitespace();
    }
}

std::string JsonLoader::loadString() {
    if (isValid()) {
        if (peekNextSymbol() == '"') {
            std::string ret;
            data.get();
            while (data.peek() != '"') {
                if (data.peek() == '\n')
                    cLine += 1;

                ret.push_back(data.get());
                if (!data.good()) {
                    valid = false;
                    std::cerr << "Unexpected end of file" << std::endl;
                    return "";
                }
            }
            skipSymbol(); // closing quote
            return ret;
        }
        error() << "Unxpected symbol '" << peekNextSymbol() << "' expecting '\"'" << std::endl;
        valid = false;
    }
}

bool JsonLoader::loadBool(bool& value) {
    if (peekNextSymbol() == 't' || peekNextSymbol() == 'f') {
        std::string word;
        while (!isWhitespace(data.peek()) && data.good()) {
            word.push_back(data.get());
            if (word == "true") {
                value = true;
                skipWhitespace();
                return true;
            }
            if (word == "false") {
                value = false;
                skipWhitespace();
                return true;
            }
            if (word.size() > 5) {
                error() << "Expected boolean value but too many characters" << std::endl;
                valid = false;
                return false;
            }
        }
        if (data.good()) {
            error() << "'" << word << "' is not a boolean value" << std::endl;
            valid = false;
            return false;
        }
    }
    else {
        valid = false;
        error() << "Unexpected character '" << peekNextSymbol() << "'" << std::endl;
        return false;
    }
    valid = false;
    error() << "Unexpected end of file while parsing boolean" << std::endl;
    return false;
}

bool JsonLoader::isNumeric(char c) {
    return isNumber(c) || c == '-';
}

bool JsonLoader::isNumber(char c) {
    return c >= '0' && c <= '9';
}

double JsonLoader::loadNumeric() {
    if (isValid()) {
        const char c = peekNextSymbol();
        if (c == '-' || (c >= '0' && c <= '9')) {
            std::string num;
            num.push_back(data.get());
            bool decimal = false;

            while (isNumber(data.peek()) || data.peek() == '.') {
                if (data.peek() == '.' && decimal) {
                    error() << "Too many decimal points in number" << std::endl;
                    return 0;
                }
                else if (data.peek() == '.')
                    decimal = true;

                num.push_back(data.get());
                if (!data.good()) {
                    error() << "Unexpected end of file";
                    valid = false;
                    return 0;
                }
            }
            skipWhitespace();
            return std::stod(num);
        }
        else {
            error() << "Invalid numeric symbol " << c << std::endl;
            valid = false;
        }
    }
    return 0;
}

bool JsonLoader::isWhitespace(char c) {
    return c==' ' || c=='\n' || c=='\r';
}

void JsonLoader::skipWhitespace() {
    while (data.good() && isWhitespace(data.peek())) {
        if (data.peek() == '\n')
            cLine += 1;
        data.get();
    }
}