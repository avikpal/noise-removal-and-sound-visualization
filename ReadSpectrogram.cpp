#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void localMaxima(cv::Mat src, cv::Mat &dst, int squareSize){
	if(squareSize==0){
		dst = src.clone();
		return;
	}

	Mat temp;
	dst = src.clone();
	Point maxLoc(0,0);

	int sqrCentre = (squareSize-1)/2 ;

	//Create the localWindow mask to do things faster
	//When we find a global maxima we will multiply the subwindow with the following mask
	//so as we don't need to search for those values again and again

	Mat localWindowMask = Mat::zeros(Size(squareSize,squareSize),CV_8U); //boolean
	localWindowMask.at<unsigned char>(sqrCentre,sqrCentre)=1;

	//Finding a threshold value to threshold the images
	//this function here returns the peak of histogram of picture
	//the picture is a threshold picture it will have a lot of zero values in
	//so the second boolean variable:
	// (boolean) ? "return peak even if it is at 0" : "return peak discarding 0"

	int threshld= maxUsedValInHistogramData(dst, false);
	threshold(dst,temp,threshld,1,THRESH_BINARY);

	dst = dst.mul(temp);

	for(int row=sqrCentre; row<dst.size().height-sqrCentre; row++){
		for(int col=sqrCentre; col<dst.size().width-sqrCentre; col++){

			if(dst.at<unsigned char>(row, col)==0)
				continue;
			temp = dst.colRange(col-sqrCentre, col+sqrCentre+1).rowRange(row-sqrCentre,row+sqrCentre+1);
			minMaxLoc(temp,NULL,NULL,NULL,&maxLoc);

			//if the maximum location of this subwindow is at center
			//it means we have the local Maxima
			//we should delete surrounding values in the subwindow
			//hence we will not try to find if a point is at localMaxima when already found

			if((maxLoc.x==sqrCentre) && (maxLoc.y==sqrCentre))
			{
				temp=temp.mul(localWindowMask);
				col+=sqrCentre;
			}
		}
	}
}

int main(int argc, char* argv[]){

	Mat IdealAudioSpectrogram, PronuncedAudioSpectrogram;
	//for loading spectrogram images
	IdealAudioSpectrogram = imread(argv[1]);
	//load the spectrogram of already recorded audio

	//TODO
	//PronuncedAudioSpectrogram = imread(argv[2]);

	//load the spectrogram of pronunced audio

	//test with values
	Mat maxoutput;
	localMaxima(IdealAudioSpectrogram, &maxoutput,3);
	imwrite("maximage.png", maxoutput);


}