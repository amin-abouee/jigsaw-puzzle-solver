#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "Piece.hpp"

Piece::Piece(int id, cv::Mat patchImage):patchImage(patchImage)
{
    this->id = id;
}

Piece::~Piece()
{

}

void Piece::foundBestBuddy()
{

}

void Piece::calculatePairwiseCompatibility(const Piece & adjecent)
{
    cv::Mat leftCol = patchImage.col(patchImage.cols-1);
    cv::Mat rightCol = adjecent.patchImage.col(0);
    double rightError = cv::norm(leftCol, rightCol, cv::NORM_L2);
    rightDissimiliratyValues.push_back(std::pair <int,double> (adjecent.id, rightError));


    //TODO :check up and down
    cv::Mat upRow = patchImage.row(patchImage.rows-1);
    cv::Mat downRow = adjecent.patchImage.row(0);
    double upError = cv::norm(upRow, downRow, cv::NORM_L2);
    upDissimiliratyValues.push_back(std::pair <int,double> (adjecent.id, upError));
}

double Piece::getRightDissimilarityValue(int inx) const
{
    return rightDissimiliratyValues[inx].second;
}

double Piece::getUpDissimilarityValues(int inx) const
{
    return upDissimiliratyValues[inx].second;
}
