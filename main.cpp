#include "poissonDiskSampling.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>


int main() {
    // Récupérer la résolution de l'écran
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Créer une fenêtre en plein écran
    sf::RenderWindow window(desktop, "Fenêtre en plein écran", sf::Style::Fullscreen);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Fermer la fenêtre avec la touche Échap ou en cliquant sur le bouton fermer
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            // Détecter un clic de souris
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Récupérer les coordonnées du clic de souris
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    int radius=50;

                    auto samples = poissonDiskSampling(desktop.width, desktop.height, radius, x, y);
                    
                    for (const auto& point : samples)
                    {
                        sf::CircleShape shape;
                        shape.setRadius(10);
                        shape.setPosition(point.first, point.second);
                        window.draw(shape);
                        window.display();
                        
                        std::this_thread::sleep_for(std::chrono::milliseconds(radius/2));
                    }
                }
            }
        }

    }

    return 0;
}