#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>

using namespace cv;
using namespace std;

int main()
{

/////////////////////////////////////////////////////////////////////////
	// Load map (google map)
	
	Mat map = imread("im_with_coord_chart_mod.png");
    
	// This part uses a grayscale binary threshold
	// Converts google map image to gray 
	// Mat gray_map;
	// cvtColor(map, gray_map, COLOR_BGR2GRAY); 

	// Converts gray to 8bits gray, was used to try to solve an issue with non binary values (issue was solved by writing (y,x) instead of (x,y) when accessing pixel value)
	// gray_map.convertTo(gray_map, CV_8U);
	// double min_val, max_val;
	// minMaxLoc(gray_map, &min_val, &max_val);
	// gray_map = 255 * (gray_map - min_val) / (max_val - min_val);

	// imshow("Grayscale map", gray_map);
	// Mat thresholded_map;
	// threshold(gray_map, thresholded_map, 200, 255, THRESH_BINARY_INV);
	// imshow("thresholded_map", thresholded_map);
	

	// Keeps only the bluish values (color value found using gimp) as white and turns everything else black
	//for google maps
	//Scalar lower(225, 167, 124);  //defined with 20.7 threshold in GIMP
	//Scalar upper(255, 219, 190);
	
	//for nautical chart
	Scalar lower(215, 151, 11);  //defined with 20.7 threshold in GIMP
	Scalar upper(255, 224, 174);

	// Create binary mask for color range
	
	Mat thresholded_map;
	inRange(map, lower, upper, thresholded_map);
	imshow("thresholded_map", thresholded_map);
	//imwrite("thresholded_map.png", thresholded_map);
    
	
	///Highpass filtering of the black and white map image, to reveal the coast line only. Will be used to confirm accuracy of thresholded satellite image. 
    


	// Define the high-pass filter kernel
	Mat kernel_high = (Mat_<float>(3,3) <<
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1);

	// Apply the high-pass filter to the binary map 
	Mat high_filtered_image;
	filter2D(thresholded_map, high_filtered_image, -1, kernel_high);

	// Display the original and filtered images
	imshow("Original Image", thresholded_map);
	imshow("Filtered Image", high_filtered_image);
	
	
	//erode the shoreline to make it bigger (maybe it is more representative than comparing our processed image to a line(hit or miss))
	Mat dilated_shoreline;
	Mat kernel_dilation = getStructuringElement(MORPH_RECT, Size(2, 2)); //best size empirically
	dilate(high_filtered_image, dilated_shoreline, kernel_dilation);
	
	
	//reverse intensity to have the shore line in black, and later compare with the thresholded satellite image (to see if shoreline is nicely detected)
	Mat reversed_shoreline;
	bitwise_not(dilated_shoreline, reversed_shoreline);
	imshow("Reversed Filtered Image", reversed_shoreline);

    
	// Counts pixel in the thresholded map, was used to try and identified segmentation fault reason
	int pixel_counter = 0;

	for(int y=0;y<thresholded_map.rows;y++)
	{
		for(int x=0;x<thresholded_map.cols;x++)
		{
		 	pixel_counter++;

		}
	}
        
	cout << "total number of pixels in thresholded_map:" << pixel_counter <<endl; 



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load the satellite image
	Mat image = imread("im_with_coord.png");

	// Convert the satellite image to grayscale
	Mat gray_image;
	cvtColor(image, gray_image, COLOR_BGR2GRAY);



	//Do a high pass filtering on the gray satellite image. It's one way of distinguishing water (low frequency) and the rest of the image. The parameters could be searched through like for color

	Mat high_filtered_sat; //testing the high pass filtering on satellite image
	filter2D(gray_image, high_filtered_sat, -1, kernel_high);
	// Display the original and filtered images
	imshow("Original Image", thresholded_map);
	imshow("Filtered Image sat", high_filtered_sat);



	// Apply a threshold to the filtered satellite image, to isolate the low frequency areas that correspond to the water bodies 
	Mat thresholded_image_filterd;
	threshold(high_filtered_sat, thresholded_image_filterd, 50, 255, THRESH_BINARY_INV); 

	// Apply a morphological operation (dilation) to remove noise and improve the quality of the thresholded image

	Mat dilated_image_filterd;
	Mat kernelK = getStructuringElement(MORPH_RECT, Size(1, 1));
	dilate(thresholded_image_filterd, dilated_image_filterd, kernelK);

	//imshow("Thresholded Image_filterd", thresholded_image_filterd);
	//imshow("Dilated Image_filterd", dilated_image_filterd);


	// Probably uselsess sequence destined to try to fix the non binary results.
	//gray_image.convertTo(gray_image, CV_8U);
	//double min_value, max_value;
	//minMaxLoc(gray_image, &min_value, &max_value);
	//gray_image = 255 * (gray_image - min_value) / (max_value - min_value);
	
	
//////////////Open file to store data


	ofstream myfile;
	myfile.open ("thresholding_results.csv");
	myfile << "threshold value, element size, nb of pixels, correct pixels, wrong pixels, false path, false obstacle, real shorline, detected shoreline, ratio correct shore line, nb reference path, nb reference obstacles\n";
	
	

////////////// Try to find the best values for color threshold and element size


	for (int threshold_val = 50; threshold_val < 100; threshold_val++)
	{
		for (int element_size = 1; element_size< 15; element_size ++)
		{

			//variables for comparing overall
			double nb_false_path = 0;
			double nb_false_obstacle = 0;
			double nb_wrong_pixel = 0;
			double nb_pixel_thresholded_image =0;
			double wtf = 0;
			double nb_correct_pixels =0;
			
			//counting the number of black and white pixels in the reference image (the chart or the map, thresholded)
			double nb_white_ref =0;
			double nb_black_ref =0;
			
			//variables for comparing to the shoreline (which serves as reference)
			double nb_black_pixel_shoreline =0;
			double nb_correct_shoreline =0;

			// Apply a threshold by color to the satellite image to isolate the dark areas that correspond to the water bodies
			Mat thresholded_image;
			threshold(gray_image, thresholded_image, threshold_val, 255, THRESH_BINARY_INV); 

			// Apply a morphological operation (dilation) to remove noise and improve the quality of the thresholded image
			Mat dilated_image;
			Mat kernel = getStructuringElement(MORPH_RECT, Size(element_size, element_size));
			dilate(thresholded_image, dilated_image, kernel);
			//imshow("dilated Image", dilated_image);
			
			// Combine the satellite image filtered with highpass and the one thresholded by color to get more complete analysis (but not used later in the code)

			Mat combined;
			bitwise_and (dilated_image_filterd, dilated_image, combined);
			//imshow("combined Image", combined);
			//waitKey(0);
			
			// Select which type of processing to analyze (color / color dilated / color dilated and frequency combined)
			Mat image_to_analyze = dilated_image; // dilated_image () OR thresholded_image OR combined (ie dilated image and dilated high pass)
		 
			    
			for(int y=0;y<image_to_analyze.rows;y++) 
			{
				for(int x=0;x<image_to_analyze.cols;x++) 
				{
					nb_pixel_thresholded_image++;
					int pixel_val_thresholded = (int)image_to_analyze.at<uchar>(y, x); 
					
					// comparing to overall map 
					
					int pixel_val_map = (int)thresholded_map.at<uchar>(y, x); //EXTREMELY IMPORTANT TO HAVE (y,x) AND NOT THE OTHER WAY 
					
					// count the numbers of reference pixels
					if (pixel_val_map == 0) nb_black_ref++;
					if (pixel_val_map == 255) nb_white_ref++;
					
					// count the number of correct pixels
					if (pixel_val_map == pixel_val_thresholded) nb_correct_pixels++;
					
					// Count the number of incorrect pixels and classifies them
					if (!(pixel_val_map == pixel_val_thresholded)) 
					{

						nb_wrong_pixel++;
						if (pixel_val_thresholded == 255 && pixel_val_map == 0) nb_false_path++;
						else if (pixel_val_thresholded == 0 && pixel_val_map == 255) nb_false_obstacle++;
						else wtf++;

					}
					 
					
					
					
					// comparing to the shoreline
					
					int pixel_val_shoreline = (int)reversed_shoreline.at<uchar>(y, x);
					if (pixel_val_shoreline == 0)
					{
						nb_black_pixel_shoreline++;
						
						if (pixel_val_thresholded ==0) nb_correct_shoreline++;
					}
					
					
					
					
					
				}

			} 


			

			//imwrite("thresholded.png", thresholded_image);

			if (threshold_val==60 && element_size==1) 
			{
			imshow("Image to analyze", image_to_analyze);
			//imshow("Thresholded Image", thresholded_image);
			//imshow("Dilated Image", dilated_image);
			waitKey(0);
			}

			//cout << "total nb_pixel_thresholded_image	" << nb_pixel_thresholded_image << endl;
			//cout << "threshold_val	" << threshold_val << endl;
			//cout << "element_size	" << element_size << endl;
			//cout << "nb_correct_pixels	" << nb_correct_pixels << "(" << nb_correct_pixels/nb_pixel_thresholded_image*100  << "%)" <<endl;
			//cout << "nb_wrong_pixel	" << nb_wrong_pixel << "(" << nb_wrong_pixel/nb_pixel_thresholded_image*100  << "%)" <<endl;
			//cout << "nb_false_obstacle	" << nb_false_obstacle<< "(" << nb_false_obstacle/nb_wrong_pixel*100  << "%)"  << endl;
			//cout << "nb_false_path	" << nb_false_path<< "(" << nb_false_path/nb_wrong_pixel*100  << "%)"  << endl;
			//cout << "wdtff	" << wtf <<"(" << wtf/nb_wrong_pixel*100  << "%)" << endl;
			
			myfile << threshold_val << "," << element_size << "," << nb_pixel_thresholded_image << "," <<  nb_correct_pixels << "," <<  nb_wrong_pixel << "," <<  nb_false_path << "," <<  nb_false_obstacle << "," << nb_black_pixel_shoreline << "," << nb_correct_shoreline <<"," << nb_correct_shoreline/nb_black_pixel_shoreline*100 << ","<< nb_white_ref << "," << nb_black_ref << "\n";

			//waitKey(0);

		}
	}

	myfile.close();

	return 0;

}

