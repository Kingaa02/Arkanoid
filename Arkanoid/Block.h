#pragma once

class Block
{
public:
    float x = 0, x2 = 0;
    float y = 0, y2 = 0;
    float points = 0;
    int hitCount;

    Block(int x);

    Block()
    {
        hitCount = 0;
    }

        int getHitCount() const {
            return hitCount;
        }

        void setHitCount(int count) {
            hitCount = count;
        }
    
};

