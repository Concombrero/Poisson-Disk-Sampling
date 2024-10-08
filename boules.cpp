#include "boules.hpp"
#include <cmath>

// Constructeur
Boules::Boules(float startX, float startY, float startVX, float startVY, float r, sf::Color c)
    : x(startX), y(startY), v_x(startVX), v_y(startVY), radius(r), color(c){
    }

// Méthodes de set
void Boules::setX(float newX) { x = newX; xDraw=newX+radius;}
void Boules::setY(float newY) { y = newY; yDraw=newY+radius;}
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
        v_x = -v_x * RESTITUTION;
    }
    if (x + radius > windowWidth) {  // Mur droit
        x = windowWidth - radius;
        v_x = -v_x * RESTITUTION;
    }
    if (y - radius < 0) {  // Mur haut
        y = radius;
        v_y = -v_y * RESTITUTION;
    }
    if (y + radius> windowHeight) {  // Mur bas
        y = windowHeight - radius;
        v_y = -v_y * RESTITUTION;
    }


    //Collision
    for (auto& other : boules) {
        if (&other != this) {
            // Mettre en place les variables distance
            float distX = x - other.getX();
            float distY = y - other.getY();
            float distance = std::sqrt(distX * distX + distY * distY);

            if (distance < radius + other.getRadius()) {
                // Mise en place de n
                sf::Vector2f n;
                n.x = distX;
                n.y = distY;
                float norme = std::sqrt(n.x * n.x + n.y * n.y);
                
                if (norme != 0) {
                    n.x /= norme;
                    n.y /= norme;
                }

                // Composantes de vitesse des deux boules avant collision
                sf::Vector2f v1(v_x, v_y);
                sf::Vector2f v2(other.getVX(), other.getVY());

                sf::Vector2f dv = v1 - v2;

                float dotProduct = dv.x * n.x + dv.y * n.y;

                float impactVelocity = dotProduct * RESTITUTION;
                v_x -= n.x * impactVelocity;
                v_y -= n.y * impactVelocity;
                other.setVX(other.getVX() + n.x * impactVelocity);
                other.setVY(other.getVY() + n.y * impactVelocity);


                // Repositionner légèrement pour éviter l'interpénétration
                float overlap = radius + other.getRadius() - distance-8;
                x -= overlap * (distX / distance) * 0.5f;
                y -= overlap * (distY / distance) * 0.5f;
                other.setX(other.getX() + overlap * (distX / distance) * 0.5f);
                other.setY(other.getY() + overlap * (distY / distance) * 0.5f);
            }
        }
    }
    
    
    //Frotement
    if (y+radius>windowHeight-TOL_FROTEMENT)
    {
        v_x*=FROTEMENT;        
    }
    
}
