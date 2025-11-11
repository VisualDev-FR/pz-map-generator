using System.Xml.Linq;

public class TileSet
{
    public int Firstgid { get; set; }

    public string Name { get; set; }

    public int Tilewidth { get; set; }

    public int Tileheight { get; set; }

    public SourceImage ImageSource { get; set; }

    public static TileSet Parse(XElement node)
    {
        return new TileSet()
        {
            Firstgid = int.Parse(node.Attribute("firstgid").Value),
            Name = node.Attribute("name").Value,
            Tilewidth = int.Parse(node.Attribute("tilewidth").Value),
            Tileheight = int.Parse(node.Attribute("tileheight").Value),
            ImageSource = SourceImage.Parse(node.Element("image")),
        };
    }
}