#include "boules.hpp"
#include <cmath>

// Constructeur
Boules::Boules(float startX, float startY, float startVX, float startVY, float r, sf::Color c)
    : x(startX), y(startY), v_x(startVX), v_y(startVY), radius(r), color(c) {}

// Méthodes de set
void Boules::setX(float newX) { x = newX; }
void Boules::setY(float newY) { y = newY; }
void Boules::setVX(float newVX) { v_x = newVX; }
void Boules::setVY(float newVY) { v_y = newVY; }
void Boules::setRadius(float newRadius) { radius = newRadius; }
void Boules::setColor(sf::Color newColor) { color = newColor; }

// Méthodes de get
float Boules::getX() const { return x; }
float Boules::getY() const { return y; }
float Boules::getVX() const { return v_x; }
float Boules::getVY() const { return v_y; }
float Boules::getRadius() const { return radius; }
sf::Color Boules::getColor() const { return color; }

// Affichage de la boule
sf::CircleShape Boules::display() const {
    sf::CircleShape shape(radius);
    shape.setPosition(x - radius, y - radius);  // Centrer le cercle
    shape.setFillColor(color);
    return shape;
}

// Mise à jour de la position et gestion des collisions
void Boules::update(std::vector<Boules>& boules, int windowWidth, int windowHeight) {
    // Appliquer la gravité (accélère la vitesse en y)
    v_y += GRAVITATIONAL * dt;

    // Mise à jour de la position en fonction de la vitesse
    x += v_x * dt;
    y += v_y * dt;

    // Gestion des collisions avec les bords de la fenêtre
    if (x - radius < 0) {  // Mur gauche
        x = radius;
        v_x = -v_x;
    }
    if (x + radius > windowWidth) {  // Mur droit
        x = windowWidth - radius;
        v_x = -v_x;
    }
    if (y - radius < 0) {  // Mur haut
        y = radius;
        v_y = -v_y;
    }
    if (y + radius > windowHeight) {  // Mur bas
        y = windowHeight - radius;
        v_y = -v_y;
    }

    // Gestion des collisions avec les autres boules
    for (auto& other : boules) {
        if (&other != this) {
            float distX = other.getX() - x;
            float distY = other.getY() - y;
            float distance = std::sqrt(distX * distX + distY * distY);

            if (distance < radius + other.getRadius()) {
                // Échanger les vitesses en cas de collision
                float tempVX = v_x;
                float tempVY = v_y;
                v_x = other.getVX();
                v_y = other.getVY();
                other.setVX(tempVX);
                other.setVY(tempVY);

                // Repositionner légèrement pour éviter l'interpénétration
                float overlap = radius + other.getRadius() - distance;
                x -= overlap * (distX / distance) * 0.5f;
                y -= overlap * (distY / distance) * 0.5f;
                other.setX(other.getX() + overlap * (distX / distance) * 0.5f);
                other.setY(other.getY() + overlap * (distY / distance) * 0.5f);
            }
        }
    }
}
