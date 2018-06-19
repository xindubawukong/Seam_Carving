


#include <iostream>
#include "SeamCarver.h"

using namespace std;



int main() {

    Mat img=imread("/Users/xindubawukong/Desktop/thu_22/计算机图形学基础/zuoye3/Seam_Carving/1.jpg");
    if (!img.data) {
        cout<<"Invalid data!"<<endl;
        img.release();
        return 0;
    }
    //imshow("original image",img);

    SeamCarver* seam_carver=new SeamCarver(img);
    //seam_carver->show_image("original image");
    seam_carver->show_energy("energy");
    //vector<int> seam=seam_carver->get_vertical_seam();seam_carver->show_vertical_seam(seam);
    for (int i=img.cols*0.2;i>0;i--) {
        if (i%10==0) cout<<i<<endl;
        vector<int> seam=seam_carver->get_vertical_seam();
        seam_carver->remove_vertical_seam(seam);
    }
    for (int i=img.rows*0.2;i>0;i--) {
        if (i%10==0) cout<<i<<endl;
        vector<int> seam=seam_carver->get_horizontal_seam();
        seam_carver->remove_horizontal_seam(seam);
    }
    seam_carver->show_image("carved image");
    seam_carver->show_record("record image");

    waitKey();

    img.release();

    return 0;

}