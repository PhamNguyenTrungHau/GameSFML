#pragma once
#ifndef TEXTUREHOLDER_H_
#define	TEXTUREHOLDER_H_
#include"SFML/Graphics.hpp"
#include<map>
#include<string>
using namespace sf;
using namespace std;

class TextureHolder
{
	//A map container from the STL
	//That holds related pairs of String and Texture
	map<String, Texture> m_Texture;

	// A pointer of the same type as the class itself
	// The one and only instance
	static TextureHolder *m_s_Instance;

public:
	TextureHolder();
	static Texture & GetTexture(const string & filename);
};
#endif // !TEXTUREHOLDER_H_

