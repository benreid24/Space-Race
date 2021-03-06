#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

typedef std::shared_ptr<sf::Texture> TextureReference;

/**
 * Data structure to store information about individual frames in an animation
 *
 * \ingroup Media
 */
struct AnimationFrame
{
    sf::Vector2f sourcePos, size, renderOffset;
    unsigned int length;
    int rotation, alpha;
    double scaleX,scaleY;
};

/**
 * This class handles the loading and storage of animation data. This enables the flyweight pattern
 * to be utilized when used in conjunction with the Animation class
 *
 * \ingroup Media
 */
class AnimationSource
{
public:
    /**
     * Creates an empty animation
     */
    AnimationSource();

    /**
     * Loads an animation from the given file
     *
     * \param file The full path of the file to load
     */
    AnimationSource(const std::string& file);

    /**
     * Frees loaded resources
     */
    ~AnimationSource();

    /**
     * Loads an animation from the given file
     *
     * \param file The full path of the file to load
     */
    void load(const std::string& file);

    /**
     * Tells whether or not the animation automatically repeats when it finishes playing
     *
     * \return True if the animation replays by itself
     */
    bool isLooping() const;

    /**
     * Returns a Sprite object that is clipped, oriented and positioned where it needs to be according to the
     * animation data
     *
     * \param i The index of the frame to return
     * \param pos The desired on screen position of the animation
     * \param scale Scale modifier to apply to all pieces
     * \param rotation Rotation angle offset in degrees
     * \param centerOrigin Whether or not to center the origin when offsetting
     * \return A vector of Sprite objects that are ready to be rendered
     */
    const std::vector<sf::Sprite>& getFrame(unsigned int i, const sf::Vector2f& pos,
                                            const sf::Vector2f& scale, float rotation,
                                            bool centerOrigin);

    /**
     * Given the current frame and elapsed time, combined with internal animation data, returns the new frame
     *
     * \param cFrm The current frame index
     * \param lTime The time elapsed since the last update
     * \return The index of the new animation frame that should be rendered
     */
    unsigned int incFrame(unsigned int cFrm, unsigned long lTime);

    /**
     * Tells the total number of frames in the loaded animation
     *
     * \return The total number of frames in the animation
     */
    unsigned int numFrames() const;

    /**
	 * Returns the bounding size of the given frame
	 */
    sf::Vector2f getFrameSize(unsigned int i);

    /**
     * Returns the base filename of the spritesheet
     */
	const std::string& getSpritesheetFilename() const;

	/**
	 * Returns whether or not the spritesheet was found
	 */
    bool spritesheetFound() const;

private:
    TextureReference sheet;
    std::vector<std::vector<AnimationFrame> > frames;
    bool loop;
    std::vector<sf::Sprite> sprites;
    std::string spriteSheetFile;
};

typedef std::shared_ptr<AnimationSource> AnimationReference;

class Animation
{
public:
    /**
     * Creates an empty animation
     */
    Animation();

    /**
     * Sets the source data to the given AnimationSource
     *
     * \param src The AnimationSource to use
     * \param isMapAnim Whether or not this is a map animation. Determines origin
     */
    Animation(AnimationReference src, bool centerOrigin = false);

    /**
     * Frees allotted resources
     */
    ~Animation();

    /**
     * Sets the source of the animation
     *
     * \param src The AnimationSource to use
     * \param centerOrigin True to center the animation at the origin, false for top left
     */
    void setSource(AnimationReference src, bool centerOrigin);

    /**
     * Updates the animation. Sets the proper frame based on elapsed time since the last call to update
     */
    void update() const;

    /**
     * Sets the current frame to the given frame and resets the internal timer
     *
     * \param frm The index of the frame to make current
     */
    void setFrame(unsigned int frm) const;

    /**
     * Tells whether or not the animation has finished playing
     *
     * This function returns true always if the animation is set to loop. Otherwise, it returns true only
     * if play has been called and the animation finished displaying all of its frames
     *
     * \return Whether or not the animation finished
     */
    bool finished() const;

    /**
     * Tells whether or not the animation is set to loop
     *
     * \return Whether or not the animation is set to repeat automatically when finished
     */
    bool isLooping() const;

    /**
     * Sets whether or not the animation should loop
     *
     * \param loop True to loop, false otherwise
     */
    void setLooping(bool loop);

    /**
     * Starts playing the animation. This sets the current frame to 0 and resets the internal timer
     */
    void play();

    /**
     * Tells whether or not the Animation is currently playing
     */
    bool isPlaying() const;

    /**
     * Returns the current frame
     */
    unsigned int getCurrentFrame() const;

    /**
     * Sets the desired position of the animation on screen. Individual frames are offset from this position
     *
     * \param pos The desired on screen position
     */
    void setPosition(sf::Vector2f pos);

    /**
     * Sets the scale to apply to all components of the Animation
     * \param scale The scale to apply
     */
    void setScale(const sf::Vector2f& scale);

    /**
     * Sets the relative rotation of the animation. Individual components' rotations are offset from this
     *
     * \param angle Angle of rotation, consistent with SFML coordinate system
     */
    void setRotation(float angle);

    /**
     * Returns the size of the first frame
     */
	sf::Vector2f getSize() const;

    /**
     * Renders the animation to the given window
     *
     * \param window A pointer to the window to render to
     */
    void draw(sf::RenderTarget& window) const;

private:
    AnimationReference animSrc;
    sf::Vector2f position, scale;
    float rotation;
    bool looping, isCenterOrigin;

    mutable unsigned int curFrm, lastFrmChangeTime;
    mutable bool playing;
};

#endif // ANIMATION_HPP
