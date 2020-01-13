#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <Entities/Entity.hpp>
#include <Environment/Background.hpp>

/**
 * Represents a playable level and all entities within
 */
class Environment {
public:
    enum PlayerStatus {
        Playing,
        Won,
        Dead,
        Stranded
    };
    
    /**
     * Creates the default test environment
     */
    Environment();

    /**
     * Loads the Environment from the file
     */
    Environment(const std::string& file);

    /**
     * Updates the environment and all entities within
     */
    void update(float dt);

    /**
     * Renders to the target
     */
    void render(sf::RenderTarget& target);

    /**
     * Returns the PlayerStatus of the environment
     */
    PlayerStatus getPlayerStatus() const;

private:
    sf::View camera;

    std::string name;
    sf::FloatRect victoryRegion;
    sf::FloatRect bounds;

    Background background;

    std::vector<Entity::Ptr> entities;
    Entity::Ptr player;
};

#endif