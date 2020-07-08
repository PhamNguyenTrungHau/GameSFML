#include "TextureHolder.h"
#include<assert.h>

 TextureHolder *TextureHolder::m_s_Instance = nullptr;

//Contructors
TextureHolder::TextureHolder()
{
	assert(TextureHolder::m_s_Instance == nullptr);
	TextureHolder::m_s_Instance = this;
}

Texture & TextureHolder::GetTexture(const string & filename)
{
	// Get a reference to m_Texture using m_s_Instance
	auto & m = m_s_Instance->m_Texture;
	// auto is the equivalent of map<string, Texture>

	// Create an iterator to hold a key-value-pair(kvp)
	// and search for the equivalent kvp
	// using the passed in filename
	auto keyValuePair = m.find(filename);
	// auto is equivalent of map<string, Texture>::Iterator
	
	// Did we find a match?
	if (keyValuePair != m.end())
	{
		//Yes
		//Return the texture,
		//the second part of the kvp, the texture	
		return keyValuePair->second;
	}
	else
	{
		//Filename not find
		//Create	 a new key value pair using the filename
		auto & texture = m[filename];
		//Load the texture	from file in the usual way
		texture.loadFromFile(filename);
		//Return the texture to the calling code
		return texture;
	}

}
