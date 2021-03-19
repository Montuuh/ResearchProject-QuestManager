#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "ModuleCollisions.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

struct Collider;

Map::Map() : Module()
{
    name.Create("map");
}

Map::~Map()
{}

bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

void Map::Draw()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* L;
	L = data.layers.start;

	TileSet* tileset = NULL;
	while (L != NULL)
	{
		if (L->data->properties.GetProperty("noDraw") == 0)
		{
			L = L->next;
			continue;
		}

		// Draw function
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = L->data->Get(x, y);
				if (tileId > 0)
				{
					tileset = GetTilesetFromTileId(tileId);
					SDL_Rect n = tileset->GetTileRect(tileId);
					iPoint pos = MapToWorld(x, y);
					app->render->DrawTexture(tileset->texture, pos.x, pos.y, &n);
				}
			}
		}
		L = L->next;
	}
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float halfWidth = data.tileWidth * 0.5f;
		float halfHeight = data.tileHeight * 0.5f;
		ret.x = int((x / halfWidth + y / halfHeight) / 2);
		ret.y = int((y / halfHeight - (x / halfWidth)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* L = data.tilesets.start;
	TileSet* tileset = L->data;

	while (L != NULL)
	{
		if (L->next == nullptr)
		{
			break;
		}

		if (id < L->next->data->firstgid)
		{
			return tileset = L->data;
		}

		L = L->next;
	}

	return tileset = L->data;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rec = { 0 };

	int relativeId = id - firstgid;
	rec.w = tileWidth;
	rec.h = tileHeight;
	rec.x = margin + ((rec.w + spacing) * (relativeId % numTilesWidth));
	rec.y = margin + ((rec.h + spacing) * (relativeId / numTilesWidth));

	return rec;
}

int TileSet::GetTileRelativeId(int id)const
{
	return (id - firstgid);
}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	int result = defaultValue;
	SString targetName(value);
	ListItem<Property*>* L = list.start;
	while (L != NULL)
	{
		SString name(L->data->name.GetString());
		if (name == targetName)
		{
			result = L->data->value;
			break;
		}
		L = L->next;
	}

	return result;
}

bool Map::CleanUp()
{
	ListItem<TileSet*>* L;
	L = data.tilesets.start;

	while (L != NULL)
	{
		RELEASE(L->data);
		L = L->next;
	}
	data.tilesets.Clear();

	ListItem<MapLayer*>* L2;
	L2 = data.layers.start;

	while (L2 != NULL)
	{
		RELEASE(L2->data);
		L2 = L2->next;
	}
	data.layers.Clear();


	mapFile.reset();

	return true;
}

bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);
	pugi::xml_parse_result result = mapFile.load_file("Assets/Maps/map.tmx");

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true)
	{
		ret = LoadMap();
	}

	pugi::xml_node tilesetNode;
	for (tilesetNode = mapFile.child("map").child("tileset"); tilesetNode && ret; tilesetNode = tilesetNode.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tilesetNode, set);

		if (ret == true) ret = LoadTilesetImage(tilesetNode, set);

		data.tilesets.Add(set);
	}

	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		LoadProperties(layer, lay->properties);

		if (ret == true)
			data.layers.Add(lay);
	}

	if (ret == true)
	{
	}

	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		LOG("Filling Map info");

		SString strType(map.attribute("orientation").as_string());
		data.type = StrToMapType(strType);
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		SString color(map.attribute("backgroundcolor").as_string());
		color.Trim();
		sscanf_s(color.GetString(), "%02x%02x%02x", (uint)&data.backgroundColor.r, (uint)&data.backgroundColor.g, (uint)&data.backgroundColor.b);
		data.backgroundColor.a = 255;
	}

	return ret;
}

