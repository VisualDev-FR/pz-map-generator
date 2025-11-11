using System.Xml.Linq;

public class SourceImage
{
    public string Source { get; set; }

    public int Width { get; set; }

    public int Height { get; set; }

    public static SourceImage Parse(XElement node)
    {
        return new SourceImage()
        {
            Source = node.Attribute("source").Value,
            Width = int.Parse(node.Attribute("width").Value),
            Height = int.Parse(node.Attribute("height").Value),
        };
    }
}