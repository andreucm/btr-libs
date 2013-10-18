#include "trackStamped.h"
#include <iostream>

int main(int argc, char *argv[])
{
        CtrackStamped track;
        double ts, t1, t2;
        cv::Point2f pt;
        correspondencePair pair;
        
        track.setId(17);
        track.setVizColor();
        
        ts = 1.1; pt.x = 1; pt.y = 2;
        track.addPoint(ts,pt);
        
        ts = 2.1; pt.x = 2; pt.y = 4;
        track.addPoint(ts,pt);

        ts = 3.1; pt.x = 3; pt.y = 6;
        track.addPoint(ts,pt);

        ts = 4.1; pt.x = 4; pt.y = 8;
        track.addPoint(ts,pt);
        
        track.print();
        
        pt = track.getPoint(2);
        std::cout << "point at(2) is: (" << pt.x << "," << pt.y << ")" << std::endl;
        
        t1 = 1.5; t2 = 4.;
        int res = track.getCorrespondenceInterpolated(t1, t2, pair);
        std::cout << "res: " << res << std::endl;
        if ( res == 0 ) pair.print();
        
        return 0;
}