bool Map::LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;

	LOG("Filling TilesetDetails");
	set->firstgid = tilesetNode.attribute("firstgid").as_int();
	set->name = tilesetNode.attribute("name").as_string();
	set->tileWidth = tilesetNode.attribute("tilewidth").as_int();
	set->tileHeight = tilesetNode.attribute("tileheight").as_int();
	set->spacing = tilesetNode.attribute("spacing").as_int();
	set->margin = tilesetNode.attribute("margin").as_int();

	set->offsetX = tilesetNode.child("tileoffset").attribute("x").as_int();
	set->offsetY = tilesetNode.child("tileoffset").attribute("y").as_int();
	return ret;
}

// L03: TODO: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tilesetNode.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load Tileset image
		LOG("Filling TilesetDetails");
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		set->texture = app->tex->Load(tmp.GetString());
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

bool Map::StoreId(pugi::xml_node& node, MapLayer* layer, int index)
{
	bool ret = true;

	layer->data[index] = node.attribute("gid").as_uint();

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->data = new uint[(data.width * data.height * sizeof(uint))];
	memset(layer->data, 0, size_t(data.width * data.height * sizeof(uint)));
	pugi::xml_node gidNode;

	int i = 0;
	for (gidNode = node.child("data").child("tile"); gidNode && ret; gidNode = gidNode.next_sibling("tile"))
	{
		if (ret == true) ret = StoreId(gidNode, layer, i);
		++i;
	}

	LOG("Layer <<%s>> has loaded %d tiles", layer->name.GetString(), i);
	return ret;
}

MapTypes operator++(MapTypes& mode)
{
	mode = static_cast<MapTypes>((mode + 1) % 4);
	return mode;
}

MapTypes Map::StrToMapType(SString s)
{
	SString StrType[4];
	StrType[0] = "unknown", StrType[1] = "orthogonal", StrType[2] = "isometric", StrType[3] = "staggered";
	MapTypes type = MapTypes::MAPTYPE_UNKNOWN;
	for (int i = 0; i < 4; ++i)
	{
		if (s == StrType[i])
			return type;
		++type;
	}
	return MAPTYPE_UNKNOWN;
}

bool to_bool(std::string const& s) {
	return s != "0";
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node propertiesNode;
	propertiesNode = node.child("properties");

	pugi::xml_node propertyNode = propertiesNode.child("property");

	int i = 0;
	while (propertyNode != NULL)
	{
		Property* prop = new Property();

		prop->name = propertyNode.attribute("name").as_string();

		if (propertyNode.attribute("type"))
		{
			prop->value = propertyNode.attribute("value").as_bool();
		}
		else
		{
			prop->value = to_bool(propertyNode.attribute("value").as_string());
		}

		properties.list.Add(prop);
		propertyNode = propertyNode.next_sibling("property");
	}

	//...
	return ret;
}

int Map::LoadColliders()
{
	SString colliderTilesetName("meta");

	ListItem<MapLayer*>* L;
	L = data.layers.start;
	while (L != NULL)
	{
		SString name = L->data->name;
		if (name == colliderTilesetName)
		{
			break;
		}
		L = L->next;
	}

	TileSet* tileset = NULL;
	for (int j = 0; j < data.height; ++j)
	{
		for (int i = 0; i < data.width; ++i)
		{
			int tileId = L->data->Get(i, j);
			if (tileId > 0)
			{
				iPoint pos = MapToWorld(i, j);
				SDL_Rect r =
				{
					pos.x,
					pos.y,
					data.tileWidth,
					data.tileHeight
				};
				tileset = GetTilesetFromTileId(tileId);
				int relativeId = tileset->GetTileRelativeId(tileId);

				switch (relativeId)
				{
				case 0: // Void --> Do nothing
					break;
				case 1: // Red color --> Block
					r.x += 11;
					r.w -= 22;
					r.y += 11;
					r.h -= 22;
					app->collisions->AddCollider(r, Collider::Type::BLOCK, nullptr);
					break;
				case 2: // Blue color --> 
					break;
				case 3: // Green color --> 
					break;
				case 4: // Black blue color --> 
					break;
				case 5: // Orange color --> 
					break;
				}
			}
		}
	}

	return true;
}