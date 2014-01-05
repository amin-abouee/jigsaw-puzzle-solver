#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Piece.hpp"

Piece::Piece(int id, cv::Mat patchImage):patchImage(patchImage)
{

    //TODO: clear SortedDissimilarity and MAP
    this->id = id;
    this->bestBuddyRightId = -1;
    this->bestBuddyDownId = -1;
}

Piece::~Piece()
{

}

void Piece::foundBestBuddy()
{

}

void Piece::calculatePairwiseCompatibility(const Piece & adjecent)
{
    // Right
    cv::Mat leftCol = patchImage.col(patchImage.cols-1);
    cv::Mat rightCol = adjecent.patchImage.col(0);
    double rightError = cv::norm(leftCol, rightCol, cv::NORM_L2);

    sortedRightDissimiliratyValues.push_back(std::pair <int,double> (adjecent.id, rightError));
    rightDissimiliratyValues[adjecent.id] = rightError;

    // Down
    cv::Mat upRow = patchImage.row(patchImage.rows-1);
    cv::Mat downRow = adjecent.patchImage.row(0);
    double downError = cv::norm(upRow, downRow, cv::NORM_L2);

    sortedDownDissimiliratyValues.push_back(std::pair <int,double> (adjecent.id, downError));
    downDissimiliratyValues[adjecent.id] = downError;

    // Left
    leftCol = adjecent.patchImage.col(patchImage.cols-1);
    rightCol = patchImage.col(0);
    double leftError = cv::norm(leftCol, rightCol, cv::NORM_L2);

    sortedLeftDissimiliratyValues.push_back(std::pair <int, double> (adjecent.id, leftError));
    leftDissimiliratyValues[adjecent.id] = leftError;

    // Up
    upRow = adjecent.patchImage.row(patchImage.rows-1);
    downRow = patchImage.row(0);
    double upError = cv::norm(upRow, downRow, cv::NORM_L2);

    sortedUpDissimiliratyValues.push_back(std::pair <int,double> (adjecent.id, upError));
    upDissimiliratyValues[adjecent.id] = upError;

}

double Piece::getRightDissimilarityValue(int inx) const
{
    return rightDissimiliratyValues.at(inx);
}

double Piece::getDownDissimilarityValues(int inx) const
{
    return downDissimiliratyValues.at(inx);
}

void Piece::sortDissimilarityValues()
{
    std::sort (sortedRightDissimiliratyValues.begin(), sortedRightDissimiliratyValues.end());
    std::sort (sortedDownDissimiliratyValues.begin(), sortedDownDissimiliratyValues.end());
    std::sort (sortedLeftDissimiliratyValues.begin(), sortedLeftDissimiliratyValues.end());
    std::sort (sortedUpDissimiliratyValues.begin(), sortedUpDissimiliratyValues.end());
}

void Piece::setBestBuddies()
{
    bestBuddyRightId = sortedRightDissimiliratyValues[0].first;
    bestBuddyDownId = sortedDownDissimiliratyValues[0].first;
    bestBuddyUpId = sortedUpDissimiliratyValues[0].first;
    bestBuddyLeftId = sortedLeftDissimiliratyValues[0].first;
}

bool Piece::isBestBuddy (const Piece & neighbour, SpatialRelation::Direction direction) const
{
    if(direction == SpatialRelation::Up)
        return (this->bestBuddyUpId == neighbour.id);

    if(direction == SpatialRelation::Left)
        return (this->bestBuddyLeftId == neighbour.id);

    if(direction == SpatialRelation::Down)
        return (this->bestBuddyDownId == neighbour.id);

    if(direction == SpatialRelation::Right)
        return (this->bestBuddyRightId == neighbour.id);
}

int Piece::getBestMatch(std::vector<bool> & pieceAvailability, SpatialRelation::Direction direction) const
{
    if(direction == SpatialRelation::Up)
    {
        for (const auto & dissimilarity : sortedUpDissimiliratyValues)
        {
            int pieceId = dissimilarity.first;
            if(pieceAvailability[pieceId] == true)
                return pieceId;
        }
    }

    if(direction == SpatialRelation::Down)
    {
        for (const auto & dissimilarity : sortedDownDissimiliratyValues)
        {
            int pieceId = dissimilarity.first;
            if(pieceAvailability[pieceId] == true)
                return pieceId;
        }
    }

    if(direction == SpatialRelation::Left)
    {
        for (const auto & dissimilarity : sortedLeftDissimiliratyValues)
        {
            int pieceId = dissimilarity.first;
            if(pieceAvailability[pieceId] == true)
                return pieceId;
        }
    }

    if(direction == SpatialRelation::Right)
    {
        for (const auto & dissimilarity : sortedRightDissimiliratyValues)
        {
            int pieceId = dissimilarity.first;
            if(pieceAvailability[pieceId] == true)
                return pieceId;
        }
    }

}
