#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/**
 * This class publicly stores all of the static properties of the game, such as file paths and window resolutions
 */
class Properties
{
public:
    static constexpr float GravitationalConstant = 1500;
    static constexpr float StdToSFMLRotationOffset = 90;

    static const int ScreenWidth = 1920;
    static const int ScreenHeight = 1080;

    static const std::string FontPath;

    static const std::string GameSavePath;

    static const std::string EnvironmentFilePath;
    
    static const std::string EntityAnimationPath;
    static const std::string SpriteSheetPath;

    static const std::string ScriptPath;
    static const std::string ScriptExtension;

    static const std::string PlaylistPath;
    static const std::string MusicPath;
    static const std::string AudioPath;

    static sf::Font PrimaryFont;
};

#endif // PROPERTIES_HPP
