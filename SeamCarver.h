


#ifndef SEAM_CARVING_SEAMCARVER_H
#define SEAM_CARVING_SEAMCARVER_H



#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



class SeamCarver {

private:

    Mat img,img0;
    Mat E;
    pair<int,int> record[5000][5000];
    int f[5000][5000];
    int from[5000][5000];

public:

    SeamCarver(Mat _img);
    ~SeamCarver();

    int getE(int i,int j) {return E.at<uchar>(i,j);}

    void compute_energy();

    vector<int> get_vertical_seam();
    vector<int> get_horizontal_seam();

    void remove_vertical_seam(vector<int> seam);
    void remove_horizontal_seam(vector<int> seam);

    void show_image(string s);
    void show_energy(string s);
    void show_record(string s);
    void show_vertical_seam(vector<int> seam);
    void show_horizontal_seam(vector<int> seam);

};


#endif //SEAM_CARVING_SEAMCARVER_H
