/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int, int> ul, int w, int h, RGBAPixel a, double v)
        : upLeft(ul), width(w), height(h), avg(a), var(v), NW(NULL), NE(NULL), SE(NULL), SW(NULL) {}

// Second Node constructor, given
SQtree::Node::Node(stats &s, pair<int, int> ul, int w, int h)
        : upLeft(ul), width(w), height(h), NW(NULL), NE(NULL), SE(NULL), SW(NULL) {
    avg = s.getAvg(ul, w, h);
    var = s.getVar(ul, w, h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
    clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree &other) {
    copy(other);
}

// SQtree assignment operator, given.
SQtree &SQtree::operator=(const SQtree &rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG &imIn, double tol) {
    stats s(imIn);

    pair<int, int> rootUL(0, 0);

    root = buildTree(s, rootUL, (int) imIn.width(), (int) imIn.height(), tol);


}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node *SQtree::buildTree(stats &s, pair<int, int> &ul,
                                int w, int h, double tol) {
    // Your code here.
    if (w == 0 || h == 0) {
        return NULL;
    }


    if (w == 1 && h == 1) {
        return new Node(ul, w, h, s.getAvg(ul, w, h), 0);
    }
    if (s.getVar(ul, w, h) <= tol) {
        return new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));
    }
    double min;
    min = -1;
    pair<int, int> xy = make_pair(0, 0);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (x != 0 || y != 0) {
                double NE = 0;
                double NW = 0;
                double SW = 0;
                double SE = 0;

                if(x != 0 && y != 0)
                NW = s.getVar(ul, x, y);

                if(y!= 0)
                NE = s.getVar({ul.first + x, ul.second}, w - x, y);

                if(x!=0)
                SW = s.getVar({ul.first, ul.second + y}, x, h - y);

                SE = s.getVar({ul.first + x, ul.second + y}, w - x, h - y);

                // cout << "ulL:  " << ul.first <<"ulR:  " << ul.second << endl;
                // cout << "w:  " << w <<"h:  " << h << endl;
                // cout << "x:  " << x <<"y:  " << y << endl;
                // cout << "NE:  " << NE << endl;
                // cout << "NW:  " << NW << endl;
                // cout << "SW:  " << SW << endl;
                // cout << "SE:  " << SE << endl;

                double max = NE;
                if (max <= NW)
                    max = NW;
                if (max <= SW)
                    max = SW;
                if (max <= SE)
                    max = SE;

                if (min >= max || min == -1) {
                    min = max;
                    xy.first = x;
                    xy.second = y;
                }
                // cout << "min:  " << min << endl;

            }


        }
    }


    int x = xy.first;
    int y = xy.second;

    Node *newNode = new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));

    // root->NW = buildTree(s, ul, ul.first + x - 1, ul.second + y - 1, tol);
    newNode->NW = buildTree(s, ul, x, y, tol);

    pair<int, int> pair1 = {ul.first + x, ul.second};
    newNode->NE = buildTree(s, pair1, w - x, y, tol);

    pair<int, int> pair2 = {ul.first, ul.second + y};
    newNode->SW = buildTree(s, pair2, x, h - y, tol);

    pair<int, int> pair3 = {ul.first + x, ul.second + y};
    newNode->SE = buildTree(s, pair3, w - x, h - y, tol);

    return newNode;
}


/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
    PNG im(root->width,root->height);
    render(im, root);
    return im;
}

void SQtree::render(PNG& im, Node* root){
    if(!root){
        return;
    }
    if (root->SW == NULL && root->SE == NULL && root->NW == NULL && root->NE == NULL){
        for(int i = 0; i< root->width; i++){
            for(int j = 0; j< root->height; j++){
                RGBAPixel* pix = im.getPixel(root->upLeft.first+i,root->upLeft.second+j);
                *pix = root->avg;
            }
        }
    } 
    render(im,root->NE);
    render(im,root->NW);
    render(im,root->SW);
    render(im,root->SE);
    
}


/**
 * Delete allocated memory.
 */
void SQtree::clear() {
    // Your code here.
    clear(root);
}

void SQtree::clear(Node *&subRoot) {
    if (!subRoot) {
        return;
    }
    clear(subRoot->NW);
    clear(subRoot->NE);
    clear(subRoot->SE);
    clear(subRoot->SW);
    delete subRoot;
    subRoot = NULL;
}


void SQtree::copy(const SQtree &other) {
    root = copy(other.root);

}

SQtree::Node *SQtree::copy(const Node *other) {
    if (!other) {
        return NULL;
    }

//   RGBAPixel temp(other->avg.r, other->avg.g, other->avg.b);
    // cout << other->upLeft.first << other->upLeft.second<< endl;
    // cout << other->width<<endl;
    // cout << other->var << endl;
    Node *copyNode = new Node(make_pair(other->upLeft.first, other->upLeft.second), other->width, other->height,
                              other->avg, other->var);
    copyNode->NW = copy(other->NW);
    copyNode->NE = copy(other->NE);
    copyNode->SE = copy(other->SE);
    copyNode->SW = copy(other->SW);
    return copyNode;

}

int SQtree::size() {
    return getSize(root);
}

// int SQtree::getSize(const Node *subRoot) const {
//    if (subRoot == NULL) {
//        return 0;
//    }

//    int value = 0;

//    if (subRoot->NE && subRoot->NW && subRoot->SE && subRoot->SW) {
//        value++;
//    }

//    value += getSize(subRoot->NW) + getSize(subRoot->NE) + getSize(subRoot->SW) + getSize(subRoot->SE);

//    return value;

// }

int SQtree::getSize(const Node *subRoot) const {
    if (!subRoot) {
        return 0;
    }

    return 1 + getSize(subRoot->NW) + getSize(subRoot->NE) + getSize(subRoot->SW) + getSize(subRoot->SE);
}


