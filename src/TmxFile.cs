using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Linq;


public class TmxFile()
{
    public string Version => MapNode.Attribute("version").Value;

    public string Orientation => MapNode.Attribute("orientation").Value;

    public int Width => int.Parse(MapNode.Attribute("width").Value);

    public int Height => int.Parse(MapNode.Attribute("height").Value);

    public int TileWidth => int.Parse(MapNode.Attribute("tilewidth").Value);

    public int TileHeight => int.Parse(MapNode.Attribute("tileheight").Value);

    private readonly Dictionary<int, TileSet> TileSets = new();

    private readonly Dictionary<string, Layer> Layers = new();

    private readonly Dictionary<string, ObjectGroup> ObjectGroups = new();

    private int[] gids;

    private XElement MapNode;

    private XElement BmpSettings;

    public static TmxFile Read(string path)
    {
        var tmxFile = new TmxFile();
        var document = XDocument.Load(path);

        tmxFile.MapNode = document.Root;
        tmxFile.ReadTilesets(document);
        tmxFile.ReadLayers(document);
        tmxFile.ReadObjectGroups(document);
        tmxFile.BmpSettings = document.Descendants("bmp-settings").First();
        tmxFile.gids = tmxFile.TileSets.Keys.OrderBy(k => k).ToArray();

        return tmxFile;
    }

    private void ReadTilesets(XDocument document)
    {
        var xmlTileSets = document.Descendants("tileset");

        foreach (var node in xmlTileSets)
        {
            var tileset = TileSet.Parse(node);
            this.TileSets[tileset.Firstgid] = tileset;
        }

        Console.WriteLine($"{TileSets.Count} tilesets parsed.");
    }

    public void ReadLayers(XDocument document)
    {
        var xmlLayers = document.Descendants("layer");

        foreach (var node in xmlLayers)
        {
            var layer = Layer.Parse(node);
            this.Layers[layer.Name] = layer;
        }

        Console.WriteLine($"{Layers.Count} layers parsed.");
    }

    public void ReadObjectGroups(XDocument document)
    {
        var xmlObjectGroups = document.Descendants("objectgroup");

        foreach (var node in xmlObjectGroups)
        {
            var objectGroup = ObjectGroup.Parse(node);
            this.ObjectGroups[objectGroup.Name] = objectGroup;
        }

        Console.WriteLine($"{ObjectGroups.Count} objectgroups parsed.");
    }

    public void Save(string path)
    {
        var document = new XDocument();

        var mapNode = new XElement("map",
            new XAttribute("version", Version),
            new XAttribute("orientation", Orientation),
            new XAttribute("width", Width),
            new XAttribute("height", Height),
            new XAttribute("tilewidth", TileWidth),
            new XAttribute("tileheight", TileHeight)
        );

        foreach (var tileset in TileSets.Values)
        {
            mapNode.Add(tileset.ToXml());
        }

        foreach (var layer in Layers.Values)
        {
            mapNode.Add(layer.ToXml());
        }

        foreach (var objectGroup in ObjectGroups.Values)
        {
            mapNode.Add(objectGroup.ToXml());
        }

        mapNode.Add(BmpSettings);
        document.Add(mapNode);
        document.Save(path);
    }

    public void Clear(int x0, int y0, int x1, int y1)
    {
        const int replacer = 10679;

        for (int x = x0; x <= x1 + 1; x++)
        {
            for (int y = y0; y <= y1 + 1; y++)
            {
                foreach (var layer in Layers.Values)
                {
                    layer.SetGID(x, y, replacer);
                }
            }
        }
    }

    public int GetGID(string layerName, int x, int y)
    {
        return Layers[layerName].GetGID(x, y);
    }

    public TileSet FindTileSet(string layerName, int x, int y)
    {
        var gid = GetGID(layerName, x, y);

        return FindTileSet(gid);
    }

    public TileSet FindTileSet(int gid)
    {
        var gids = this.gids.ToArray();

        while (gids.Length > 1)
        {
            var index = gids.Length / 2;
            var currentValue = gids[index];

            if (currentValue < gid)
            {
                gids = gids[index..gids.Length];
            }
            else
            {
                gids = gids[0..index];
            }
        }

        return TileSets[gids[0]];
    }
}