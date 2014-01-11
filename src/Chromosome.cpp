#include <iostream>
#include "Choromosome.hpp"
#include "Piece.hpp"
#include "SpatialRelation.hpp"
#include <random>

// check
Choromosome::Choromosome(int width, int height)
{
    // check this place
    this->width = width;
    this->height = height;
    std::vector <int> temp (width, -1);
    piecesArrangment = std::vector < std::vector <int> > (height, temp);
    availabalePieces = std::vector <bool> (height * width, true);
    fitness = 0;
    occupiedPosition = 0;
    lockHorizontalShift = false;
    lockVerticallShift = false;
    //    numPicesInRows = std::vector <int> (height,0);
    //    numPicesInCols = std::vector <int> (width,0);
}

Choromosome::Choromosome(int width, int height,std::vector <int> inTem)
{
    this->width = width;
    this->height = height;
    std::vector <int> temp (width, -1);
    piecesArrangment = std::vector < std::vector <int> > (height, temp);

    int inx=0;
    for(size_t i=0; i<height; i++)
        for(size_t j=0; j<width; j++)
            piecesArrangment[i][j]= inTem[inx++];
}

Choromosome::~Choromosome()
{

}

// check
void Choromosome::generateChoromosome(std::vector <int> & randVec)
{
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::shuffle(randVec.begin(), randVec.end(),generator);

    int inx = 0;
    for(size_t i=0; i< height; i++)
    {
        for(size_t j=0; j< width; j++)
        {
            availabalePieces[inx] = false;
            piecesArrangment[i][j] = randVec[inx++];
        }
    }
    occupiedPosition = height * width;
}

void Choromosome::swapTwoPieces(int row1, int col1, int row2, int col2)
{
    int temp = piecesArrangment[row1][col1];
    piecesArrangment[row1][col1] = piecesArrangment[row2][col2];
    piecesArrangment[row2][col2] = temp;
}

int Choromosome::getRandomAvailabalePiece(int numAvailabale)
{
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<int> distribution(1,numAvailabale);
    int randInx = distribution(generator);

    int availableSize = availabalePieces.size();
    int cnt = 0;
    for(int i=0; i< availableSize; i++)
        if (availabalePieces[i] == true)
        {
            cnt++;
            if(cnt == randInx)
                return i;
        }
}

void Choromosome::getFreeBoundries(std::vector <SpatialRelation> & freeBounderiesPositions)
{
    for(int i=0; i<height; i++)
        for(int j=0; j<width; j++)
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

                if ((i+1) < height && piecesArrangment[i+1][j] == -1)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Down, pieceInx, choroRowInx, choroColInx));
                }
                else if ((i+1) >= height && !lockVerticallShift)
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

                if ((j+1) < width && piecesArrangment[i][j+1] == -1)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Right, pieceInx, choroRowInx, choroColInx));
                }
                else if ((j+1) >= width && !lockHorizontalShift)
                {
                    freeBounderiesPositions.push_back(SpatialRelation(SpatialRelation::Right, pieceInx, choroRowInx, choroColInx));
                }
            }
        }
}

// check
void Choromosome::assignPiece(int rowInx, int colInx, int seedInx)
{
    setPiecesArrangment(rowInx, colInx, seedInx);
    checkShifftingIsAllow();
}

void Choromosome::checkShifftingIsAllow()
{
    if(!lockHorizontalShift)
    {
        if(!checkColIsEmpty(0) && !checkColIsEmpty(width-1))
            lockHorizontalShift = true;
    }

    if(!lockVerticallShift)
    {
        if(!checkRowIsEmpty(0) && !checkRowIsEmpty(height-1))
            lockVerticallShift = true;
    }
}

bool Choromosome::checkRowIsEmpty(int rowInx)
{
    for(size_t j=0; j<width;j++)
        if(piecesArrangment[rowInx][j] != -1)
            return false;
    return true;
}

bool Choromosome::checkColIsEmpty(int colInx)
{
    for(size_t i=0; i<height; i++)
        if(piecesArrangment[i][colInx] != -1)
            return false;

    return true;
}

