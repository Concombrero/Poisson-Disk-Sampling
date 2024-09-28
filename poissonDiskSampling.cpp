#include "poissonDiskSampling.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>  // Pour std::find

std::vector<std::vector<std::pair<double, double>>> creerGrille(int x, int y) {
    return std::vector<std::vector<std::pair<double, double>>>(y, std::vector<std::pair<double, double>>(x, {0, 0}));
}

std::pair<int, int> getIndice(double x, double y, double cellSize) {
    return {static_cast<int>(x / cellSize), static_cast<int>(y / cellSize)};
}

std::pair<double, double> createNewPoint(double x, double y, double radius) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distRadius(radius, 2 * radius);
    std::uniform_real_distribution<> distAngle(0, 2 * M_PI);

    double distance = distRadius(gen);
    double angle = distAngle(gen);
    double d_x = std::cos(angle) * distance;
    double d_y = std::sin(angle) * distance;

    return {x + d_x, y + d_y};
}

void insertPoint(std::vector<std::vector<std::pair<double, double>>> &grid, int xIndex, int yIndex, double x, double y) {
    grid[yIndex][xIndex] = {x, y};
}

bool verifyPoint(const std::vector<std::vector<std::pair<double, double>>> &grid, double x, double y, double width, double height, double cellSize, double radius) {
    auto [i, j] = getIndice(x, y, cellSize);

    if (!(radius <= x && x <= width - radius && radius <= y && y <= height - radius)) {
        return false;
    }

    if (grid[j][i].first != 0 || grid[j][i].second != 0) {
        return false;
    }

    for (int dj = -2; dj <= 2; ++dj) {
        for (int di = -2; di <= 2; ++di) {
            int ni = i + di, nj = j + dj;
            if (ni >= 0 && ni < grid[0].size() && nj >= 0 && nj < grid.size() && (grid[nj][ni].first != 0 || grid[nj][ni].second != 0)) {
                double neighbor_x = grid[nj][ni].first;
                double neighbor_y = grid[nj][ni].second;
                double dist_sq = std::pow(neighbor_x - x, 2) + std::pow(neighbor_y - y, 2);
                if (dist_sq < std::pow(radius, 2)) {
                    return false;
                }
            }
        }
    }

    insertPoint(const_cast<std::vector<std::vector<std::pair<double, double>>> &>(grid), i, j, x, y);
    return true;
}

// Fonction pour comparer deux paires (points)
bool comparePoints(const std::pair<double, double>& a, const std::pair<double, double>& b) {
    return (std::abs(a.first - b.first) < 1e-6) && (std::abs(a.second - b.second) < 1e-6);
}

std::vector<std::pair<double, double>> poissonDiskSampling(double width, double height, double radius, int x_0, int y_0, int k) {
    double cellSize = radius / std::sqrt(DIMENSION);
    int nb_col = static_cast<int>(width / cellSize);
    int nb_row = static_cast<int>(height / cellSize);
    auto grille = creerGrille(nb_col, nb_row);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distX(radius, width - radius);
    std::uniform_real_distribution<> distY(radius, height - radius);

    auto [i, j] = getIndice(x_0, y_0, cellSize);
    insertPoint(grille, i, j, x_0, y_0);

    std::vector<std::pair<double, double>> samples = {{x_0, y_0}};
    std::vector<std::pair<double, double>> activeList = {{x_0, y_0}};

    while (!activeList.empty()) {
        std::uniform_int_distribution<> distActive(0, activeList.size() - 1);
        auto [x, y] = activeList[distActive(gen)];
        bool point_added = false;

        for (int attempt = 0; attempt < k; ++attempt) {
            auto [x_temp, y_temp] = createNewPoint(x, y, radius);
            if (verifyPoint(grille, x_temp, y_temp, width, height, cellSize, radius)) {
                activeList.push_back({x_temp, y_temp});
                samples.push_back({x_temp, y_temp});
                point_added = true;
                break;
            }
        }

        if (!point_added) {
            // Recherche manuelle dans activeList pour trouver et supprimer le point (x, y)
            auto it = std::find_if(activeList.begin(), activeList.end(), [&](const std::pair<double, double>& p) {
                return comparePoints(p, {x, y});
            });
            if (it != activeList.end()) {
                activeList.erase(it);
            }
        }
    }

    return samples;
}