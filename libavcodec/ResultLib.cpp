//
//  ResultLib.cpp
//  RoiReader
//
//  Created by Bichuan Guo on 2/15/16.
//  Copyright © 2016 loft. All rights reserved.
//

#include "ResultLib.hpp"
#include <stdlib.h>

int Result::width = 0;
int Result::height = 0;
int ResultLib::width = 0;
int ResultLib::height = 0;
int ResultLib::blockSize = 16;

std::pair<int, int> Result::dimension(int xScale, int yScale)
{
    int w = (width - 1) / xScale + 1;
    int h = (height - 1) / yScale + 1;
    return std::pair<int, int>(w, h);
}

Result::Result(uchar xScale, uchar yScale) :
    xScale(xScale), yScale(yScale) {
        std::pair<int, int> size = dimension(xScale, yScale);
        data = new uchar*[size.second];
        for (int i = 0; i < size.second; i++) {
            data[i] = new uchar[size.first];
        }
}

void Result::destroy() {
    std::pair<int, int> size = dimension(xScale, yScale);
    for (int i = 0; i < size.second; i++) {
        delete data[i];
    }
    delete data;
}

int8_t* Result::ToFloatArray(int qp1, int qp2, int8_t (*mapping)(uchar, int, int)) {
    int8_t* res;
    res = new int8_t[width * height];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            res[i * width + j] = mapping(data[i / xScale][j / yScale], qp1, qp2);
        }
    }
    
    return res;
}

int8_t Result::linearMapping(uchar val, int low, int high) {
    float ratio = (float)val / 255;
    return int8_t (high + ratio * (low - high));
}

void Result::freeFloatArray(int8_t*& a) {
    if (a == nullptr) {
        return;
    }
    delete a;
    a = nullptr;
}

ResultLib::ResultLib() {
    dict.clear();
    map.clear();
}

uint32_t ResultLib::read(std::string filename) {
    FILE *fp = fopen(filename.c_str(), "rb");
    int buffer;
    fread(&buffer, sizeof(int), 1, fp);
    ResultLib::width = buffer;
    Result::width = (ResultLib::width - 1) / ResultLib::blockSize + 1;
    fread(&buffer, sizeof(int), 1, fp);
    ResultLib::height = buffer;
    Result::height = (ResultLib::height - 1) / ResultLib::blockSize + 1;
    fread(&buffer, sizeof(int), 1, fp);
    ResultLib::blockSize = buffer;
    int mapSize, dictSize;
    fread(&mapSize, sizeof(int), 1, fp);
    fread(&dictSize, sizeof(int), 1, fp);
    fread(&buffer, sizeof(int), 1, fp);
    if(buffer) return false;
    int usage;
    uint32_t length = 0;
    uchar xScale, yScale;
    Result *res;
    for (int i = 0; i < dictSize; i++) {
        fread(&usage, sizeof(int), 1, fp);
        if (!usage) {
            res = nullptr;
        }
        else {
            fread(&xScale, sizeof(uchar), 1, fp);
            fread(&yScale, sizeof(uchar), 1, fp);
            res = new Result(xScale, yScale);
            std::pair<int, int> size = Result::dimension(xScale, yScale);
            for (int j = 0; j < size.second; j++) {
                fread(res->data[j], sizeof(uchar), size.first, fp);
            }
            length = uint32_t(size.first * size.second);
        }
        fread(&buffer, sizeof(int), 1, fp);
        if(buffer) return false;
        dict.push_back(res);
    }
    fread(&buffer, sizeof(int), 1, fp);
    if(buffer) return false;
    for (int i = 0; i < mapSize; i++) {
        fread(&buffer, sizeof(int), 1, fp);
        map.push_back(buffer);
    }
    fread(&buffer, sizeof(int), 1, fp);
    if(buffer) return false;
    fclose(fp);
    return length;
}

void ResultLib::destroy()
{
    for (int i = 0; i < dict.size(); i++) {
        if (dict[i]) {
            dict[i]->destroy();
            delete dict[i];
            dict[i] = nullptr;
        }
    }
    dict.clear();
    map.clear();
}

int8_t* ResultLib::getQpMap(int frameIdx, int qp1, int qp2) {
    if (frameIdx < 0 || frameIdx >= map.size()) {
        return nullptr;
    }
    
    if (map[frameIdx] == POS_UNDEFINED) {
        return nullptr;
    }
    
    assert(map[frameIdx] >= 0 && map[frameIdx] < dict.size());
    Result* res = dict[map[frameIdx]];
    assert(res);
    return res->ToFloatArray(qp1, qp2);
    
}