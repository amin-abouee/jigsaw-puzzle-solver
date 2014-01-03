#ifndef GENETICALGORITHM_HPP
#define GENETICALGORITHM_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include "Choromosome.hpp"
#include "Piece.hpp"

class GeneticAlgorithm
{
public:

    //C'tor
    GeneticAlgorithm(cv::Mat image, int size);

    //D'tor
    virtual ~GeneticAlgorithm();

    const Choromosome & selectionChromosome();

    void selectElitism(int numElitism);

    void crossOver(const Choromosome & parent1, const Choromosome & parent2);

    cv::Mat splitImage(int rowPiece, int colPiece) const ;

    void generatePopulation(int numChoromosomes);

    void generatePieces();

    void evaluateAllChoromosoms();

    void initiatePieces();

private:

    std::vector <Choromosome> population;

    std::vector <Choromosome> newPopulation;

    int numPieces;

    std::vector <Piece> pieces;

    cv::Mat image;

    cv::Size patchSize;

};

#endif /* GENETICALGORITHM_HPP */
