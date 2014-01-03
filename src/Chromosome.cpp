#include <iostream>
#include "Choromosome.hpp"
#include "Piece.hpp"
#include <random>

Choromosome::Choromosome(int width, int height): choroSize(width, height)
{
    piecesArrangment.reserve(choroSize.height);
    fitness = 0;
}

Choromosome::~Choromosome()
{

}

void Choromosome::generateChoromosome()
{
    int inx = 0;
    for(size_t i=0; i< choroSize.height; i++)
    {
        std::vector <int> row;
        for(size_t j=0; j< choroSize.width; j++)
            row.push_back(inx++);
        piecesArrangment.push_back(row);
    }
    std::random_shuffle(piecesArrangment.begin(), piecesArrangment.end());
}

void Choromosome::foundBounderies()
{

}

void Choromosome::assignPiece(int index)
{

}

void Choromosome::calculateFitness(const std::vector<Piece> &pieces)
{
    fitness = 0;
    for(size_t i=0; i<choroSize.height-1; i++)
        for(size_t j=0; j< choroSize.width-1; j++)
        {
            int currentInx = piecesArrangment[i][j];
            int rightInx = piecesArrangment[i][j+1];
            int downInx = piecesArrangment[i+1][j];
            fitness += pieces[currentInx].getRightDissimilarityValue(rightInx);
            fitness += pieces[currentInx].getUpDissimilarityValues(downInx);
        }
}

void Choromosome::checkImageBounderies()
{

}

double Choromosome::getFitness() const
{
    return fitness;
}

bool Choromosome::operator < (const Choromosome & rhs) const
{
    return this->fitness < rhs.getFitness();
}