void Choromosome::assignPiece(SpatialRelation currentBoundary, int neighbourId)
{
    assert(neighbourId != -1);
    if(currentBoundary.direction == SpatialRelation::Up)
    {
        if(currentBoundary.choroRowInx == 0 && !lockVerticallShift)
        {
            shiftDown();
            setPiecesArrangment(currentBoundary.choroRowInx,currentBoundary.choroColInx, neighbourId);
        }
        else if (currentBoundary.choroRowInx != 0)
            setPiecesArrangment(currentBoundary.choroRowInx-1,currentBoundary.choroColInx, neighbourId);

    }

    else if (currentBoundary.direction == SpatialRelation::Down)
    {
        if(currentBoundary.choroRowInx == (height-1) && !lockVerticallShift)
        {
            shiftUp();
            setPiecesArrangment(currentBoundary.choroRowInx,currentBoundary.choroColInx, neighbourId);
            //            piecesArrangment[currentBoundary.choroRowInx+1][currentBoundary.choroColInx] = neighbourId;
        }
        else if(currentBoundary.choroRowInx != (height-1))
            setPiecesArrangment(currentBoundary.choroRowInx+1,currentBoundary.choroColInx, neighbourId);
        //            piecesArrangment[currentBoundary.choroRowInx+1][currentBoundary.choroColInx] = neighbourId;
    }

    else if (currentBoundary.direction == SpatialRelation::Left )
    {
        if (currentBoundary.choroColInx == 0 && !lockHorizontalShift)
        {
            shiftRight();
            setPiecesArrangment(currentBoundary.choroRowInx,currentBoundary.choroColInx, neighbourId);
            //            piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx-1] = neighbourId;
        }
        else if (currentBoundary.choroColInx != 0)
            setPiecesArrangment(currentBoundary.choroRowInx,currentBoundary.choroColInx-1, neighbourId);
        //            piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx-1] = neighbourId;

    }

    else if (currentBoundary.direction == SpatialRelation::Right )
    {
        if (currentBoundary.choroColInx == (width-1) && !lockHorizontalShift)
        {
            shiftLeft();
            setPiecesArrangment(currentBoundary.choroRowInx,currentBoundary.choroColInx, neighbourId);
            //            piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx+1] = neighbourId;
        }
        else if (currentBoundary.choroColInx != (width-1))
            setPiecesArrangment(currentBoundary.choroRowInx,currentBoundary.choroColInx+1, neighbourId);
        //            piecesArrangment[currentBoundary.choroRowInx][currentBoundary.choroColInx+1] = neighbourId;

    }

    checkShifftingIsAllow();
}

void Choromosome::setPiecesArrangment(int rowInx, int colInx, int neighbourId)
{
    //    std::cout << "row: "<< rowInx <<" col: "<< colInx << " id: " << neighbourId << std::endl;
    piecesArrangment[rowInx][colInx] = neighbourId;
    occupiedPosition ++;
    assert(availabalePieces[neighbourId] == true);
    availabalePieces[neighbourId] = false;
}

// check
void Choromosome::calculateFitness(const std::vector<Piece> & pieces)
{
    fitness = 0;
    for(size_t i=0; i<height-1; i++)
        for(size_t j=0; j< width-1; j++)
        {
            int currentInx = piecesArrangment[i][j];
            int rightInx = piecesArrangment[i][j+1];
            int downInx = piecesArrangment[i+1][j];
            //            std::cout << "current index: "<<currentInx<<" right index: " <<rightInx << " down index: "<<downInx << std::endl;
            fitness += pieces[currentInx].getRightDissimilarityValue(rightInx);
            fitness += pieces[currentInx].getDownDissimilarityValues(downInx);
        }
}

int Choromosome::getNeighbour(SpatialRelation currentBoundary) const
{
    for(int i=0; i< height; i++)
        for(int j=0; j<width; j++)
            if(piecesArrangment[i][j] == currentBoundary.pieceIndex)
            {
                if(currentBoundary.direction == SpatialRelation::Up && (i-1) >= 0)
                    return piecesArrangment[i-1][j];

                else if (currentBoundary.direction == SpatialRelation::Down && (i+1) < height)
                    return piecesArrangment[i+1][j];

                else if (currentBoundary.direction == SpatialRelation::Left && (j-1) >= 0)
                    return piecesArrangment[i][j-1];

                else if (currentBoundary.direction == SpatialRelation::Right && (j+1) < width)
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
    for(size_t i=0; i<height; i++)
        for(size_t j=1; j<width;j++)
            piecesArrangment[i][j-1]= piecesArrangment[i][j];

    for(size_t i=0; i<height; i++)
        piecesArrangment[i][width-1] = -1;
}

void Choromosome::shiftRight()
{
    for(int i=0; i<height; i++)
        for(int j=width-2; j>= 0;j--)
            piecesArrangment[i][j+1]= piecesArrangment[i][j];

    for(size_t i=0; i<height; i++)
        piecesArrangment[i][0] = -1;
}

void Choromosome::shiftUp()
{
    for(size_t i=1; i<height; i++)
        for(size_t j=0; j<width;j++)
            piecesArrangment[i-1][j]= piecesArrangment[i][j];

    for(size_t j=0; j<width; j++)
        piecesArrangment[height-1][j] = -1;
}

void Choromosome::shiftDown()
{
    for(int i=height-2; i>= 0; i--)
        for(int j=0; j<width;j++)
            piecesArrangment[i+1][j]= piecesArrangment[i][j];

    for(size_t j=0; j<width;j++)
        piecesArrangment[0][j]  = -1;
}

void Choromosome::printChoromosome() const
{
    for(size_t i=0; i<height; i++)
    {
        for(size_t j=0; j<width; j++)
            std::cout << piecesArrangment[i][j]<< " ";
    }
    std::cout << std::endl;
}

void Choromosome::checkChoro()
{
    for(size_t i=0; i<height; i++)
        for(size_t j=0; j<width; j++)
            assert(piecesArrangment[i][j] != -1);
}
