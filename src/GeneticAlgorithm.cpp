#include "GeneticAlgorithm.hpp"
#include "Choromosome.hpp"
#include "Piece.hpp"
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <assert.h>

GeneticAlgorithm::GeneticAlgorithm(cv::Mat image, int size):image(image), patchSize(size,size)
{
    numPieces = (image.rows / size) * (image.cols / size);
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
        Choromosome temp(rows, cols);
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
            pieces[i].calculatePairwiseCompatibility(pieces[j]);
}

void GeneticAlgorithm::evaluateAllChoromosoms()
{
    for(auto & choromosome : population)
    {
        choromosome.calculateFitness(pieces);
    }
}

const Choromosome & GeneticAlgorithm::selectionChromosome()
{

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

}

cv::Mat GeneticAlgorithm::splitImage(int rowPiece, int colPiece) const
{
    cv::Rect ROIRect(colPiece * patchSize.width, rowPiece * patchSize.height, patchSize.width, patchSize.height);
    cv::Mat ROI = image(ROIRect);
    return ROI;
}
