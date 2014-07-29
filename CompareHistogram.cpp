//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/**
 * @function main
 */
int main( int argc, char** argv )
{
    Mat src, hsv_src;
    Mat distorted, hsv_distorted;
    Mat intended, hsv_intened;

  if (argc == 3){

    src = imread( argv[1], 1 );
    distorted = imread( argv[2], 1 );
    //intended = imread( argv[3], 1 );

    // Convert to HSV
    cvtColor( src, hsv_src, COLOR_BGR2HSV );
    cvtColor( distorted, hsv_distorted, COLOR_BGR2HSV );
    //cvtColor( intended, hsv_intened, COLOR_BGR2HSV );



    // Using 50 bins for hue and 60 for saturation
    int h_bins = 50; int s_bins = 60;
    int histSize[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float* ranges[] = { h_ranges, s_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };


    // Histograms
    MatND hist_src;
    MatND hist_distorted;
    //MatND hist_intended;

    // Calculate the histograms for the HSV images
    calcHist( &hsv_src, 1, channels, Mat(), hist_src, 2, histSize, ranges, true, false );
    normalize( hist_src, hist_src, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_distorted, 1, channels, Mat(), hist_distorted, 2, histSize, ranges, true, false );
    normalize( hist_distorted, hist_distorted, 0, 1, NORM_MINMAX, -1, Mat() );


    // Apply the histogram comparison methods
	// Methods represnt comparison crtique as
	// Method[0] => Correlation
	// Method[1] => Chi-square
	// Method[2] => intersection
	// Method[3] => Bhattacharya distance
    for( int i = 0; i < 4; i++ )
    {
        int compare_method = i;
        double src_distorted = compareHist( hist_src, hist_distorted, compare_method );

        printf( " Method [%d]  : %f  \n", i,src_distorted);
    }

    printf( "Done \n" );

    return 0;
  }
}
