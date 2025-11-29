#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <optional>
#include <vector>
#include <iostream>
#include <cmath>
#include <memory>

using namespace std;

class TrafficLight {
private:
    sf::RectangleShape lightPole;
    sf::RectangleShape lightBase;
    sf::RectangleShape lightBox;
    sf::RectangleShape redVisor;
    sf::RectangleShape yellowVisor;
    sf::RectangleShape greenVisor;
    sf::CircleShape redLight;
    sf::CircleShape redLens;
    sf::CircleShape yellowLight;
    sf::CircleShape yellowLens;
    sf::CircleShape greenLight;
    sf::CircleShape greenLens;
    char currentLight = 'G';

    void updateColors() {
        redLight.setFillColor(currentLight == 'R' ? sf::Color(255, 50, 50) : sf::Color::Transparent);
        redLens.setFillColor(currentLight == 'R' ? sf::Color(255, 100, 100, 150) : sf::Color::Transparent);
        yellowLight.setFillColor(currentLight == 'Y' ? sf::Color(255, 255, 50) : sf::Color::Transparent);
        yellowLens.setFillColor(currentLight == 'Y' ? sf::Color(255, 255, 100, 150) : sf::Color::Transparent);
        greenLight.setFillColor(currentLight == 'G' ? sf::Color(50, 255, 50) : sf::Color::Transparent);
        greenLens.setFillColor(currentLight == 'G' ? sf::Color(100, 255, 100, 150) : sf::Color::Transparent);
    }

public:
    TrafficLight() {
        lightPole = sf::RectangleShape({16.f, 225.f});
        lightPole.setFillColor(sf::Color(65, 65, 65));
        lightPole.setPosition(sf::Vector2f(750.f, 275.f));

        lightBase = sf::RectangleShape({82.f, 28.f});
        lightBase.setFillColor(sf::Color(45, 45, 45));
        lightBase.setPosition(sf::Vector2f(717.f, 492.f));

        lightBox = sf::RectangleShape({72.f, 255.f});
        lightBox.setFillColor(sf::Color(25, 25, 25));
        lightBox.setPosition(sf::Vector2f(715.f, 100.f));
        lightBox.setOutlineColor(sf::Color(100, 100, 100));
        lightBox.setOutlineThickness(3.f);

        redVisor = sf::RectangleShape({72.f, 14.f});
        redVisor.setFillColor(sf::Color(25, 25, 25));
        redVisor.setPosition(sf::Vector2f(714.f, 105.f));

        yellowVisor = sf::RectangleShape({72.f, 14.f});
        yellowVisor.setFillColor(sf::Color(25, 25, 25));
        yellowVisor.setPosition(sf::Vector2f(714.f, 185.f));

        greenVisor = sf::RectangleShape({72.f, 14.f});
        greenVisor.setFillColor(sf::Color(25, 25, 25));
        greenVisor.setPosition(sf::Vector2f(714.f, 265.f));

        redLight = sf::CircleShape(29.f);
        redLight.setOutlineColor(sf::Color(180, 0, 0));
        redLight.setOutlineThickness(4.f);
        redLight.setPosition(sf::Vector2f(724.f, 122.f));

        redLens = sf::CircleShape(23.f);
        redLens.setPosition(sf::Vector2f(730.f, 128.f));

        yellowLight = sf::CircleShape(29.f);
        yellowLight.setOutlineColor(sf::Color(180, 180, 0));
        yellowLight.setOutlineThickness(4.f);
        yellowLight.setPosition(sf::Vector2f(724.f, 202.f));

        yellowLens = sf::CircleShape(23.f);
        yellowLens.setPosition(sf::Vector2f(730.f, 208.f));

        greenLight = sf::CircleShape(29.f);
        greenLight.setOutlineColor(sf::Color(0, 120, 0));
        greenLight.setOutlineThickness(4.f);
        greenLight.setPosition(sf::Vector2f(724.f, 282.f));

        greenLens = sf::CircleShape(23.f);
        greenLens.setPosition(sf::Vector2f(730.f, 288.f));

        updateColors();
    }

    void setColor(char color) {
        currentLight = color;
        updateColors();
    }

    char getColor() const {
        return currentLight;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(lightPole);
        window.draw(lightBase);
        window.draw(lightBox);
        window.draw(redVisor);
        window.draw(yellowVisor);
        window.draw(greenVisor);
        window.draw(redLight);
        window.draw(redLens);
        window.draw(yellowLight);
        window.draw(yellowLens);
        window.draw(greenLight);
        window.draw(greenLens);
    }
};

