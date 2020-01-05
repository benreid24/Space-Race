#include <Properties.hpp>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));
    Properties::PrimaryFont.loadFromFile(Properties::FontPath+"PressStart2P.ttf");
    return 0;
}