#ifndef PIECE_HPP
#define PIECE_HPP

#include <opencv2/opencv.hpp>
#include <vector>

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

    double getUpDissimilarityValues (int inx) const;

private:

    int id;
    cv::Mat patchImage;
    std::vector < std::pair <int, double> > rightDissimiliratyValues;
    std::vector < std::pair <int, double> > upDissimiliratyValues;
};

#endif /* PIECE_HPP */
