#include "SpatialRelation.hpp"

SpatialRelation::SpatialRelation(Direction direction, int pieceIndex, int choroRowInx, int choroColInx)
{
    this->direction = direction;
    this->pieceIndex = pieceIndex;
    this->choroRowInx = choroRowInx;
    this->choroColInx = choroColInx;
}

SpatialRelation::SpatialRelation(Direction direction, int pieceIndex)
{
    this->direction = direction;
    this->pieceIndex = pieceIndex;
}

SpatialRelation::~SpatialRelation() {}
