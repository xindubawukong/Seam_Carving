


#include "SeamCarver.h"



#define inf 1e30



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
    E=Mat(img.rows,img.cols,CV_32S);
//    for (int i=1;i<img.rows-1;i++)
//        for (int j=1;j<img.cols-1;j++) {
//            Vec3b l=img.at<Vec3b>(i,j-1);
//            Vec3b r=img.at<Vec3b>(i,j+1);
//            Vec3b u=img.at<Vec3b>(i-1,j);
//            Vec3b d=img.at<Vec3b>(i+1,j);
//            int res=0;
//            for (int k=0;k<3;k++) {
//                res+=(l[k]-r[k])*(l[k]-r[k]);
//                res+=(u[k]-d[k])*(u[k]-d[k]);
//            }
//            E.at<int>(i,j)=res;
//        }
    for (int i = 1; i < img.rows-1; ++i) {
        uchar* prev = img.ptr<uchar>(i-1);	//Pointer to previous row
        uchar* curr = img.ptr<uchar>(i);		//Pointer to current row
        uchar* next = img.ptr<uchar>(i+1);	//Pointer to next row

        for (int j = 1; j < img.cols-1; ++j) {
            int val = 0;
            //Energy along the x-axis
            val += (prev[3*j]-curr[3*j]) * (prev[3*j]-curr[3*j]);
            val += (next[3*j]-curr[3*j]) * (next[3*j]-curr[3*j]);
            val += (prev[3*j+1]-curr[3*j+1]) * (prev[3*j+1]-curr[3*j+1]);
            val += (next[3*j+1]-curr[3*j+1]) * (next[3*j+1]-curr[3*j+1]);
            val += (prev[3*j+2]-curr[3*j+2]) * (prev[3*j+2]-curr[3*j+2]);
            val += (next[3*j+2]-curr[3*j+2]) * (next[3*j+2]-curr[3*j+2]);

            //Energy along the y-axis
            val += (curr[3*j+3]-curr[3*j]) * (curr[3*j+3]-curr[3*j]);
            val += (curr[3*j]-curr[3*j-3]) * (curr[3*j]-curr[3*j-3]);
            val += (curr[3*j+4]-curr[3*j+1]) * (curr[3*j+4]-curr[3*j+1]);
            val += (curr[3*j+1]-curr[3*j-2]) * (curr[3*j+1]-curr[3*j-2]);
            val += (curr[3*j+5]-curr[3*j+2]) * (curr[3*j+5]-curr[3*j+2]);
            val += (curr[3*j+2]-curr[3*j-1]) * (curr[3*j+2]-curr[3*j-1]);
            E.at<int>(i, j) = val;
        }
    }
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
    double minf=inf;
    int minj=0;
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