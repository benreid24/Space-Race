#include <Environment/Backgrounds/BackgroundElementGenerator.hpp>
#include <Util/Timer.hpp>
#include <Util/Util.hpp>
#include <Properties.hpp>
#include <cmath>
#include <iostream>

namespace {
constexpr int   keySkew          = 100000;
constexpr float cleanPeriod      = 10.0;
constexpr int   bucketSize       = 1000;
constexpr int   maxRenderBuckets = 10;
constexpr int   lowRenderInc     = 4;
}

bool BackgroundElementGenerator::BucketKeyCmp::operator()(
        const BackgroundElementGenerator::BucketKey& lhs,
        const BackgroundElementGenerator::BucketKey& rhs) {
    const int skewedLeft = lhs.x * keySkew + lhs.y;
    const int skewedRight = rhs.x * keySkew + rhs.y;
    return skewedLeft < skewedRight;
}

BackgroundElementGenerator::BucketKey::BucketKey(int x, int y)
: x(x), y(y) {}

bool BackgroundElementGenerator::BucketKey::operator==(const BucketKey& key) const {
    return x == key.x && y == key.y;
}

BackgroundElementGenerator::BucketKey::operator sf::FloatRect() const {
    return {
        static_cast<float>(x) * bucketSize,
        static_cast<float>(y) * bucketSize,
        bucketSize,
        bucketSize
    };
}

std::vector<BackgroundElementGenerator::BucketKey> BackgroundElementGenerator::BucketKey::gen(
                                                                    const sf::FloatRect& region) {
    const int x = std::floor(region.left / static_cast<float>(bucketSize)) - 1;
    const int y = std::floor(region.top / static_cast<float>(bucketSize)) - 1;
    const int w = std::ceil(region.width / static_cast<float>(bucketSize)) + 1;
    const int h = std::ceil(region.height / static_cast<float>(bucketSize)) + 1;

    std::vector<BucketKey> keys;
    keys.reserve(w * h);

    for (int cx = x; cx <= x + w; ++cx) {
        for (int cy = y; cy <= y + h; ++cy) {
            keys.push_back(BucketKey(cx, cy));
        }
    }

    return keys;
}

BackgroundElementGenerator::BackgroundElementGenerator(const std::string& file, const sf::Vector2f& minScale, const sf::Vector2f& maxScale)
: gfx(Properties::EnvironmentImagePath, Properties::EnvironmentAnimPath, file, false)
, minScale(minScale), maxScale(maxScale) {
    gfx.setScale(maxScale);
    maxGfxSize = gfx.getSize();
}

void BackgroundElementGenerator::update(const sf::FloatRect& region) {
    const std::vector<BucketKey> keys = BucketKey::gen(region);
    for (unsigned int i = 0; i<keys.size(); ++i) {
        if (buckets.find(keys[i]) == buckets.end())
            buckets[keys[i]] = generate(keys[i]);
    }
    if (Timer::get().timeElapsedSeconds() - lastCleanTime > cleanPeriod) {
        lastCleanTime = Timer::get().timeElapsedSeconds();
        cleanBuckets(keys);
    }
}

void BackgroundElementGenerator::cleanBuckets(const std::vector<BucketKey>& keys) {
    for (auto i = buckets.begin(); i!=buckets.end(); /* noop */) {
        auto j = i++;
        if (std::find(keys.begin(), keys.end(), j->first) == keys.end())
            buckets.erase(j);
    }
}

const sf::Vector2f& BackgroundElementGenerator::getElementSize() const {
    return maxGfxSize;
}

sf::Vector2f BackgroundElementGenerator::getElementScale() const {
    return {
        randomFloat(minScale.x, maxScale.x),
        randomFloat(minScale.y, maxScale.y)
    };
}

void BackgroundElementGenerator::render(sf::RenderTarget& target) {
    const sf::FloatRect region(
        target.getView().getCenter() - target.getView().getSize() / 2.0f,
        target.getView().getSize()
    );
    const std::vector<BucketKey> keys = BucketKey::gen(region);
    if (keys.size() <= maxRenderBuckets) {
        //proper render
        for (unsigned int i = 0; i<keys.size(); ++i) {
            auto bucket = buckets.find(keys[i]);
            if (bucket != buckets.end()) {
                for (unsigned int j = 0; j<bucket->second.size(); ++j) {
                    gfx.setPosition(bucket->second[j].position);
                    gfx.setScale(bucket->second[j].scale);
                    gfx.render(target);
                }
            }
            else
                std::cerr << "Attempted to render bucket that was not generated\n";
        }
    }
    else if (buckets.size() > 0) {
        //low density render of entire region
        std::cout << "low res: " << keys.size() << "\n";
        const ElementBucket& bucket = buckets.begin()->second;
        const sf::FloatRect ogOffset = buckets.begin()->first;
        for (unsigned int i = 0; i<keys.size(); ++i) {
            const sf::FloatRect subregion = keys[i];
            const sf::Vector2f offset(
                subregion.left - ogOffset.left,
                subregion.top - ogOffset.top
            );
            for (unsigned int j = 0; j<bucket.size(); j += lowRenderInc) {
                gfx.setPosition(bucket[i].position + offset);
                gfx.setScale(bucket[i].scale);
                gfx.render(target);
            }
        }
    }
    else
        std::cerr << "Render called on empty BackgroundElementGenerator\n";
}