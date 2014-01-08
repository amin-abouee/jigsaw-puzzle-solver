#include <iostream>
#include "Choromosome.hpp"
#include "Piece.hpp"
#include "SpatialRelation.hpp"
#include <random>

// check
Choromosome::Choromosome(int width, int height): choroSize(width, height)
{
    // check this place
    std::vector <int> temp (choroSize.width, -1);
    piecesArrangment = std::vector < std::vector <int> > (choroSize.height, temp);
    availabalePieces = std::vector <bool> (height * width, true);
    fitness = 0;
    occupiedPosition = 0;
    lockHorizontalShift = false;
    lockVerticallShift = false;
//    numPicesInRows = std::vector <int> (height,0);
//    numPicesInCols = std::vector <int> (width,0);
}

Choromosome::~Choromosome()
{

}

// check
void Choromosome::generateChoromosome(std::vector <int> & randVec)
{
    std::random_shuffle(randVec.begin(), randVec.end());

    int inx = 0;
    for(size_t i=0; i< choroSize.height; i++)
    {
        for(size_t j=0; j< choroSize.width; j++)
        {
            availabalePieces[inx] = false;
            piecesArrangment[i][j] = randVec[inx++];
//            numPicesInCols[j]++;
//            numPicesInRows[i]++;
        }
    }

//    std::random_shuffle(piecesArrangment.begin(), piecesArrangment.end());
    occupiedPosition = choroSize.height * choroSize.width;
}

void Choromosome::getFreeBoundries(std::vector <SpatialRelation> & freeBounderiesPositions)
{
    for(int i=0; i<choroSize.height; i++)
        for(int j=0; j<choroSize.width; j++)
        {
            if(piecesArrangment[i][j] != -1)
            {
                int pieceInx = piecesArrangment[i][j];
                int choroRowInx = i;
                int choroColInx = j;

                if(i-1 >= 0 && piecesArrangment[i-1][j] == -1 )
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Up, pieceInx, choroRowInx, choroColInx));
                }
                else if(i-1 < 0 && !lockVerticallShift)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Up, pieceInx, choroRowInx, choroColInx));
                }

                if ((i+1) < choroSize.height && piecesArrangment[i+1][j] == -1)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Down, pieceInx, choroRowInx, choroColInx));
                }
                else if ((i+1) >= choroSize.height && !lockVerticallShift)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Down, pieceInx, choroRowInx, choroColInx));
                }

                if ((j-1) >= 0 && piecesArrangment[i][j-1] == -1)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Left, pieceInx, choroRowInx, choroColInx));
                }
                else if (j-1 < 0 && !lockHorizontalShift)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Left, pieceInx, choroRowInx, choroColInx));
                }

                if ((j+1) < choroSize.width && piecesArrangment[i][j+1] == -1)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Right, pieceInx, choroRowInx, choroColInx));
                }
                else if ((j+1) >= choroSize.width && !lockHorizontalShift)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Right, pieceInx, choroRowInx, choroColInx));
                }
            }
        }
}

// check
void Choromosome::assignPiece(int rowInx, int colInx, int seedInx)
{
    piecesArrangment[rowInx][colInx] = seedInx;
    occupiedPosition ++;

//    numPicesInCols[colInx]++;
//    numPicesInRows[rowInx]++;
//    int avalSize = availabalePieces.size();

//    for(int i=0; i<availabalePieces.size();i++)
//        std::cout << (availabalePieces[i] ? "yes" : "no") << std::endl;

    assert(availabalePieces[seedInx] == true);
    availabalePieces[seedInx] = false;
}

void Choromosome::checkImageBounderies(SpatialRelation currentBoundary)
{
    if(!lockHorizontalShift)
    {
        if(currentBoundary.direction == SpatialRelation::Right)
        {
            if (piecesArrangment[currentBoundary.choroRowInx][0] != -1)
                lockHorizontalShift = true;
        }

        else if(currentBoundary.direction == SpatialRelation::Left)
        {
            if (piecesArrangment[currentBoundary.choroRowInx][choroSize.height-1] != -1)
                lockHorizontalShift = true;
        }
    }

    if(!lockVerticallShift)
    {
        if(currentBoundary.direction == SpatialRelation::Down)
        {
            if (piecesArrangment[0][currentBoundary.choroColInx] != -1)
                lockVerticallShift = true;
        }

        else if(currentBoundary.direction == SpatialRelation::Up)
        {
            if (piecesArrangment[choroSize.width-1][currentBoundary.choroColInx] != -1)
                lockVerticallShift = true;
        }
    }
}

