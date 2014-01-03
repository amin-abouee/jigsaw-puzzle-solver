#include <opencv2/opencv.hpp>
#include "GeneticAlgorithm.hpp"
#include "Choromosome.hpp"


int main(int argc, char* argv[])
{
    static_assert(__cplusplus > 199711L, "Program requires C++11 capable compiler");

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);

    int patchSize = atoi(argv[2]);

    GeneticAlgorithm GA(image, patchSize);

    const int numGeneration = 100;
    const int numElitism = 4;

    GA.generatePopulation(1000);

    for(int i=0; i<numGeneration; i++)
    {
        GA.evaluateAllChoromosoms();
        GA.selectElitism(numElitism);
        const Choromosome & parent1 = GA.selectionChromosome();
        const Choromosome & parent2 = GA.selectionChromosome();
        GA.crossOver(parent1, parent2);
    }

    return 0;
}
