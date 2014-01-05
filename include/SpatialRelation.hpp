#ifndef SPATIALRELATION_HPP
#define SPATIALRELATION_HPP

#include <iostream>

class SpatialRelation
{
public:

    enum Direction
    {
        Up,
        Right,
        Down,
        Left
    };

    //C'tor
    SpatialRelation(Direction direction, int pieceIndex, int choroRowInx, int choroColInx );

    SpatialRelation(Direction direction, int pieceIndex);

    //D'tor
    virtual ~SpatialRelation();

//    int get

//private:

    Direction direction;

    int pieceIndex;

    int choroRowInx;

    int choroColInx;
};

#endif // SPATIALRELATION_HPP
