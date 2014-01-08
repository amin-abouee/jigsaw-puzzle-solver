#ifndef CHOROMOSOME_HPP
#define CHOROMOSOME_HPP

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include "Piece.hpp"
#include "SpatialRelation.hpp"
#include <queue>

class Choromosome
{
public:

    //C'tor
    Choromosome(int width, int height);

    //D'tor
    virtual ~Choromosome();

    void getFreeBoundries(std::vector<SpatialRelation> &freeBounderiesPositions);

    void assignPiece(SpatialRelation currentBoundary, int neighbourId);

    void assignPiece(int rowInx, int colInx, int seedInx);

    void calculateFitness(const std::vector<Piece>& pieces);

    int getNeighbour (SpatialRelation currentBoundary) const;

    bool checkPieceAvailability (int pieceId) const;

    void checkImageBounderies(SpatialRelation currentBoundary);

    void generateChoromosome(std::vector<int> &randVec);

    bool operator < (const Choromosome & rhs) const;

    double getFitness() const;

    int getOccupiedPositions() const;

    void shiftLeft();

    void shiftRight();

    void shiftUp();

    void shiftDown();

    std::vector <bool> availabalePieces;

private:

    std::vector < std::vector <int> > piecesArrangment;

//    std::vector <int> numPicesInRows;

//    std::vector <int> numPicesInCols;

    cv::Size choroSize;

    double fitness;

    int occupiedPosition;

    bool lockVerticallShift;

    bool lockHorizontalShift;
};

#endif /* CHOROMOSOME_HPP */
