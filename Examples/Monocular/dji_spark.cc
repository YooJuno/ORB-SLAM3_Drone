#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include "etoeNet.h"

#include <opencv4/opencv2/opencv.hpp>

#include<System.h>


#include <fstream> 
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "termios.h"
#include <sys/signal.h>





#define _WEBCAM_BUILD_


using namespace std;

      

int main(int argc, char **argv)
{
#ifdef _WEBCAM_BUILD_
    if(argc != 4)
    {
        cerr << endl << "argc:" << argc << "!= 3"<< endl;
    }

    cv::VideoCapture cap(argv[3]);



    if (!cap.isOpened()) {
        cerr << endl << "Could not open camera feed." << endl;
        return -1;
    }
    ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);
    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;

    

#ifdef COMPILEDWITHC11
    std::chrono::steady_clock::time_point initT = std::chrono::steady_clock::now();
#else
    std::chrono::monotonic_clock::time_point initT = std::chrono::monotonic_clock::now();
#endif

    // Main loop
    while(true)
    {
        cv::Mat frame;
        cap >> frame;


        if(frame.empty())
            continue;
#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point nowT = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point nowT = std::chrono::monotonic_clock::now();
#endif
        // Pass the image to the SLAM system
        SLAM.TrackMonocular(frame, std::chrono::duration_cast<std::chrono::duration<double> >(nowT-initT).count());

    }
    // Stop all threads
    SLAM.Shutdown();
    
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

#else
    
#endif
    return 0;
}
