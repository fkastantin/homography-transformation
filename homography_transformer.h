#ifndef HOMOGRAPHY_TRANSFORMER
#define HOMOGRAPHY_TRANSFORMER

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>


class HomographyTransformer {
    private:
    cv::Mat H;
    int new_plane_width, new_plane_height;
    double multiplication_ratio;
    int starting_x, starting_y;

    cv::Mat mapping;

    void init(){
        mapping = cv::Mat::zeros(3, new_plane_width * new_plane_height, CV_64F);

        for (int i = 0; i < new_plane_width; i++) {
            for (int j = 0; j < new_plane_height; j++) {
                mapping.at<double>(0, i * new_plane_height + j) = (i + starting_x) * 1.0 / multiplication_ratio;
                mapping.at<double>(1, i * new_plane_height + j) = (j + starting_y) * 1.0 / multiplication_ratio;
                mapping.at<double>(2, i * new_plane_height + j) = 1;
            }
        }

        cv::Mat invTr = H.inv();

        mapping = invTr * mapping;
    }

    public:

    /**
     * Constructor
     * 
     * @param H Homography matrix 3*3 (if you have the matrix stored in txt file, you can read it using the function HomographyTransformer::readMatrix)
     * @param new_plane_width New image width
     * @param new_plane_height New image height
     * @param multiplication_ratio How much do you want to scale the output image (its value depends on your correspondences which you used to calculate the homography matrix), if the value is 1 then no scaling will be applied
     * @param starting_x How many pixels do you want to translate the output image of x axis
     * @param starting_y How many pixels do you want to translate the output image of y axis (its value depends on the your correspondences and multiplication_ratio)
     * 
     */
    HomographyTransformer(
        cv::Mat &H, 
        int new_plane_width, 
        int new_plane_height, 
        double multiplication_ratio, 
        int starting_x, 
        int starting_y
    ){
        this->H = H;
        this->new_plane_width = new_plane_width;
        this->new_plane_height = new_plane_height;
        this->multiplication_ratio = multiplication_ratio;
        this->starting_x = starting_x;
        this->starting_y = starting_y;
        // Initlize mapping vector
        init();
    }

    /**
     * Do transformation to the new plane
     * 
     * @param origin_plane Original image
     * @return The image after transformation
     */
    cv::Mat transformImage(
        cv::Mat &origin_plane
    ) {
        
        cv::Mat new_plane = cv::Mat::zeros(new_plane_height, new_plane_width, origin_plane.type());

        const int WIDTH = origin_plane.cols;
	    const int HEIGHT = origin_plane.rows;

        for (int i = 0; i < new_plane_width; i++) {
            for (int j = 0; j < new_plane_height; j++) {
                int newX = round(mapping.at<double>(0, i * new_plane_height + j) / mapping.at<double>(2, i * new_plane_height + j));
                int newY = round(mapping.at<double>(1, i * new_plane_height + j) / mapping.at<double>(2, i * new_plane_height + j));


                if ((newX >= 0) && (newX < WIDTH) && (newY >= 0) && (newY < HEIGHT))
                    new_plane.at<cv::Vec3b>(j, i) = origin_plane.at<cv::Vec3b>(newY, newX);
            }
        }

        return new_plane;
    }

    /**
     * Read matrix from txt file
     * 
     * @param file_name The src file path
     * @param rows Number of rows of the matrix
     * @param cols Number of columns of the matrix
     * @param K The output matrix
     */
    static void readMatrix(std::string file_name, int rows, int cols, cv::Mat& K_) {
        std::ifstream file_stream(file_name);

        K_ = cv::Mat::zeros(rows, cols, CV_64F);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                double e; file_stream >> e;
                K_.at<double>(i, j) = e;
            }
        }

        file_stream.close();
    }
};

#endif