void Choromosome::assignPiece(SpatialRelation currentBoundary, int neighbourId)
{
    checkImageBounderies(currentBoundary);

    if(currentBoundary.direction == SpatialRelation::Up)
    {
        if(currentBoundary.choroRowInx == 0 && !lockVerticallShift)
        {
            shiftDown();
            currentBoundary.choroRowInx ++;
            piecesArrangment[currentBoundary.choroRowInx-1][currentBoundary.choroColInx] = neighbourId;
        }
        else if (currentBoundary.choroRowInx != 0)
            piecesArrangment[currentBoundary.choroRowInx-1][currentBoundary.choroColInx] = neighbourId;

//        numPicesInCols[currentBoundary.choroColInx]++;
//        numPicesInRows[currentBoundary.choroRowInx-1]++;

    }

    else if (currentBoundary.direction == SpatialRelation::Down)
    {
        if(currentBoundary.choroRowInx == (choroSize.height-1) && !lockVerticallShift)
        {
            shiftUp();
            currentBoundary.choroRowInx --;
            piecesArrangment[currentBoundary.choroRowInx+1][currentBoundary.choroColInx] = neighbourId;
        }
        else if(currentBoundary.choroRowInx != (choroSize.height-1))
            piecesArrangment[currentBoundary.choroRowInx+1][currentBoundary.choroColInx] = neighbourId;

//        numPicesInCols[currentBoundary.choroColInx]++;
//        numPicesInRows[currentBoundary.choroRowInx+1]++;
    }

    else if (currentBoundary.direction == SpatialRelation::Left && !lockHorizontalShift)
    {
        if (currentBoundary.choroColInx == 0)
        {
            shiftRight();
            currentBoundary.choroColInx ++;
            piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx-1] = neighbourId;
        }
        else if (currentBoundary.choroColInx != 0)
            piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx-1] = neighbourId;

//        numPicesInCols[currentBoundary.choroColInx-1]++;
//        numPicesInRows[currentBoundary.choroRowInx]++;
    }

    else if (currentBoundary.direction == SpatialRelation::Right && !lockHorizontalShift)
    {
        if (currentBoundary.choroColInx == (choroSize.width-1))
        {
            shiftLeft();
            currentBoundary.choroColInx --;
            piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx+1] = neighbourId;
        }
        else if (currentBoundary.choroColInx != (choroSize.width-1))
            piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx+1] = neighbourId;

//        numPicesInCols[currentBoundary.choroColInx+1]++;
//        numPicesInRows[currentBoundary.choroRowInx]++;
    }

    occupiedPosition ++;

    assert(availabalePieces[neighbourId] == true);
    availabalePieces[neighbourId] = false;
}


// check
void Choromosome::calculateFitness(const std::vector<Piece> & pieces)
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
    for(int i=0; i< choroSize.height; i++)
        for(int j=0; j<choroSize.width; j++)
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

// check
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

// sort based on operator < over fitness parameter
bool Choromosome::operator < (const Choromosome & rhs) const
{
    return this->fitness < rhs.getFitness();
}

void Choromosome::shiftLeft()
{
    for(size_t i=0; i<choroSize.height; i++)
        for(size_t j=1; j<choroSize.width;j++)
            piecesArrangment[i][j-1]= piecesArrangment[i][j];

    for(size_t i=0; i<choroSize.height; i++)
        piecesArrangment[i][choroSize.width-1] = -1;
}

void Choromosome::shiftRight()
{
    for(size_t i=0; i<choroSize.height; i++)
        for(size_t j=choroSize.width-2; j<= 0;j--)
            piecesArrangment[i][j+1]= piecesArrangment[i][j];

    for(size_t i=0; i<choroSize.height; i++)
        piecesArrangment[i][0] = -1;
}

void Choromosome::shiftUp()
{
    for(size_t i=1; i<choroSize.height; i++)
        for(size_t j=0; j<choroSize.width;j++)
            piecesArrangment[i-1][j]= piecesArrangment[i][j];

    for(size_t j=0; j<choroSize.width; j++)
        piecesArrangment[choroSize.height-1][j] = -1;
}

void Choromosome::shiftDown()
{
    for(size_t i = choroSize.height-2; i<= 0; i--)
        for(size_t j=0; j<choroSize.width;j++)
            piecesArrangment[i+1][j]= piecesArrangment[i][j];

    for(size_t j=0; j<choroSize.width;j++)
        piecesArrangment[0][j]  = -1;
}
