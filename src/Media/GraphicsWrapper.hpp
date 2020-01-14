#ifndef GRAPHICSWRAPPER_HPP
#define GRAPHICSWRAPPER_HPP

#include <SFML/Graphics.hpp>
#include <Media/Animation.hpp>
#include <Util/ResourceTypes.hpp>
#include <variant>

/**
 * Wrapper class around sf::Sprite and Animation. Handles loading and rendering
 */
class GraphicsWrapper {
public:
    GraphicsWrapper(const std::string& imagePath, const std::string& animPath,
                    const std::string& gfx, bool centerOrigin);

    bool load(const std::string& imagePath, const std::string& animPath,
              const std::string& gfx, bool centerOrigin);

    void setPosition(const sf::Vector2f& pos);
    void setScale(const sf::Vector2f& scale);
    void setRotation(float degrees);

    sf::Vector2f getSize() const;

    void render(sf::RenderTarget& target) const;

private:
    typedef std::variant<TextureReference, AnimationReference> TSrc;
    typedef std::variant<sf::Sprite, Animation>                TGfx;

    TSrc src;
    TGfx gfx;
    bool centerOrigin;
};

#endif