#ifndef BOULES_HPP
#define BOULES_HPP

#include <SFML/Graphics.hpp>
#include <vector>

const float GRAVITATIONAL = 9.81f;
const int frameRate = 15;
const float dt = 1.0f / frameRate;

class Boules {
private:
    float x, y;           // Position
    float v_x, v_y;       // Vitesse
    float radius;         // Rayon
    sf::Color color;      // Couleur

public:
    // Constructeur
    Boules(float startX, float startY, float startVX, float startVY, float r, sf::Color c);

    // Méthodes de set
    void setX(float newX);
    void setY(float newY);
    void setVX(float newVX);
    void setVY(float newVY);
    void setRadius(float newRadius);
    void setColor(sf::Color newColor);

    // Méthodes de get
    float getX() const;
    float getY() const;
    float getVX() const;
    float getVY() const;
    float getRadius() const;
    sf::Color getColor() const;

    // Méthode d'affichage
    sf::CircleShape display() const;

    // Mise à jour avec la gestion des collisions
    void update(std::vector<Boules>& boules, int windowWidth, int windowHeight);
};

#endif // BOULES_HPP
