


#include "SeamCarver.h"



#define inf 999999999



SeamCarver::SeamCarver(Mat _img) {
    _img.copyTo(img);
    _img.copyTo(img0);
    for (int i=0;i<img.rows;i++)
        for (int j=0;j<img.cols;j++)
            record[i][j]=make_pair(i,j);
    compute_energy();
    cout<<"rows="<<img.rows<<endl<<"cols="<<img.cols<<endl;
}



SeamCarver::~SeamCarver() {
    img.release();
    E.release();
}



void SeamCarver::compute_energy() {
    Mat img_gray;
    cvtColor(img,img_gray,CV_BGR2GRAY);
    Mat E1,E2;
    Sobel(img_gray,E1,-1,0,1);
    Sobel(img_gray,E2,-1,1,0);
    E=Mat(img.rows,img.cols,CV_8U);
    for (int i=0;i<img.rows;i++)
        for (int j=0;j<img.cols;j++)
            E.at<uchar>(i,j)=((int)E1.at<uchar>(i,j)+(int)E2.at<uchar>(i,j))/2;
    E1.release();
    E2.release();
}



vector<int> SeamCarver::get_vertical_seam() {
    for (int i=0;i<img.rows;i++)
        for (int j=0;j<img.cols;j++)
            if (i==0) f[i][j]=getE(i,j);
            else f[i][j]=inf;
    for (int i=0;i<img.rows-1;i++)
        for (int j=1;j<img.cols-1;j++) {
            if (f[i+1][j-1]>f[i][j]+getE(i+1,j-1)) {
                f[i+1][j-1]=f[i][j]+getE(i+1,j-1);
                from[i+1][j-1]=j;
            }
            if (f[i+1][j]>f[i][j]+getE(i+1,j)) {
                f[i+1][j]=f[i][j]+getE(i+1,j);
                from[i+1][j]=j;
            }
            if (f[i+1][j+1]>f[i][j]+getE(i+1,j+1)) {
                f[i+1][j+1]=f[i][j]+getE(i+1,j+1);
                from[i+1][j+1]=j;
            }
        }
    int minf=inf,minj=0;
    for (int j=1;j<img.cols-1;j++)
        if (f[img.rows-1][j]<minf) {
            minf=f[img.rows-1][j];
            minj=j;
        }
    vector<int> seam(img.rows);
    for (int i=img.rows-1,j=minj;i>=0;i--) {
        seam[i]=j;
        j=from[i][j];
    }
    return seam;
}



vector<int> SeamCarver::get_horizontal_seam() {
    transpose(img,img);
    transpose(E,E);
    vector<int> seam=get_vertical_seam();
    transpose(img,img);
    transpose(E,E);
    return seam;
}



void SeamCarver::remove_vertical_seam(vector<int> seam) {
    Mat tmp(img.rows,img.cols-1,CV_8UC3);
    for (int i=0;i<img.rows;i++) {
        for (int j=0;j<seam[i];j++)
            tmp.at<Vec3b>(i,j)=img.at<Vec3b>(i,j);
        for (int j=seam[i];j<img.cols-1;j++) {
            tmp.at<Vec3b>(i,j)=img.at<Vec3b>(i,j+1);
            record[i][j]=record[i][j+1];
        }
    }
    tmp.copyTo(img);
    tmp.release();
    compute_energy();
}



void SeamCarver::remove_horizontal_seam(vector<int> seam) {
    Mat tmp(img.rows-1,img.cols,CV_8UC3);
    for (int j=0;j<img.cols;j++) {
        for (int i=0;i<seam[j];i++)
            tmp.at<Vec3b>(i,j)=img.at<Vec3b>(i,j);
        for (int i=seam[j];i<img.rows-1;i++) {
            tmp.at<Vec3b>(i,j)=img.at<Vec3b>(i+1,j);
            record[i][j]=record[i+1][j];
        }
    }
    tmp.copyTo(img);
    tmp.release();
    compute_energy();
}



void SeamCarver::show_image(string s) {
    imshow(s,img);
}



void SeamCarver::show_energy(string s) {
    imshow(s,E);
}



void SeamCarver::show_record(string s) {
    Mat tmp(img0.rows,img0.cols,CV_8UC3);
    for (int i=0;i<tmp.rows;i++)
        for (int j=0;j<tmp.cols;j++)
            tmp.at<Vec3b>(i,j)=Vec3b(0,0,255);
    for (int i=0;i<img.rows;i++)
        for (int j=0;j<img.cols;j++)
            tmp.at<Vec3b>(record[i][j].first,record[i][j].second)=img.at<Vec3b>(i,j);
    imshow(s,tmp);
    tmp.release();
}



void SeamCarver::show_vertical_seam(vector<int> seam) {
    Mat tmp;
    img.copyTo(tmp);
    for (int i=0;i<img.rows;i++)
        tmp.at<Vec3b>(i,seam[i])=Vec3b(0,0,255);
    imshow("seam",tmp);
    tmp.release();
}



void SeamCarver::show_horizontal_seam(vector<int> seam) {
    Mat tmp;
    img.copyTo(tmp);
    for (int j=0;j<img.cols;j++)
        tmp.at<Vec3b>(seam[j],j)=Vec3b(0,0,255);
    imshow("seam",tmp);
    tmp.release();
}