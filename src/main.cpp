#include <opencv2/opencv.hpp>
#include "GeneticAlgorithm.hpp"
#include "Choromosome.hpp"


int main(int argc, char* argv[])
{
    static_assert(__cplusplus > 199711L, "Program requires C++11 capable compiler");
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
    //cvtColor(image, image, CV_BGR2Lab);

    int patchSize = atoi(argv[2]);

    GeneticAlgorithm GA(image, patchSize);

    const int numPopulation = 1000;
    const int numGeneration = 100;
    const int numElitism = 4;
    const int numCrossOverOperation = 996;

    GA.generatePopulation(numPopulation);

    // don't use inside the code
    GA.generatePieces();
    GA.initiatePieces();

    for(int i=0; i<numGeneration; i++)
    {
        GA.evaluateAllChoromosoms();
        GA.selectElitism(numElitism);

        std::cout << "generation: " << i+1 << std::endl;
        for(int j=0; j<numCrossOverOperation; j++)
        {
//            std::cout << "cross: " << j << std::endl;
            const Choromosome & parent1 = GA.selectionChromosome();
            const Choromosome & parent2 = GA.selectionChromosome();

            if(&parent1 == &parent2)
                std::cout <<"error"<<std::endl;

            GA.crossOver(parent1, parent2);
        }

        Choromosome best = GA.getBestChromosome();
        best.printChoromosome();
        GA.copyNewPopulationToPopulation();
    }

//    Choromosome best = GA.getBestChromosome();
//    best.printChoromosome();

    return 0;
}