class Road {
private:
    sf::RectangleShape roadShape;
    vector<sf::RectangleShape> laneDashes;
    vector<sf::RectangleShape> zebraStripes;

public:
    Road() {
        roadShape = sf::RectangleShape({800.f, 110.f});
        roadShape.setFillColor(sf::Color(90, 90, 90));
        roadShape.setPosition(sf::Vector2f(0.f, 395.f));

        for (int i = 0; i < 16; ++i) {
            sf::RectangleShape dash({35.f, 6.f});
            dash.setFillColor(sf::Color::White);
            dash.setPosition(sf::Vector2f(float(i * 50 + 10), 447.f));
            laneDashes.push_back(dash);
        }

        for (int i = 0; i < 6; ++i) {
            sf::RectangleShape stripe({8.f, 110.f});
            stripe.setFillColor(sf::Color::White);
            stripe.setPosition(sf::Vector2f(710.f + float(i * 12), 395.f));
            zebraStripes.push_back(stripe);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(roadShape);
        for (const auto& d : laneDashes) window.draw(d);
        for (const auto& s : zebraStripes) window.draw(s);
    }
};

class Car {
private:
    sf::Texture carTexture;
    unique_ptr<sf::Sprite> sprite;
    sf::SoundBuffer engineBuffer;
    unique_ptr<sf::Sound> engineSound;
    float carX;
    float stopPos;
    float currentSpeed = 0.f;
    bool hasPassedLight = false;

public:
    Car(float stop) : carX(-150.f), stopPos(stop) {
        if (carTexture.loadFromFile("file/car.png")) {
            sprite = make_unique<sf::Sprite>(carTexture);
            sprite->setScale({0.3f, 0.4f});
            sprite->setOrigin(sf::Vector2f(carTexture.getSize()) * 0.5f);
        } else {
            cerr << "Error: car.png not found!\n";
        }

        if (engineBuffer.loadFromFile("file/engine.wav")) {
            engineSound = make_unique<sf::Sound>(engineBuffer);
            engineSound->setLooping(true);
        } else {
            cerr << "Warning: engine.wav not found!\n";
        }
    }

    void update(float dt, char light) {
        currentSpeed = 0.f;
        if (light == 'G') {
            currentSpeed = 280.f;
        } else if (light == 'Y') {
            currentSpeed = 130.f;
        } else if (light == 'R') {
            if (!hasPassedLight && carX < stopPos) {
                currentSpeed = 130.f; // Approach slowly if not passed
            } else if (hasPassedLight) {
                currentSpeed = 130.f; // Continue if already passed
            }
        }

        float newX = carX + currentSpeed * dt;
        if (light == 'R' && !hasPassedLight && newX > stopPos) {
            newX = stopPos;
            currentSpeed = 0.f;
        }
        carX = newX;

        // Update hasPassedLight
        if (carX > stopPos + 50.f) { // Buffer to detect passing
            hasPassedLight = true;
        }
        if (carX < 0.f) { // When looping back
            hasPassedLight = false;
        }

        if (carX > 800.f + 150.f) {
            carX = -150.f;
            hasPassedLight = false;
        }

        if (sprite) {
            sprite->setPosition({carX + 70.f, 460.f});
        }

        bool isMoving = currentSpeed > 0.f;
        if (engineSound) {
            if (isMoving && engineSound->getStatus() != sf::Sound::Status::Playing) {
                engineSound->play();
            } else if (!isMoving && engineSound->getStatus() == sf::Sound::Status::Playing) {
                engineSound->stop();
            }
            engineSound->setPitch(light == 'G' ? 1.0f : 0.7f);
        }
    }

    string getStatus() const {
        if (currentSpeed > 200.f) {
            return "Car is moving fast!";
        } else if (currentSpeed > 0.f) {
            return "Car is moving slowly...";
        } else {
            return "Car stopped at red light.";
        }
    }

    void draw(sf::RenderWindow& window) {
        if (sprite) {
            window.draw(*sprite);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Traffic Control Bangladesh");
    window.setFramerateLimit(120);

    sf::Font font;
    if (!font.openFromFile("file/arial.ttf")) {

    }

    sf::Text header(font, "TRAFFIC CONTROL BANGLADESH", 35);
    header.setFillColor(sf::Color::White);
    header.setPosition(sf::Vector2f(160.f, 15.f));

    sf::Text status(font, "", 22);
    status.setFillColor(sf::Color::White);
    status.setPosition(sf::Vector2f(50.f, 550.f));

    sf::Text controls(font, "Controls: [G]reen | [Y]ellow | [R]ed", 20);
    controls.setFillColor(sf::Color::White);
    controls.setPosition(sf::Vector2f(50.f, 520.f));

    TrafficLight trafficLight;
    Road road;
    const float stopPos = 550.f;
    Car car(stopPos);

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            if (!eventOpt) continue;
            if (eventOpt->is<sf::Event::Closed>()) {
                window.close();
            } else if (auto* keyPressed = eventOpt->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::G) trafficLight.setColor('G');
                else if (keyPressed->code == sf::Keyboard::Key::Y) trafficLight.setColor('Y');
                else if (keyPressed->code == sf::Keyboard::Key::R) trafficLight.setColor('R');
            }
        }

        car.update(dt, trafficLight.getColor());
        status.setString(car.getStatus());

        window.clear(sf::Color(20, 20, 40));
        window.draw(header);
        window.draw(controls);
        window.draw(status);
        trafficLight.draw(window);
        road.draw(window);
        car.draw(window);
        window.display();
    }

    return 0;
}
