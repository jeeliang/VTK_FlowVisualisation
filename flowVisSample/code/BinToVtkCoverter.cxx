#include <iostream>
#include <fstream>
#include <vector>
#include "vtkSmartPointer.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"

int main()
{
    std::string inputFileName = "../data/cloud.bin";
    std::string outputFileName = "../data/cloud.vtk";

    // Read the binary file
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile)
    {
        std::cerr << "Error opening input file: " << inputFileName << std::endl;
        return 1;
    }

    // Get the file size
    inputFile.seekg(0, std::ios::end);
    std::streamsize fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    // Calculate the number of points in the binary data
    std::size_t numPoints = fileSize / sizeof(float) / 3;

    // Read the binary data into a vector
    std::vector<float> data(numPoints * 3);
    inputFile.read(reinterpret_cast<char*>(data.data()), fileSize);
    inputFile.close();

    // Create VTK points and add the coordinates
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints(numPoints);
    for (std::size_t i = 0; i < numPoints; ++i)
    {
        float x = data[i * 3];
        float y = data[i * 3 + 1];
        float z = data[i * 3 + 2];
        points->SetPoint(i, x, y, z);
    }

    // Create VTK polydata and set the points
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);

    // Write the polydata to a VTK file
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName(outputFileName.c_str());
    writer->SetInputData(polyData);
    writer->Write();

    std::cout << "File converted successfully: " << outputFileName << std::endl;

    return 0;
}
