#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){
    vector< vector< long >> red(im.width(), vector<long>(im.height()));
    vector< vector< long >> green(im.width(), vector<long>(im.height()));
    vector< vector< long >> blue(im.width(), vector<long>(im.height()));

    // initialized all the channel
    for (int col = 0; col < (int)im.width(); col++) {
        for (int row = 0; row < (int)im.height(); row++) {
            RGBAPixel *curr = im.getPixel(col, row);
            red[col][row] = curr->r;
            green[col][row] = curr->g;
            blue[col][row] = curr->b;

        }
    }

    vector< vector< long >> tempSumR(im.width(), vector<long>(im.height()));
    vector< vector< long >> tempSumG(im.width(), vector<long>(im.height()));
    vector< vector< long >> tempSumB(im.width(), vector<long>(im.height()));
    vector< vector< long >> tempSumsqR(im.width(), vector<long>(im.height()));
    vector< vector< long >> tempSumsqG(im.width(), vector<long>(im.height()));
    vector< vector< long >> tempSumsqB(im.width(), vector<long>(im.height()));

    sumRed = tempSumR;
    sumGreen = tempSumG;
    sumBlue = tempSumB;
    sumsqRed = tempSumsqR;
    sumsqGreen = tempSumsqG;
    sumsqBlue = tempSumsqB;


    sumRed[0][0] = red[0][0];
    sumGreen[0][0] = green[0][0];
    sumBlue[0][0] = blue[0][0];

    sumsqRed[0][0] = red[0][0] * red[0][0];
    sumsqGreen[0][0] = green[0][0] * green[0][0];
    sumsqBlue[0][0] = blue[0][0] * blue[0][0];


    // initialize the first row
    for (unsigned int col = 1; col < im.width(); col++) {
        sumRed[col][0] = sumRed[col-1][0] + red[col][0];
        sumGreen[col][0] = sumGreen[col-1][0] + green[col][0];
        sumBlue[col][0] = sumBlue[col-1][0] + blue[col][0];

        sumsqRed[col][0] = sumsqRed[col-1][0] + red[col][0] * red[col][0];
        sumsqGreen[col][0] = sumsqGreen[col-1][0] + green[col][0] * green[col][0];
        sumsqBlue[col][0] = sumsqBlue[col-1][0] + blue[col][0] * blue[col][0];
    }

    // initialize the first column
    for (unsigned int row = 1; row < im.height(); row++) {
        sumRed[0][row] = sumRed[0][row-1] + red[0][row];
        sumGreen[0][row] = sumGreen[0][row-1] + green[0][row];
        sumBlue[0][row] = sumBlue[0][row-1] + blue[0][row];

        sumsqRed[0][row] = sumsqRed[0][row-1] + red[0][row] * red[0][row];
        sumsqGreen[0][row] = sumsqGreen[0][row-1] + green[0][row] * green[0][row];
        sumsqBlue[0][row] = sumsqBlue[0][row-1] + blue[0][row] * blue[0][row];
    }

    // set all sums and squared sums
    for (unsigned int col = 1; col < im.width(); col++) {
        for (unsigned int row = 1; row < im.height(); row++) {
            sumRed[col][row] = red[col][row] + sumRed[col-1][row] + sumRed[col][row-1] - sumRed[col-1][row-1];
            sumGreen[col][row] = green[col][row] + sumGreen[col-1][row] + sumGreen[col][row-1] - sumGreen[col-1][row-1];
            sumBlue[col][row] = blue[col][row] + sumBlue[col-1][row] + sumBlue[col][row-1] - sumBlue[col-1][row-1];

            sumsqRed[col][row] = red[col][row] * red[col][row] + sumsqRed[col-1][row] + sumsqRed[col][row-1] - sumsqRed[col-1][row-1];
            sumsqGreen[col][row] = green[col][row] * green[col][row] + sumsqGreen[col-1][row] + sumsqGreen[col][row-1] - sumsqGreen[col-1][row-1];
            sumsqBlue[col][row] = blue[col][row] * blue[col][row] + sumsqBlue[col-1][row] + sumsqBlue[col][row-1] - sumsqBlue[col-1][row-1];

        }
    }


}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){

    // cout << "size : " << sumRed.size() << "*" << sumRed[0].size() << " UL: ("<< ul.first << " " << ul.second << ") w:  " << w << " h: " << h << " color:" << channel << endl;


    if (w == 0 || h == 0) {
        return 0;
    }


    long value = 0;
    if (channel == 'r') {
        if (ul.first == 0 && ul.second == 0) {
            value = sumRed[w-1][h-1];
        } else if (ul.first == 0) {
            value = sumRed[w-1][ul.second+h-1] - sumRed[w-1][ul.second-1];
        } else if (ul.second == 0) {
            value = sumRed[ul.first+w-1][h-1] - sumRed[ul.first-1][h-1];
        } else {
            value = sumRed[ul.first + w -1][ul.second + h - 1] - sumRed[ul.first + w - 1][ul.second - 1]
                    - sumRed[ul.first - 1][ul.second + h -1] + sumRed[ul.first - 1][ul.second - 1];
        }
    } else if (channel == 'g') {
        if (ul.first == 0 && ul.second == 0) {
            value = sumGreen[w-1][h-1];
        } else if (ul.first == 0) {
            value = sumGreen[w-1][ul.second+h-1] - sumGreen[w-1][ul.second-1];
        } else if (ul.second == 0) {
            value = sumGreen[ul.first+w-1][h-1] - sumGreen[ul.first-1][h-1];
        } else {
            value = sumGreen[ul.first + w -1][ul.second + h - 1] - sumGreen[ul.first + w - 1][ul.second - 1]
                    - sumGreen[ul.first - 1][ul.second + h -1] + sumGreen[ul.first - 1][ul.second - 1];
        }
    } else if (channel == 'b') {
        if (ul.first == 0 && ul.second == 0) {
            value = sumBlue[w-1][h-1];
        } else if (ul.first == 0) {
            value = sumBlue[w-1][ul.second+h-1] - sumBlue[w-1][ul.second-1];
        } else if (ul.second == 0) {
            value = sumBlue[ul.first+w-1][h-1] - sumBlue[ul.first-1][h-1];
        } else {
            value = sumBlue[ul.first + w -1][ul.second + h - 1] - sumBlue[ul.first + w - 1][ul.second - 1]
                    - sumBlue[ul.first - 1][ul.second + h -1] + sumBlue[ul.first - 1][ul.second - 1];
        }
    }

    return value;

}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){

    if (w == 0 || h == 0) {
        return 0;
    }

    long value = 0;
    if (channel == 'r') {
        if (ul.first == 0 && ul.second == 0) {
            value = sumsqRed[w-1][h-1];
        } else if (ul.first == 0) {
            value = sumsqRed[w-1][ul.second+h-1] - sumsqRed[w-1][ul.second-1];
        } else if (ul.second == 0) {
            value = sumsqRed[ul.first+w-1][h-1] - sumsqRed[ul.first-1][h-1];
        } else {
            value = sumsqRed[ul.first + w -1][ul.second + h - 1] - sumsqRed[ul.first + w - 1][ul.second - 1]
                    - sumsqRed[ul.first - 1][ul.second + h -1] + sumsqRed[ul.first - 1][ul.second - 1];
        }
    } else if (channel == 'g') {
        if (ul.first == 0 && ul.second == 0) {
            value = sumsqGreen[w-1][h-1];
        } else if (ul.first == 0) {
            value = sumsqGreen[w-1][ul.second+h-1] - sumsqGreen[w-1][ul.second-1];
        } else if (ul.second == 0) {
            value = sumsqGreen[ul.first+w-1][h-1] - sumsqGreen[ul.first-1][h-1];
        } else {
            value = sumsqGreen[ul.first + w -1][ul.second + h - 1] - sumsqGreen[ul.first + w - 1][ul.second - 1]
                    - sumsqGreen[ul.first - 1][ul.second + h -1] + sumsqGreen[ul.first - 1][ul.second - 1];
        }
    } else if (channel == 'b') {
        if (ul.first == 0 && ul.second == 0) {
            value = sumsqBlue[w-1][h-1];
        } else if (ul.first == 0) {
            value = sumsqBlue[w-1][ul.second+h-1] - sumsqBlue[w-1][ul.second-1];
        } else if (ul.second == 0) {
            value = sumsqBlue[ul.first+w-1][h-1] - sumsqBlue[ul.first-1][h-1];
        } else {
            value = sumsqBlue[ul.first + w -1][ul.second + h - 1] - sumsqBlue[ul.first + w - 1][ul.second - 1]
                    - sumsqBlue[ul.first - 1][ul.second + h -1] + sumsqBlue[ul.first - 1][ul.second - 1];
        }
    }

    return value;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    double redV;
    double greenV;
    double blueV;
    
    // if(w == 0|| h ==0){
    //     cout<<"w == 0|| h ==0"<<endl;
    // }
    redV = (double)getSumSq('r', ul, w, h) - (double)getSum('r', ul, w, h) * (double)getSum('r', ul, w, h) / (double)(w * h);
    greenV = (double)getSumSq('g', ul, w, h) - (double)getSum('g', ul, w, h) * (double)getSum('g', ul, w, h) / (double)(w * h);
    blueV = (double)getSumSq('b', ul, w, h) - (double)getSum('b', ul, w, h) * (double)getSum('b', ul, w, h) /(double) (w * h);

    return redV + greenV + blueV;

}

RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    long avgRed = getSum('r', ul, w, h) / (w * h);
    long avgGreen = getSum('g', ul, w, h) / (w * h);
    long avgBlue = getSum('b', ul, w, h) / (w * h);

    return RGBAPixel(avgRed, avgGreen, avgBlue);
}
