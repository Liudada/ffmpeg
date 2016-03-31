//
//  ResultLib.hpp
//  RoiReader
//
//  Created by Bichuan Guo on 2/15/16.
//  Copyright © 2016 loft. All rights reserved.
//

#ifndef ResultLib_hpp
#define ResultLib_hpp

#define POS_UNDEFINED -1

typedef unsigned char uchar;

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <string>

class Result
{
    friend class ResultLib;
public:
    Result(uchar xScale, uchar yScale);
    void destroy();
    
    static std::pair<int, int> dimension(int xScale, int yScale);
    int8_t* ToFloatArray(int qp1, int qp2, int8_t (*mapping)(uchar, int, int) = Result::linearMapping);
    static void freeFloatArray(int8_t*& a);
    
    static int8_t linearMapping(uchar val, int low, int high);
    
private:
    static int width;
    static int height;
    uchar** data;
    uchar xScale;
    uchar yScale;
};

class ResultLib
{
public:
    ResultLib();
    
    uint32_t read(std::string filename);
    void destroy();
    int8_t* getQpMap(int frameIdx, int qp1, int qp2);
    
private:
    static int width;
    static int height;
    static int blockSize;
    std::vector<Result*> dict;
    std::vector<int> map;
    
};

#endif /* ResultLib_hpp */