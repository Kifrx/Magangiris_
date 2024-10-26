#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

//zikri
//deteksi bola kasti

int main() {

    Scalar lower(35, 30, 80);
    Scalar upper(75, 255, 255);
    
    VideoCapture videoCapture(0); 
    if (!videoCapture.isOpened()) {
        cerr << "Error: Could not open video capture." << endl;
        return -1;
    }

    while (true) {
      Mat frame;
        if (!videoCapture.read(frame)) {
            cerr << "Error: Could not read frame." << endl;
            break;
        }

        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        Mat threshold_hsv;
        inRange(hsv, lower, upper, threshold_hsv);
        
        Mat res;
        bitwise_and(frame, frame, res, threshold_hsv);

        Mat frame_grey;
        cvtColor(res, frame_grey, COLOR_BGR2GRAY);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(frame_grey, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);


        for (const auto& cnt : contours) {
            if (cnt.size() > 0) {
                Point2f center;
                float radius;
                minEnclosingCircle(cnt, center, radius);

                if (radius > 120) {
                    circle(frame, center, static_cast<int>(radius), Scalar(255, 0, 255), 2);

       
                    double dist = 159.59665197690032 - 
                                  6.1134615435764985 * radius + 
                                  0.095445319642689888 * pow(radius, 2) - 
                                  0.00071963853187203511 * pow(radius, 3) + 
                                  0.0000027869812247284325 * pow(radius, 4) - 
                                  0.0000000053337689624525208 * pow(radius, 5) + 
                                  0.0000000000039905365509557071 * pow(radius, 6);
                    
             
                    ostringstream textStream;
                    textStream << "Distance: " << dist << " cm";
                    string text = textStream.str();

                    putText(frame, text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);

                }
            }
        }

        imshow("Original", frame);

        if (waitKey(1) == 'q') {
            break;
        }
    }
    videoCapture.release();
    destroyAllWindows();

    return 0;
}
