#include <opencv2/opencv.hpp>
#include "Choromosome.hpp"
#include "GeneticAlgorithm.hpp"

int main(int argc, char* argv[])
{
    static_assert(__cplusplus > 199711L, "Program requires C++11 capable compiler");
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
    //    cvtColor(image, image, CV_BGR2Lab);

    int patchSize = atoi(argv[2]);

    GeneticAlgorithm GA(image, patchSize);

    // change
    const int numPopulation = 200;
    const int numGeneration = 100;
    const int numElitism = 10;
    const int numCrossOverOperation = numPopulation - numElitism;

    GA.generatePopulation(numPopulation);

    // don't use inside the code
    GA.generatePieces();
    GA.initiatePieces();

    for (int i = 0; i < numGeneration; i++)
    {
        std::cout << "generation: " << i + 1 << std::endl;
        GA.evaluateAllChoromosoms();
        Choromosome best = GA.getBestChromosome();
        best.printChoromosome();
        GA.selectElitism(numElitism);

        for (int j = 0; j < numCrossOverOperation; j++)
        {
            const Choromosome& parent1 = GA.selectionChromosome();
            const Choromosome& parent2 = GA.selectionChromosome();

            if (&parent1 == &parent2)
                continue;

            GA.crossOver(parent1, parent2);
        }

        GA.copyNewPopulationToPopulation();
    }

    GA.evaluateAllChoromosoms();
    Choromosome best = GA.getBestChromosome();
    best.printChoromosome();

    return 0;
}
