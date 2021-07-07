#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./ven/stb_image.h"
#include "./ven/stb_image_write.h"

class Image {
public:
	Image() {
		m_Data = nullptr;
	}
	Image(std::string _fileName) {
		load(_fileName);
	}
	~Image() {
		if(m_Data != nullptr)
			stbi_image_free(m_Data);
	}

	bool load(std::string _fileName) {
		m_Data = stbi_load(_fileName.c_str(), &m_Width, &m_Height, &m_Format, STBI_rgb_alpha);
		return check() && m_Data != nullptr;
	}
	void save(std::string _fileName) {
		stbi_write_png(_fileName.c_str(), m_Width, m_Height, m_Format, m_Data, m_Width*m_Format);
	}

	bool check() {
		return m_Width == 64 && m_Height == 64 && m_Format == STBI_rgb_alpha;
	}

	Image& operator+=(const Image& _img) {
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
	Image& operator-=(const Image& _img) {
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


protected:
	int m_Width;
	int m_Height;
	int m_Format;
public:
	uint8_t* m_Data;
};