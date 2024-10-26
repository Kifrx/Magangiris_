#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap("/home/kifrixs/Downloads/Video asli.avi");
    if (!cap.isOpened()) {
        cerr << "Error: Cannot open video." << endl;
        return -1;
    }

    Mat frame;
    Point2f robot_position(0, 0); 
    Point2f initial_ball_position(0, 0);
    Point2f center(320, 240); 
    bool first_detection = true; 
    while (cap.read(frame)) {
        if (frame.empty()) {
            break;
        }

        Mat hsv_frame;
        cvtColor(frame, hsv_frame, COLOR_BGR2HSV);
        Mat mask;
        inRange(hsv_frame, Scalar(0, 150, 150), Scalar(15, 255, 255), mask);

        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        if (!contours.empty()) {
         
            size_t largest_index = 0;
            double largest_area = 0;

            for (size_t i = 0; i < contours.size(); i++) {
                double area = contourArea(contours[i]);
                if (area > largest_area) {
                    largest_area = area;
                    largest_index = i;
                }
            }

            vector<Point> largest_contour = contours[largest_index];

    
            Moments ball_moments = moments(largest_contour);
            Point2f ball_center(ball_moments.m10 / ball_moments.m00, ball_moments.m01 / ball_moments.m00);
            float radius = sqrt(largest_area / CV_PI); 

           
            circle(frame, ball_center, radius, Scalar(255, 0, 0), 2); 
            circle(frame, ball_center, 5, Scalar(0, 255, 0), -1); 

           
            if (first_detection) {
                initial_ball_position = ball_center; 
                first_detection = false; 
            }

            Point2f rel_position = ball_center - initial_ball_position;
            robot_position = Point2f(rel_position.x * 10, rel_position.y * 10); 

            putText(frame, "Robot Position: (" + to_string(static_cast<int>(robot_position.x)) + ", " + 
                    to_string(static_cast<int>(robot_position.y)) + " cm)", 
                    Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(10, 0, 255), 1);
            putText(frame, "Ball Position: (" + to_string((int)ball_center.x) + ", " + 
                    to_string((int)ball_center.y) + ")", 
                    Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(10, 0, 255), 1);
        } else {
            putText(frame, "No Ball Detected", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(10, 0, 255), 1);
        }

        imshow("Video", frame);

        if (waitKey(30) == 'q') {
            break;
        }
    }
    cap.release();
    destroyAllWindows();

    return 0;
}
