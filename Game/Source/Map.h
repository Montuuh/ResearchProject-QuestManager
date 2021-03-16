#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "SDL/include/SDL.h"
#include "PugiXml\src\pugixml.hpp"

struct TileSet
{
	SString	name;
	int firstgid;
	int margin;
	int spacing;
	int tileWidth;
	int tileHeight;

	SDL_Texture* texture;
	int texWidth;
	int texHeight;
	int numTilesWidth;
	int numTilesHeight;
	int offsetX;
	int offsetY;

	SDL_Rect GetTileRect(int id) const;
	int GetTileRelativeId(int id)const;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Property
{
	SString name;
	bool value;
};

struct Properties
{
	~Properties()
	{
	}

	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
};

struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{
		width = 0;
		height = 0;
	}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;

	List<MapLayer*> layers;
};

class Map : public Module
{
public:
	Map();

	virtual ~Map();

	bool Awake(pugi::xml_node& conf);

	void Draw();

	bool CleanUp();

	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y) const;

	int LoadColliders();

	MapData data;
	MapTypes StrToMapType(SString s);

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool StoreId(pugi::xml_node& node, MapLayer* layer, int index);

	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;


};
#endif