# Shifty-Quadtree
## About
The inspiration for this project came from an [article](https://mymodernmet.com/algorithm-art-color-study-dimitris-ladopoulos/) about an artist whose work recreates classic portraits with a blocky effect. The exact algorithm he used is not given in the article, but this application creates similar images using a strategy for image representation that underlies common lossy image compression algorithms.

The original image will be represented in memory as a binary tree whose nodes contain information about rectangles. Splits are performed so as to minimize hue variability across the resulting rectangles. The variability measurement used is the *entropy*, which is computed as a function of the distribution of colours in a rectangle. In order to find the colours of any rectangle in a given image, we used dynamic programming to speed up the running time to be constant. 

To achieve an artistic effect we prune, or cut off, parts of the binary tree. A parameter, tolerance, is used to evaluate a subtree's suitability for pruning. A node is pruned if all of the leaves in its subtree are within tolerance of its average.

## Demo
*with tolerance value of 50000*

![image](https://user-images.githubusercontent.com/53128080/147374704-b559bec3-ba34-4c05-b30d-bb05b1dd3032.png)
![image](https://user-images.githubusercontent.com/53128080/147374846-273955f6-a70f-4706-9c5d-966a1e83a353.png)
