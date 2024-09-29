#include "poissonDiskSampling.hpp"
#include "boules.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

const int RADIUS_SAMPLING=10;
const int RADIUS_BOULE=5;

int main() {
    // Récupérer la résolution de l'écran
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Créer une fenêtre en plein écran
    sf::RenderWindow window(desktop, "Fenêtre en plein écran", sf::Style::Fullscreen);

    // Déclarer le vecteur boules en dehors de la boucle d'événements
    std::vector<Boules> boules;

    //Nb aléatoire
    std::srand(static_cast<unsigned>(std::time(0)));

    while (window.isOpen()) {
        sf::Event event;
        
        while (window.pollEvent(event)) {
            // Fermer la fenêtre avec la touche Échap ou en cliquant sur le bouton fermer
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            // Détecter un clic de souris
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Récupérer les coordonnées du clic de souris
                    std::cout << "Démarrage !\n";
                    
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;

                    // Utiliser poissonDiskSampling pour générer des points autour du clic
                    auto samples = poissonDiskSampling(desktop.width, desktop.height, RADIUS_SAMPLING, x, y);

                    // Ajouter les nouvelles boules générées
                    for (const auto& point : samples) {
                        int v_x = (std::rand() % 21) - 10;
                        boules.emplace_back(point.first, point.second, v_x, 0, RADIUS_BOULE, sf::Color::Red); // Correction de la couleur
                        window.draw(boules.back().display());
                        window.display();
                        std::this_thread::sleep_for(std::chrono::milliseconds(RADIUS_SAMPLING/2));

                    }
                }
            }
        }

        // Effacer la fenêtre avant de dessiner
        window.clear();

        // Mettre à jour et dessiner les boules
        for (auto& boule : boules) {
            boule.update(boules, desktop.width, desktop.height); // Mise à jour de la position et des collisions
            window.draw(boule.display());  // Dessiner chaque boule
        }

        // Afficher les modifications à l'écran
        window.display();

        // Délai pour limiter la vitesse d'affichage (par exemple, pour éviter de surcharger le processeur)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // Correspond à environ 60 FPS
    }

    return 0;
}
