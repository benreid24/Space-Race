#ifndef BACKGROUNDELEMENTGENERATOR_HPP
#define BACKGROUNDELEMENTGENERATOR_HPP

#include <Media/GraphicsWrapper.hpp>
#include <memory>

/**
 * Base generator class for background elements
 */
class BackgroundElementGenerator {
public:
    typedef std::shared_ptr<BackgroundElementGenerator> Ptr;

    void update(const sf::FloatRect& region);

    void render(sf::RenderTarget& target);

protected:
    struct Element {
        const sf::Vector2f position;
        const sf::Vector2f scale;
        Element(const sf::Vector2f& pos, const sf::Vector2f& scale) : position(pos), scale(scale) {}
    };
    typedef std::vector<Element> ElementBucket;

    BackgroundElementGenerator(const std::string& file,
        const sf::Vector2f& minScale, const sf::Vector2f& maxScale);

    /**
     * Custom generators supply their logic here
     */
    virtual ElementBucket generate(const sf::FloatRect& region) = 0;

    sf::Vector2f getElementScale() const;
    const sf::Vector2f& getElementSize() const;

private:
    struct BucketKey {
        const int x;
        const int y;

        static std::vector<BucketKey> gen(const sf::FloatRect& region);

        BucketKey(int x, int y);
        bool operator==(const BucketKey& key) const;
        operator sf::FloatRect() const;
    };
    struct BucketKeyCmp {
        bool operator()(const BucketKey& lhs, const BucketKey& rhs);
    };

    float lastCleanTime;
    std::map<BucketKey, ElementBucket, BucketKeyCmp> buckets;

    GraphicsWrapper gfx;
    const sf::Vector2f minScale;
    const sf::Vector2f maxScale;
    sf::Vector2f maxGfxSize;

    void cleanBuckets(const std::vector<BucketKey>& keys);
};

#endif