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
    Choromosome(int width, int height, std::vector <int> inTem);

    //D'tor
    virtual ~Choromosome();

    void getFreeBoundries(std::vector<SpatialRelation> &freeBounderiesPositions);

    void assignPiece(SpatialRelation currentBoundary, int neighbourId);

    void assignPiece(int rowInx, int colInx, int seedInx);

    void calculateFitness(const std::vector<Piece>& pieces);

    int getNeighbour (SpatialRelation currentBoundary) const;

    bool checkPieceAvailability (int pieceId) const;

    void checkShifftingIsAllow ();

    bool checkRowIsEmpty (int rowInx);

    bool checkColIsEmpty (int colInx);

    void generateChoromosome(std::vector<int> &randVec);

    bool operator < (const Choromosome & rhs) const;

    void setPiecesArrangment (int rowInx, int colInx, int neighbourId);

    double getFitness() const;

    int getOccupiedPositions() const;

    void shiftLeft();

    void shiftRight();

    void shiftUp();

    void shiftDown();

    void printChoromosome() const;

    std::vector <bool> availabalePieces;

    void checkChoro ();

private:

    std::vector < std::vector <int> > piecesArrangment;

    int width;

    int height;

    double fitness;

    int occupiedPosition;

    bool lockVerticallShift;

    bool lockHorizontalShift;

};

#endif /* CHOROMOSOME_HPP */
