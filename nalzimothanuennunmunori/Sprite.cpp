#include "Sprite.h"


Sprite::Sprite(const std::string path) : path(path), size(Vector2::zero) {}

Sprite::~Sprite() {}


void Sprite::InitSprite()
{
    if (this->colorSet.size() == 0)
    {
        std::ifstream file(this->path, std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            UINT w = 0;
            UINT h = 0;
            char header[54];
            file.read(header, 54);
            for (UINT i = 0; i < 4; i++) {
                w |= (header[18 + i] & 255) << (8 * i);
                h |= (header[22 + i] & 255) << (8 * i);
            }
            const int pad = (4 - (3 * w) % 4) % 4;
            const int imgsize = (3 * w + pad) * h;
            char* img = new char[imgsize];
            file.read(img, imgsize);
            file.close();
            for (UINT y = 0; y < h; y++) {
                for (UINT x = 0; x < w; x++) {
                    const int i = 3 * x + y * (3 * w + pad);
                    this->colorSet.push_back(RGB((img[i] & 255), (img[i + 1] & 255), (img[i + 2] & 255)));
                }
            }
            delete[] img;
            this->size.x = w;
            this->size.y = h;
        }
    }
}