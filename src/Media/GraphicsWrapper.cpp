#include <Media/GraphicsWrapper.hpp>
#include <Util/ResourcePool.hpp> //TODO - make resource pools templated singletons
#include <Util/BinaryFile.hpp> // TODO - make FileUtil
#include <Properties.hpp>

GraphicsWrapper::GraphicsWrapper(const std::string& imagePath, const std::string& animPath,
                                 const std::string& gfx, bool centerOrigin) {
    load(imagePath, animPath, gfx, centerOrigin);
}

bool GraphicsWrapper::load(const std::string& imagePath, const std::string& animPath,
                           const std::string& filename, bool originCentered) {
    centerOrigin = originCentered;
    if (BinaryFile::getExtension(filename) == Properties::AnimationExtension) {
        std::string file = filename;
        if (!BinaryFile::exists(file))
            file = animPath + file;
        if (!BinaryFile::exists(file)) {
            std::cerr << "Failed to load Animation: " << file << std::endl;
            return false;
        }
        src = animPool.loadResource(file);
        Animation anim;
        anim.setSource(*std::get_if<AnimationReference>(&src), centerOrigin);
        gfx = anim;
    }
    else {
        std::string file = filename;
        if (!BinaryFile::exists(file))
            file = imagePath + file;
        if (!BinaryFile::exists(file)) {
            std::cerr << "Failed to load Image: " << file << std::endl;
            return false;
        }
        src = imagePool.loadResource(file);
        sf::Sprite spr;
        spr.setTexture(**std::get_if<TextureReference>(&src), true);
        if (centerOrigin)
            spr.setOrigin(
                spr.getGlobalBounds().width/2,
                spr.getGlobalBounds().height/2
            );
        gfx = spr;
    }
    return true;
}

void GraphicsWrapper::setPosition(const sf::Vector2f& pos) {
    sf::Sprite* spr = std::get_if<sf::Sprite>(&gfx);
    if (spr)
        spr->setPosition(pos);
    else {
        Animation* anim = std::get_if<Animation>(&gfx);
        if (anim)
            anim->setPosition(pos);
        else
            std::cerr << "Invalid GraphicsWrapper\n";
    }
}

void GraphicsWrapper::setRotation(float degrees) {
    sf::Sprite* spr = std::get_if<sf::Sprite>(&gfx);
    if (spr)
        spr->setRotation(degrees);
    else {
        Animation* anim = std::get_if<Animation>(&gfx);
        if (anim)
            anim->setRotation(degrees);
        else
            std::cerr << "Invalid GraphicsWrapper\n";
    }
}

void GraphicsWrapper::setScale(const sf::Vector2f& scale) {
    sf::Sprite* spr = std::get_if<sf::Sprite>(&gfx);
    if (spr) {
        spr->setScale(scale);
        if (centerOrigin)
            spr->setOrigin(
                spr->getGlobalBounds().width/2,
                spr->getGlobalBounds().height/2
            );
    }
    else {
        Animation* anim = std::get_if<Animation>(&gfx);
        if (anim)
            anim->setScale(scale);
        else
            std::cerr << "Invalid GraphicsWrapper\n";
    }
}

sf::Vector2f GraphicsWrapper::getSize() const {
    const sf::Sprite* spr = std::get_if<sf::Sprite>(&gfx);
    if (spr)
        return {
            spr->getGlobalBounds().width,
            spr->getGlobalBounds().height
        };
    else {
        const Animation* anim = std::get_if<Animation>(&gfx);
        if (anim)
            return anim->getSize();
        else {
            std::cerr << "Invalid GraphicsWrapper\n";
            return {0,0};
        }
    }
}

void GraphicsWrapper::render(sf::RenderTarget& target) const {
    const sf::Sprite* spr = std::get_if<sf::Sprite>(&gfx);
    if (spr)
        target.draw(*spr);
    else {
        const Animation* anim = std::get_if<Animation>(&gfx);
        anim->draw(target);
    }
}