// poisson_disk_sampling.hpp
#ifndef POISSONDISKSAMPLING_HPP
#define POISSONDISKSAMPLING_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

// Définition de la constante globale
const int DIMENSION = 2;

// Déclarations des fonctions
std::vector<std::vector<std::pair<double, double>>> creerGrille(int x, int y);

std::pair<int, int> getIndice(double x, double y, double cellSize);

std::pair<double, double> createNewPoint(double x, double y, double radius);

void insertPoint(std::vector<std::vector<std::pair<double, double>>> &grid, int xIndex, int yIndex, double x, double y);

bool verifyPoint(const std::vector<std::vector<std::pair<double, double>>> &grid, double x, double y, double width, double height, double cellSize, double radius);

bool comparePoints(const std::pair<double, double>& a, const std::pair<double, double>& b);

std::vector<std::pair<double, double>> poissonDiskSampling(double width, double height, double radius, int x_0, int y_0, int k = 30);

#endif // POISSON_DISK_SAMPLING_HPP
