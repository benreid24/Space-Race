#include <Environment/Environment.hpp>
#include <Util/Timer.hpp>
#include <Util/Util.hpp>
#include <Properties.hpp>

#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));
    Properties::PrimaryFont.loadFromFile(Properties::FontPath+"PressStart2P.ttf");

    Environment environment("test.json");
    sf::RenderWindow window(
        sf::VideoMode(Properties::ScreenWidth, Properties::ScreenHeight, 32),
        "Space Race",
        sf::Style::Close | sf::Style::Titlebar
    );

    const float renderTimeGap = 0.015; // ~60 fps
    const float minPhysicsTime = 0.002; // 500 fps
    float lastPhysicsTime = Timer::get().timeElapsedSeconds();
    float lastRenderTime = Timer::get().timeElapsedSeconds();

    float fps = 60;
    sf::Text fpsText;
    fpsText.setFont(Properties::PrimaryFont);
    fpsText.setFillColor(sf::Color::Red);
    fpsText.setCharacterSize(18);
    fpsText.setOutlineColor(sf::Color::White);
    fpsText.setOutlineThickness(1);
    fpsText.setPosition({10,10});

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
        }

        environment.update((Timer::get().timeElapsedSeconds() - lastPhysicsTime)/2);

        if (Timer::get().timeElapsedSeconds() - lastRenderTime >= renderTimeGap) {
            environment.render(window);

            fps = 0.9 * fps + 0.1 / (Timer::get().timeElapsedSeconds() - lastRenderTime);
            fpsText.setString("FPS: " + intToString(fps));
            window.setView(window.getDefaultView());
            window.draw(fpsText);
            
            window.display();
            lastRenderTime = Timer::get().timeElapsedSeconds();
        }

        const float refTime = std::max(lastRenderTime, lastPhysicsTime);
        const float sleepTime = Timer::get().timeElapsedSeconds() - refTime;
        lastPhysicsTime = Timer::get().timeElapsedSeconds();
        if (minPhysicsTime - sleepTime > 0)
            sf::sleep(sf::seconds(minPhysicsTime - sleepTime));
    }

    return 0;
}