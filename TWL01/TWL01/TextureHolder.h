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
	map<String, Texture> m_Texture;
	static TextureHolder *m_s_Instance;

public:
	TextureHolder();
	static Texture & GetTexture(const string & filename);

};
#endif // !TEXTUREHOLDER_H_

