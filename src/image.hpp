#pragma once

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./ven/stb_image.h"
#include "./ven/stb_image_write.h"

struct rect2 {
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
};

class MCSkinImage {
public:
	MCSkinImage() {
		m_Data = nullptr;
	}
	MCSkinImage(std::string _fileName) {
		load(_fileName);
	}
	MCSkinImage(const MCSkinImage& _other) {
		*this = _other;
	}
	~MCSkinImage() {
		if(m_Data != nullptr) stbi_image_free(m_Data);
	}

	bool load(std::string _fileName) {
		if(m_Data != nullptr) stbi_image_free(m_Data);
		
		m_Data = stbi_load(_fileName.c_str(), &m_Width, &m_Height, &m_Format, STBI_rgb_alpha);
		
		make64x64();
		
		return check() && m_Data != nullptr;
	}
	void save(std::string _fileName) {
		stbi_write_png(_fileName.c_str(), m_Width, m_Height, m_Format, m_Data, m_Width*m_Format);
	}

	bool check() {
		return m_Width == 64 && m_Height == 64 && m_Format == STBI_rgb_alpha;
	}

	void make64x64() {
		if(m_Width == 64 && m_Height == 32 && m_Format == STBI_rgb_alpha) {
			MCSkinImage finalImg;
			finalImg.m_Format = 4;
			finalImg.m_Height = 64;
			finalImg.m_Width = 64;
			finalImg.m_Data = (uint8_t*)malloc(64*64*4*sizeof(uint8_t));
			memset(finalImg.m_Data, 0, 64*64*4*sizeof(uint8_t));
			memcpy(finalImg.m_Data, m_Data, 64*32*4*sizeof(uint8_t));
			
			// finalImg.blit(*this, {7, 7, 7, 7}, {17, 7, 0, 0}, true, true); // an example of how to use the blit function

			// limb endings (the small 4x4 parts at the top and bottom of the limbs)
			finalImg.blit(*this, {4, 16, 4, 4}, {20, 48, 0, 0}, true, false);  // leg top
			finalImg.blit(*this, {8, 16, 4, 4}, {24, 48, 0, 0}, true, false);  // leg bottom

			finalImg.blit(*this, {44, 16, 4, 4}, {36, 48, 0, 0}, true, false); // arm top
			finalImg.blit(*this, {48, 16, 4, 4}, {40, 48, 0, 0}, true, false); // arm bottom

			// limb sides
			finalImg.blit(*this, {8, 20, 4, 12}, {16, 52, 0, 0}, true, false); // leg inner
			finalImg.blit(*this, {0, 20, 4, 12}, {24, 52, 0, 0}, true, false); // leg outer
			finalImg.blit(*this, {4, 20, 4, 12}, {20, 52, 0, 0}, true, false); // leg front
			finalImg.blit(*this, {12, 20, 4, 12}, {28, 52, 0, 0}, true, false); // leg back

			finalImg.blit(*this, {48, 20, 4, 12}, {32, 52, 0, 0}, true, false); // arm inner
			finalImg.blit(*this, {40, 20, 4, 12}, {40, 52, 0, 0}, true, false); // arm outer
			finalImg.blit(*this, {44, 20, 4, 12}, {36, 52, 0, 0}, true, false); // arm front
			finalImg.blit(*this, {52, 20, 4, 12}, {44, 52, 0, 0}, true, false); // arm back

			*this = finalImg;
		}
	}

	MCSkinImage& operator+=(const MCSkinImage& _img) {
		for(uint32_t i = 0; i < 64 * 64 * 4; i+=4) {
			if(_img.m_Data[i+3] == 255) {
				m_Data[i] = _img.m_Data[i];
				m_Data[i+1] = _img.m_Data[i+1];
				m_Data[i+2] = _img.m_Data[i+2];
				m_Data[i+3] = _img.m_Data[i+3];
			}
		}

		return *this;
	}
	MCSkinImage& operator-=(const MCSkinImage& _img) {
		for(uint32_t i = 0; i < 64 * 64 * 4; i+=4) {
			if(_img.m_Data[i+3] == 255) {
				m_Data[i] = 0;
				m_Data[i+1] = 0;
				m_Data[i+2] = 0;
				m_Data[i+3] = 0;
			}
		}

		return *this;
	}

	MCSkinImage& operator=(const MCSkinImage& _img) {
		if(m_Data != nullptr) stbi_image_free(m_Data);
		size_t size = _img.m_Width * _img.m_Height * _img.m_Format * sizeof(uint8_t);
		m_Data = (uint8_t*)malloc(size);
		memset(m_Data, 0, size);
		memcpy(m_Data, _img.m_Data, size);

		m_Format = _img.m_Format;
		m_Height = _img.m_Height;
		m_Width = _img.m_Width;

		return *this;
	}

	void blit(const MCSkinImage& _overlay, rect2 _cutout, rect2 _pastein, bool _flipx = false, bool _flipy = false) {
		size_t x = 0, y = 0;

		if(_flipx) {
			x = _cutout.x + _cutout.w - 1;
		}
		else {
			x = _cutout.x;
		}

		for(size_t i = (_pastein.x < 0 ? 0 : _pastein.x); i < (m_Width > _pastein.x + _cutout.w ? _pastein.x + _cutout.w : m_Width); i++) {
			if(_flipy) {
				y = _cutout.y + _cutout.h - 1;
			}
			else {
				y = _cutout.y;
			}

			for(size_t j = (_pastein.y < 0 ? 0 : _pastein.y); j < (m_Height > _pastein.y + _cutout.h ? _pastein.y + _cutout.h : m_Height); j++) { // whilst we could start from the 0 x 0 y position, this (starting from the given position) optimizes the process and makes it a bit quicker

				// j and i are the positions currently worked on, in the image we want the other one to be pasted in
				// x and y are the positions currently worked on, in the image we are pasting in

				m_Data[(j*m_Width+i)*4] = _overlay.m_Data[(y*_overlay.m_Width+x)*4];
				m_Data[(j*m_Width+i)*4+1] = _overlay.m_Data[(y*_overlay.m_Width+x)*4+1];
				m_Data[(j*m_Width+i)*4+2] = _overlay.m_Data[(y*_overlay.m_Width+x)*4+2];
				m_Data[(j*m_Width+i)*4+3] = _overlay.m_Data[(y*_overlay.m_Width+x)*4+3];


				if(_flipy) {
					y--;
				}
				else {
					y++;
				}
			}
			
			if(_flipx) {
				x--;
			}
			else {
				x++;
			}
		}
	}

public:
	int m_Width;
	int m_Height;
	int m_Format;
	uint8_t* m_Data;
};