#include <iostream>
#include "Choromosome.hpp"
#include "Piece.hpp"
#include "SpatialRelation.hpp"
#include <random>

Choromosome::Choromosome(int width, int height): choroSize(width, height)
{
    std::vector <int> temp (choroSize.width, -1);
    piecesArrangment = std::vector < std::vector <int> > (choroSize.height, temp);
    availabalePieces = std::vector <bool> (height * width, true);
    fitness = 0;
    occupiedPosition = 0;
}

Choromosome::~Choromosome()
{

}

void Choromosome::generateChoromosome()
{
    int inx = 0;
    for(size_t i=0; i< choroSize.height; i++)
    {
        for(size_t j=0; j< choroSize.width; j++)
            piecesArrangment[i][j] = inx++;
    }

    std::random_shuffle(piecesArrangment.begin(), piecesArrangment.end());
    occupiedPosition = choroSize.height * choroSize.width;
}

void Choromosome::getFreeBoundries(std::queue <SpatialRelation> & freeBounderiesPositions)
{
    for(size_t i=0; i<choroSize.height; i++)
        for(size_t j=0; j<choroSize.width; j++)
        {
            if(piecesArrangment[i][j] != -1)
            {
                int pieceInx = piecesArrangment[i][j];
                int choroRowInx = i;
                int choroColInx = j;

                if(piecesArrangment[i-1][j] == -1 && (i-1) >= 0)
                {
                    freeBounderiesPositions.push(SpatialRelation(SpatialRelation::Up, pieceInx, choroRowInx, choroColInx));
                }

                if (piecesArrangment[i+1][j] == -1 && (i+1) < choroSize.height)
                {
                    freeBounderiesPositions.push(SpatialRelation(SpatialRelation::Down, pieceInx, choroRowInx, choroColInx));
                }

                if (piecesArrangment[i][j-1] == -1 && (j-1) >= 0)
                {
                    freeBounderiesPositions.push(SpatialRelation(SpatialRelation::Left, pieceInx, choroRowInx, choroColInx));
                }

                if (piecesArrangment[i][j+1] == -1 && (j+1) < choroSize.width)
                {
                    freeBounderiesPositions.push(SpatialRelation(SpatialRelation::Right, pieceInx, choroRowInx, choroColInx));
                }
            }
        }
}

void Choromosome::assignPiece(SpatialRelation currentBoundary, int neighbourId)
{
    if(currentBoundary.direction == SpatialRelation::Up)
    {
        if(currentBoundary.choroRowInx == 0)
        {
            shiftDown();
            currentBoundary.choroRowInx ++;
        }
        piecesArrangment[currentBoundary.choroRowInx-1][currentBoundary.choroColInx] = neighbourId;
    }

    else if (currentBoundary.direction == SpatialRelation::Down)
    {
        if(currentBoundary.choroRowInx == (choroSize.height-1))
        {
            shiftUp();
            currentBoundary.choroRowInx --;
        }
        piecesArrangment[currentBoundary.choroRowInx+1][currentBoundary.choroColInx] = neighbourId;
    }

    else if (currentBoundary.direction == SpatialRelation::Left)
    {
        if (currentBoundary.choroColInx == 0)
        {
            shiftRight();
            currentBoundary.choroColInx ++;
        }
        piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx-1] = neighbourId;
    }

    else if (currentBoundary.direction == SpatialRelation::Right)
    {
        if (currentBoundary.choroColInx == (choroSize.width-1))
        {
            shiftLeft();
            currentBoundary.choroColInx --;
        }
        piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx+1] = neighbourId;
    }

    occupiedPosition ++;

    assert(availabalePieces[neighbourId] == false);
    availabalePieces[neighbourId] = false;
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
            fitness += pieces[currentInx].getDownDissimilarityValues(downInx);
        }
}

int Choromosome::getNeighbour(SpatialRelation currentBoundary) const
{
    for(size_t i=0; i< choroSize.height; i++)
        for(size_t j=0; j<choroSize.width; j++)
            if(piecesArrangment[i][j] == currentBoundary.pieceIndex)
            {
                if(currentBoundary.direction == SpatialRelation::Up && (i-1) >= 0)
                    return piecesArrangment[i-1][j];

                else if (currentBoundary.direction == SpatialRelation::Down && (i+1) < choroSize.height)
                    return piecesArrangment[i+1][j];

                else if (currentBoundary.direction == SpatialRelation::Left && (j-1) >= 0)
                    return piecesArrangment[i][j-1];

                else if (currentBoundary.direction == SpatialRelation::Right && (j+1) < choroSize.width)
                    return piecesArrangment[i][j+1];

                else
                    return -1;
            }
}

void Choromosome::checkImageBounderies()
{

}

double Choromosome::getFitness() const
{
    return fitness;
}

int Choromosome::getOccupiedPositions() const
{
    return occupiedPosition;
}

bool Choromosome::checkPieceAvailability(int pieceId) const
{
    return availabalePieces[pieceId];
}

bool Choromosome::operator < (const Choromosome & rhs) const
{
    return this->fitness < rhs.getFitness();
}

void Choromosome::shiftLeft()
{
    for(size_t i=0; i<choroSize.height; i++)
        for(size_t j=1; j<choroSize.width;j++)
            piecesArrangment[i][j-1]= piecesArrangment[i][j];
}

void Choromosome::shiftRight()
{
    for(size_t i=0; i<choroSize.height; i++)
        for(size_t j=choroSize.width-2; j<= 0;j--)
            piecesArrangment[i][j+1]= piecesArrangment[i][j];
}

void Choromosome::shiftUp()
{
    for(size_t i=1; i<choroSize.height; i++)
        for(size_t j=0; j<choroSize.width;j++)
            piecesArrangment[i-1][j]= piecesArrangment[i][j];
}

void Choromosome::shiftDown()
{
    for(size_t i = choroSize.height-2; i<= 0; i--)
        for(size_t j=0; j<choroSize.width;j++)
            piecesArrangment[i+1][j]= piecesArrangment[i][j];
}
