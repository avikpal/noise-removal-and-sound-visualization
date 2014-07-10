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

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat src, dst;
	///Load image
	src = imread(argv[1],1);

	if(!src.data)
		return -1;

	// Seperate the image in 3 places B, G and R
	vector <Mat> bgr_planes;
	split(src, bgr_planes);

	//Establish no of bins
	int histSize = 256;

	//Set ranges for B, G, R
	float range[] = {0,256};
	const float* histRange = { range };

	bool uniform = true, accumulate = false;
	Mat b_hist, g_hist, r_hist;

	//Compute the histograms:
	calcHist(&bgr_planes[0],1,0,Mat(),b_hist,1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1],1,0,Mat(),b_hist,1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2],1,0,Mat(),b_hist,1, &histSize, &histRange, uniform, accumulate);

	//Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double) hist_w/histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0,0,0));

	//Normalize the result to [0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	//Now draw for each channel
	for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  /// Display
  namedWindow("Spectrogram histogram", CV_WINDOW_AUTOSIZE );
  imshow("Spectrogram histogram", histImage );

  waitKey(0);

  return 0;
}
