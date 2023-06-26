

//####################################################

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{


    // Load map (google map)
    Mat map = imread("map.png");
    Mat gray_map;
    cvtColor(map, gray_map, COLOR_BGR2GRAY);
    imshow("Grayscale map", gray_map);
    Mat thresholded_map;
    threshold(gray_map, thresholded_map, 200, 255, THRESH_BINARY_INV);
    imshow("thresholded_map", thresholded_map);
    
    
    int pixel_counter = 0;
    
     for(int y=0;y<gray_map.rows;y++)
    {
        for(int x=0;x<gray_map.cols;x++)
        {
          pixel_counter++;
        
        }
    }
        
    cout << "total number of pixels in map:" << pixel_counter <<endl; 
    
    
/*
 // Define color range BGR
    
    Scalar lower(225, 167, 124);  //defined with 20.7 threshold in GIMP
    Scalar upper(255, 219, 190);

    // Create binary mask for color range
    Mat mask;
    inRange(map, lower, upper, mask);

    // Count number of pixels in color range
    int count = countNonZero(mask);
    std::cout << "Number of pixels around (156, 192, 249): " << count << std::endl;


    // Save output
    imwrite("blue_bag_mask.png", mask);

    // Display various images to see the steps
    namedWindow("mask", WINDOW_AUTOSIZE);
    imshow("mask", mask);   //*/




    // Load the satellite image
    Mat image = imread("satRGB.png");
    
        // Convert the image to grayscale
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);
    
    
    
/////////////////////////start of copied part
/*for (int threshold_val = 55; threshold_val < 60; threshold_val++)
{
for (int element_size = 1; element_size< 3; element_size ++)
{


int nb_false_path = 0;
int nb_false_obstacle = 0;
int nb_wrong_pixel = 0;
int nb_pixel_thresholded_image =0;
int wtf = 0;

    // Apply a threshold to the image to isolate the dark areas that correspond to the water bodies
    Mat thresholded_image;
  threshold(gray_image, thresholded_image, threshold_val, 255, THRESH_BINARY_INV); //this line is for black obstacles tsting. remove _INV for water area (since we want dark zones to be not obstacles)
 // threshold(gray_image, thresholded_image, 55, 255, THRESH_BINARY_INV); //remove _INV for water area (since we want dark zones to be not obstacles)

    // Apply a morphological operation (erosion or dilation) to remove noise and improve the quality of the thresholded image
    Mat eroded_image;
    Mat dilated_image;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(element_size, element_size));
    erode(thresholded_image, eroded_image, kernel);
    dilate(thresholded_image, dilated_image, kernel);

    // Find the contours of the thresholded areas
    //vector<vector<Point>> contours;
    
    //findContours(dilated_image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
     for(int y=0;y<thresholded_image.rows;y++)
    {
        for(int x=0;x<thresholded_image.cols;x++)
        {
	   int thresholded_image_blue = thresholded_image.at<Vec3b>(x, y)[0];//getting the pixel values//
	   int thresholded_image_green = thresholded_image.at<Vec3b>(x, y)[1];//getting the pixel values//
	   int thresholded_image_red = thresholded_image.at<Vec3b>(x, y)[2];//getting the pixel values//
	   
	   int mask_blue = mask.at<Vec3b>(x, y)[0];//getting the pixel values//
	   int mask_green = mask.at<Vec3b>(x, y)[1];//getting the pixel values//
	   int mask_red = mask.at<Vec3b>(x, y)[2];//getting the pixel values//
	   
	  /* if (thresholded_image_blue != mask_blue) 
	   {
		   nb_wrong_pixel++;
		   if (thresholded_image_blue >= 125 && mask_blue <= 125) nb_false_path++;
		   else if (thresholded_image_blue <= 125 && mask_blue >= 125) nb_false_obstacle++;
		   else 
		   {
		   cout << "thresholded_image_blue	" << thresholded_image_blue << "	mask_blue	" <<mask_blue<<endl;
		    cout << "thresholded_image_green	" << thresholded_image_green << "	mask_green	" <<mask_green<<endl;
		     cout << "thresholded_image_red	" << thresholded_image_red << "		mask_red	" <<mask_red<<endl;
		     cout << "wtf" << endl;
		     wtf++;
		   
		   }
	
	   } //*/
	   
