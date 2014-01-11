#ifndef PIECE_HPP
#define PIECE_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <map>
#include "SpatialRelation.hpp"

class Piece
{
public:

    //C'tor
    Piece(int id, cv::Mat patchImage);

    //D'tor
    virtual ~Piece();

    void foundBestBuddy();

    void calculatePairwiseCompatibility (const Piece &adjecent);

    double getRightDissimilarityValue (int inx) const;

    double getDownDissimilarityValues (int inx) const;

    void sortDissimilarityValues();

    void setBestBuddies();

    int getBestMatch (std::vector <bool> & pieceAvailability, SpatialRelation::Direction direction) const;

    bool isBestBuddy(const Piece & neighbour, SpatialRelation::Direction direction) const;

    int checkPiece();

private:

    int id;
    cv::Mat patchImage;

    std::vector < std::pair <int, double> > sortedRightDissimiliratyValues;
    std::vector < std::pair <int, double> > sortedDownDissimiliratyValues;
    std::vector < std::pair <int, double> > sortedLeftDissimiliratyValues;
    std::vector < std::pair <int, double> > sortedUpDissimiliratyValues;

    std::map <int, double> rightDissimiliratyValues;
    std::map <int, double> downDissimiliratyValues;
    std::map <int, double> leftDissimiliratyValues;
    std::map <int, double> upDissimiliratyValues;


    std::vector <int> bestBuddyRightId;
    std::vector <int> bestBuddyDownId;
    std::vector <int> bestBuddyUpId;
    std::vector <int> bestBuddyLeftId;
};

#endif /* PIECE_HPP */
