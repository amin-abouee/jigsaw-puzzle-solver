#ifndef CHOROMOSOME_HPP
#define CHOROMOSOME_HPP

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include "Piece.hpp"

class Choromosome
{
public:

    //C'tor
    Choromosome(int width, int height);

    //D'tor
    virtual ~Choromosome();

    void foundBounderies();

    void assignPiece(int index);

    void calculateFitness(const std::vector<Piece>& pieces);

    void checkImageBounderies();

    void generateChoromosome();

    bool operator < (const Choromosome & rhs) const;

    double getFitness() const;

private:

    std::vector < std::vector <int> > piecesArrangment;

    cv::Size choroSize;

    double fitness;

};

#endif /* CHOROMOSOME_HPP */
