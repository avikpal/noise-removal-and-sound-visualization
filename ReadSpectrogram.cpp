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
 
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#define THRES_MOD 10

using namespace cv;
using namespace std;

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void localMaxima(cv::Mat src,cv::Mat &dst,int squareSize)
{
if (squareSize==0)
{
    dst = src.clone();
    return;
}

Mat m0;
dst = src.clone();
Point maxLoc(0,0);

//1.Be sure to have at least 3x3 for at least looking at 1 pixel close neighbours
//  Also the window must be <odd>x<odd>
//SANITYCHECK(squareSize,3,1);
int sqrCenter = (squareSize-1)/2;

//2.Create the localWindow mask to get things done faster
//  When we find a local maxima we will multiply the subwindow with this MASK
//  So that we will not search for those 0 values again and again

Mat localWindowMask = Mat::zeros(Size(squareSize,squareSize),CV_8U);

//Mat localWindowMask = Mat::zeros(Size(squareSize,squareSize),CV_8UC3);//boolean

localWindowMask.at<unsigned char>(sqrCenter,sqrCenter)=1;

//3.Find the threshold value to threshold the image
    //this function here returns the peak of histogram of picture
    //the picture is a thresholded picture it will have a lot of zero values in it
    //so that the second boolean variable says :
    //  (boolean) ? "return peak even if it is at 0" : "return peak discarding 0"
//int thrshld =  maxUsedValInHistogramData(dst,false);

int thrshld = 75 - THRES_MOD; //say
threshold(dst,m0,thrshld,255,1);

imwrite("finalThresholdedImage.png",m0);

//4.Now delete all thresholded values from picture
dst = dst.mul(m0);

//put the src in the middle of the big array
for (int row=sqrCenter;row<dst.size().height-sqrCenter;row++)
    for (int col=sqrCenter;col<dst.size().width-sqrCenter;col++)
    {
        //1.if the value is zero it can not be a local maxima
        if (dst.at<unsigned char>(row,col)==0)
            continue;
        //2.the value at (row,col) is not 0 so it can be a local maxima point
        m0 =  dst.colRange(col-sqrCenter,col+sqrCenter+1).rowRange(row-sqrCenter,row+sqrCenter+1);
        minMaxLoc(m0,NULL,NULL,NULL,&maxLoc);
        //if the maximum location of this subWindow is at center
        //it means we found the local maxima
        //so we should delete the surrounding values which lies in the subWindow area
        //hence we will not try to find if a point is at localMaxima when already found a neighbour was
        if ((maxLoc.x==sqrCenter)&&(maxLoc.y==sqrCenter))
        {
            //m0 = m0.mul(localWindowMask);
                            //we can skip the values that we already made 0 by the above function
            col+=sqrCenter;
        }
    }
}

int main(int argc, char* argv[]){

	Mat IdealAudioSpectrogram; // TODO PronuncedAudioSpectrogram;
	//for loading spectrogram images
	IdealAudioSpectrogram = imread("finalimageGrayscale.png");
	//string ty = type2str(IdealAudioSpectrogram.type());

	//cout<<ty.c_str()<<endl;
	//load the thresholded spectrogram of already recorded audio

	//TODO
	//PronuncedAudioSpectrogram = imread(argv[2]);

	//load the spectrogram of pronunced audio

	//test with values
	vector<Mat>grayPlanes;
	split(IdealAudioSpectrogram,grayPlanes);
	Mat maxoutput;
	localMaxima(IdealAudioSpectrogram, grayPlanes[2],3);
	imwrite("maximage.png", maxoutput);


}