/*	   if (thresholded_image_blue != mask_blue) 
	   {
		   nb_wrong_pixel++;
		   if (thresholded_image_blue >= 125 && mask_blue <= 125) nb_false_path++;
		   else if (thresholded_image_blue <= 125 && mask_blue >= 125) nb_false_obstacle++;
		   else 
		   {
		   cout << "thresholded_image_blue	" << thresholded_image_blue << "	mask_blue	" <<mask_blue<<endl;
		    cout << "thresholded_image_green	" << thresholded_image_green << "	mask_green	" <<mask_green<<endl;
		     cout << "thresholded_image_red	" << thresholded_image_red << "		mask_red	" <<mask_red<<endl;
		     cout << "wtf" << endl;
		     wtf++;
		   
		   }
	
	   }
           
           nb_pixel_thresholded_image++;
        }
        
    } 
    

    // Draw the found contours on the original image
    //Mat drawn_image = image.clone();
    //drawContours(drawn_image, contours, -1, Scalar(0, 255, 0), FILLED);

    // Display the result
    //imshow("Original Image", image);
    //imshow("Grayscale Image", gray_image);
    imshow("Thresholded Image", thresholded_image);
    //imshow("Eroded Image", eroded_image);
    //imshow("Dilated Image", dilated_image);
   // imshow("Contours", drawn_image);
   
   imwrite("thresholded.png", thresholded_image);
   
   
   cout << "total nb_pixel_thresholded_image	" << nb_pixel_thresholded_image << endl;
   cout << "threshold_val	" << threshold_val << endl;
   cout << "element_size	" << element_size << endl;
   cout << "nb_wrong_pixel	" << nb_wrong_pixel << endl;
   cout << "nb_false_obstacle	" << nb_false_obstacle << endl;
   cout << "nb_false_path	" << nb_false_path << endl;
   cout << "wtf	" << wtf << endl;
   
    waitKey(0);
    
}
}
*/


///////////////////copied part


for (int threshold_val = 55; threshold_val < 60; threshold_val++)
{
	for (int element_size = 1; element_size< 3; element_size ++)
	{


	int nb_false_path = 0;
	int nb_false_obstacle = 0;
	int nb_wrong_pixel = 0;
	int nb_pixel_thresholded_image =0;
	int wtf = 0;

	    // Apply a threshold to the image to isolate the dark areas that correspond to the water bodies
	    Mat thresholded_image;
	  threshold(gray_image, thresholded_image, threshold_val, 255, THRESH_BINARY_INV); //this line is for black obstacles tsting. remove _INV for water area (
	 // threshold(gray_image, thresholded_image, 55, 255, THRESH_BINARY_INV); //remove _INV for water area (since we want dark zones to be not obstacles)

	    // Apply a morphological operation (erosion or dilation) to remove noise and improve the quality of the thresholded image
	    Mat eroded_image;
	    Mat dilated_image;
	    Mat kernel = getStructuringElement(MORPH_RECT, Size(element_size, element_size));
	    erode(thresholded_image, eroded_image, kernel);
	    dilate(thresholded_image, dilated_image, kernel);

	    // Find the contours of the thresholded areas
	    //vector<vector<Point>> contours;
	    
	    //findContours(dilated_image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	    
	     for(int y=0;y<thresholded_image.rows;y++)
	    {
		for(int x=0;x<thresholded_image.cols;x++)
		{
		//cout << "crash point A" <<endl;
		int pixel_val_map = (int)thresholded_map.at<uchar>(x, y);
		//cout << "crash point B" <<endl;
		int pixel_val_thresholded = (int)thresholded_image.at<uchar>(x, y);
		//cout << "crash point C" <<endl;
		   
		   cout << "pixel_val_map	" << pixel_val_map << "	pixel_val_thresholded	" <<pixel_val_thresholded << endl;
		   
		  if (!(pixel_val_map >= pixel_val_thresholded-50 && pixel_val_map <= pixel_val_thresholded+50)) 
		   {
		  // cout << "crash point D" <<endl;
			   nb_wrong_pixel++;
			   if (pixel_val_thresholded >= 125 && pixel_val_map <= 125) nb_false_path++;
			   else if (pixel_val_thresholded <= 125 && pixel_val_map >= 125) nb_false_obstacle++;
			   else 
			   {
			   
			   //  cout << "wtf" << endl;
			     wtf++;
			   
			   }
		
		   } 
		   

		   
		   nb_pixel_thresholded_image++;
		}
		
	    } 
	    

	    // Draw the found contours on the original image
	    //Mat drawn_image = image.clone();
	    //drawContours(drawn_image, contours, -1, Scalar(0, 255, 0), FILLED);

	    // Display the result
	    //imshow("Original Image", image);
	    //imshow("Grayscale Image", gray_image);
	    imshow("Thresholded Image", thresholded_image);
	    //imshow("Eroded Image", eroded_image);
	    //imshow("Dilated Image", dilated_image);
	   // imshow("Contours", drawn_image);
	   
	   imwrite("thresholded.png", thresholded_image);
	   
	   
	   cout << "total nb_pixel_thresholded_image	" << nb_pixel_thresholded_image << endl;
	   cout << "threshold_val	" << threshold_val << endl;
	   cout << "element_size	" << element_size << endl;
	   cout << "nb_wrong_pixel	" << nb_wrong_pixel << endl;
	   cout << "nb_false_obstacle	" << nb_false_obstacle << endl;
	   cout << "nb_false_path	" << nb_false_path << endl;
	   cout << "wtf	" << wtf << endl;
	   
	    waitKey(0);
	    
	}
}



return 0;

}

