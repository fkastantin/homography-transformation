#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <homography_transformer.h>



int main(int argc, char** argv )
{
    // Setting the random seed to get random results in each run.
	srand(time(NULL));

	if (argc < 8)
	{
		std::cout << "some arguments are missing" << std::endl;
		return -1;
	}
    std::string homography_matrix_file = argv[1];
    std::string input_image_file = argv[2];
    std::string output_image_file = argv[3];

	int output_width = std::stoi(argv[4]);
    int output_height = std::stoi(argv[5]);
    double multiplication_ratio = std::stod(argv[6]);
    int starting_x = std::stoi(argv[7]);
    int starting_y = std::stoi(argv[8]);

    // Read homography matrix
    cv::Mat H;
    HomographyTransformer::readMatrix(homography_matrix_file, 3, 3, H);
	std::cout << "Homography matrix: " << std::endl;
    std::cout << H << std::endl;
    
    cv::Mat original_image = cv::imread(input_image_file);

	clock_t begin = clock();
	HomographyTransformer homographyTransformer(H, output_width, output_height, multiplication_ratio, starting_x, starting_y);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "elapsed seconds for mapping construction: " << elapsed_secs << std::endl;

	begin = clock();
    //homographyTransformer.transformImage(original_image, new_image);
	cv::Mat new_image = homographyTransformer.transformImage(original_image);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "elapsed seconds for transformation: " << elapsed_secs << std::endl;
    imwrite(output_image_file, new_image);

    return 0;
}