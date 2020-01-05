#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <Entities/Entity.hpp>
#include <Util/File.hpp>

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
     * Loads the Environment from the File
     */
    Environment(File& file);

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
    sf::Color background;
    std::vector<Entity::Ptr> entities;
    Entity::Ptr player;
    sf::FloatRect victoryRegion;
};

#endif