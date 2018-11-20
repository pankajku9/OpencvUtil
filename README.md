# OpencvUtil
This is a small utility library over OpenCV which support these things.  
* Convert Opencv::Mat into vector  
* Store and dump to file system  
* Get data in Bitmap format and write data from bitmap format   
* Generate test image, with text etc.  
    
Opencv uses its own optimized convention to store image. This can used a interconnecting layer to and from opencv to outside Images source.

## Build and use
1. 'make lib' to generate shared lib named libopencvutil
2.  Need opencv installed on system
3.  only tried on linux 