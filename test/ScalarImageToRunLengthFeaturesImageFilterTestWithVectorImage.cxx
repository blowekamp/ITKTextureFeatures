/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkScalarImageToRunLengthFeaturesImageFilter.h"

#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkNeighborhood.h"
#include "itkTestingMacros.h"

int ScalarImageToRunLengthFeaturesImageFilterTestWithVectorImage( int argc, char *argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0]
      << " inputImageFile"
      << " maskImageFile"
      << " outputImageFile"
      << " [numberOfBinsPerAxis]"
      << " [pixelValueMin]"
      << " [pixelValueMax]"
      << " [minDistance]"
      << " [maxDistance]"
      << " [neighborhoodRadius]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int ImageDimension = 3;

  // Declare types
  typedef int   InputPixelType;
  typedef float OutputPixelType;

  typedef itk::Image< InputPixelType, ImageDimension >        InputImageType;
  typedef itk::VectorImage< OutputPixelType, ImageDimension > OutputImageType;
  typedef itk::ImageFileReader< InputImageType >              ReaderType;
  typedef itk::Neighborhood< typename InputImageType::PixelType,
    InputImageType::ImageDimension >                          NeighborhoodType;

  // Create and set up a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  // Create and set up a maskReader
  ReaderType::Pointer maskReader = ReaderType::New();
  maskReader->SetFileName( argv[2] );

  // Create the filter
  typedef itk::Statistics::ScalarImageToRunLengthFeaturesImageFilter<
    InputImageType, OutputImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();

  EXERCISE_BASIC_OBJECT_METHODS( filter, ScalarImageToRunLengthFeaturesImageFilter,
    ImageToImageFilter );


  filter->SetInput( reader->GetOutput() );
  filter->SetMaskImage( maskReader->GetOutput() );

  if( argc >= 5 )
    {
    unsigned int numberOfBinsPerAxis = std::atoi( argv[4] );
    filter->SetNumberOfBinsPerAxis( numberOfBinsPerAxis );

    FilterType::PixelType pixelValueMin = std::atof( argv[5] );
    FilterType::PixelType pixelValueMax = std::atof( argv[6] );
    filter->SetPixelValueMinMax( pixelValueMin, pixelValueMax );

    FilterType::RealType minDistance = std::atof( argv[7] );
    FilterType::RealType maxDistance = std::atof( argv[8] );
    filter->SetDistanceValueMinMax( minDistance, maxDistance );

    NeighborhoodType::SizeValueType neighborhoodRadius = std::atoi( argv[9] );
    NeighborhoodType hood;
    hood.SetRadius( neighborhoodRadius );
    filter->SetNeighborhoodRadius( hood.GetRadius() );
    }

  TRY_EXPECT_NO_EXCEPTION( filter->Update() );

  // Create and set up a writer
  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( filter->GetOutput() );

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
