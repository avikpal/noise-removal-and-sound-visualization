/*
 *  Copyright 2014-2015  Avik Pal <avikpal.me@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
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
