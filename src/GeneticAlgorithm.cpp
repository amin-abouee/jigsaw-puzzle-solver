#include "GeneticAlgorithm.hpp"
#include "Choromosome.hpp"
#include "Piece.hpp"
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <assert.h>
#include <random>

GeneticAlgorithm::GeneticAlgorithm(cv::Mat image, int size):image(image), patchSize(size,size)
{
    numPieces = (image.rows / size) * (image.cols / size);
    totalFitness = 0;
    pieces.reserve(numPieces);
}

GeneticAlgorithm::~GeneticAlgorithm()
{

}

void GeneticAlgorithm::generatePopulation(int numChoromosomes)
{
    int cols = image.cols / patchSize.width;
    int rows = image.rows / patchSize.height;
    population.reserve(numChoromosomes);

    for(size_t i=0; i<numChoromosomes; i++)
    {
        Choromosome temp(cols, rows);
        temp.generateChoromosome();
        population.push_back(temp);
    }
}

void GeneticAlgorithm::generatePieces()
{
    int cols = image.cols / patchSize.width;
    int rows = image.rows / patchSize.height;

    for(size_t i=0; i< rows; i++)
        for(size_t j=0; j<cols; j++)
        {
            cv::Mat patchImage = splitImage(i,j);
            int id = i * rows + j;
            pieces.push_back(Piece(id, patchImage));
        }
}

void GeneticAlgorithm::initiatePieces()
{
    for(size_t i=0; i<pieces.size(); i++)
        for(size_t j=0; j<pieces.size(); j++)
        {
            if(i == j)
                continue;
            pieces[i].calculatePairwiseCompatibility(pieces[j]);
        }

    for(auto & piece : pieces)
        piece.sortDissimilarityValues();
}

void GeneticAlgorithm::evaluateAllChoromosoms()
{
    totalFitness = 0;
    for(auto & choromosome : population)
    {
        choromosome.calculateFitness(pieces);
        totalFitness += choromosome.getFitness();
    }
}

const Choromosome & GeneticAlgorithm::selectionChromosome()
{
    std::default_random_engine generator;
    std::uniform_int_distribution<double> distribution(0,totalFitness);
    double randomFitness = distribution(generator);

    double sum = 0;
    for(auto & choromosome : population)
    {
        sum += choromosome.getFitness();
        if(sum >= randomFitness)
            return choromosome;
    }
    std::cerr << "choromosome not selected!!" <<std::endl;
}

void GeneticAlgorithm::selectElitism(int numElitism)
{
    assert(numElitism < population.size());

    std::sort(population.begin(), population.end());
    for(int i=0; i<numElitism; i++)
        newPopulation.push_back(population[i]);
}

void GeneticAlgorithm::crossOver(const Choromosome &parent1, const Choromosome &parent2)
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,numPieces);
    int seedInx = distribution(generator);

    int cols = image.cols / patchSize.width;
    int rows = image.rows / patchSize.height;

    Choromosome offSpring(cols, rows);
    // TODO assignPiece for one sample;
//    offSpring.assignPiece(14, 14, seedInx);

    while(offSpring.getOccupiedPositions() < numPieces)
    {
        offSpring.getFreeBoundries(freeBounderiesPositions);
        while(!freeBounderiesPositions.empty())
        {
            SpatialRelation currentBoundary = freeBounderiesPositions.front();
            freeBounderiesPositions.pop();

            bool resultParent = setNeighbourByParents(offSpring, parent1, parent2, currentBoundary);
            if(resultParent)
                continue;

            bool resultBesBuddy = setNeighbourByBestBuddy(offSpring, parent1, parent2, currentBoundary);
            if(resultBesBuddy)
                continue;

            setNeighbourByBestMatch(offSpring, currentBoundary);
        }

    }
}

cv::Mat GeneticAlgorithm::splitImage(int rowPiece, int colPiece) const
{
    cv::Rect ROIRect(colPiece * patchSize.width, rowPiece * patchSize.height, patchSize.width, patchSize.height);
    cv::Mat ROI = image(ROIRect);
    return ROI;
}

bool GeneticAlgorithm::setNeighbourByParents(Choromosome & offSpring, const Choromosome & parent1, const Choromosome & parent2, const SpatialRelation & currentBoundary)
{
    int neighbourParent1 = parent1.getNeighbour(currentBoundary);
    int neighbourParent2 = parent2.getNeighbour(currentBoundary);

    if(neighbourParent1 != -1 && neighbourParent2 != -1)
    {
        if(offSpring.checkPieceAvailability(neighbourParent1) && offSpring.checkPieceAvailability(neighbourParent2))
        {
            if(neighbourParent1 == neighbourParent2)
            {
                offSpring.assignPiece(currentBoundary, neighbourParent1);
                return true;
            }
        }
    }

    return false;
}


bool GeneticAlgorithm::setNeighbourByBestBuddy(Choromosome &offSpring, const Choromosome &parent1, const Choromosome &parent2, const SpatialRelation &currentBoundary)
{
    int neighbourParent1 = parent1.getNeighbour(currentBoundary);
    int neighbourParent2 = parent2.getNeighbour(currentBoundary);

    int currentPieceId = currentBoundary.pieceIndex;

    // TDOD check availability and -1

    if (pieces[currentPieceId].isBestBuddy(pieces[neighbourParent1], currentBoundary.direction))
    {
        offSpring.assignPiece(currentBoundary, neighbourParent1);
        return true;
    }

    else if (pieces[currentPieceId].isBestBuddy(pieces[neighbourParent2], currentBoundary.direction))
    {
        offSpring.assignPiece(currentBoundary, neighbourParent2);
        return true;
    }

    return false;
}

void GeneticAlgorithm::setNeighbourByBestMatch(Choromosome & offSpring, const SpatialRelation & currentBoundary)
{
    int neighbourId = pieces[currentBoundary.pieceIndex].getBestMatch(offSpring.availabalePieces, currentBoundary.direction);
    offSpring.assignPiece(currentBoundary, neighbourId);
}
