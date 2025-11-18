using System;
using System.IO;
using System.Linq;
using System.Text;

public class PackFile
{
    private static readonly byte[] magic = Utils.Encode("PZPK");

    public int Version { get; set; }

    public Page[] Pages { get; set; }

    public static PackFile Read(string path)
    {
        var bytes = File.ReadAllBytes(path);
        var packFile = new PackFile();
        var position = 0;

        packFile.ReadVersion(bytes, ref position);
        packFile.ReadPages(bytes, ref position);

        if (position != bytes.Length)
        {
            throw new Exception($"End of file not reached: {position:N0} / {bytes.Length:N0}");
        }

        return packFile;
    }

    private void ReadVersion(byte[] bytes, ref int position)
    {
        if (bytes[0..4].SequenceEqual(magic))
        {
            position += 4;
            Version = bytes.ReadInt32(ref position);
        }
        else
        {
            Version = 0;
        }
    }

    private void ReadPages(byte[] bytes, ref int position)
    {
        var pagesCount = bytes.ReadInt32(ref position);

        Pages = new Page[pagesCount];

        for (int i = 0; i < pagesCount; i++)
        {
            Pages[i] = Page.Read(bytes, Version, ref position);
        }
    }
}

public class Page
{
    public int Version { get; set; }

    public string Name { get; set; }

    public int HasAlpha { get; set; }

    public byte[] Png { get; set; }

    public Texture[] Textures { get; set; }

    public static Page Read(byte[] bytes, int version, ref int position)
    {
        var page = new Page();

        page.Version = version;
        page.Name = bytes.ReadWithLength(ref position).Decode(Encoding.UTF8);
        page.ReadTextures(bytes, ref position);
        page.ReadPNG(bytes, ref position);

        return page;
    }

    private void ReadTextures(byte[] bytes, ref int position)
    {
        var texturesCount = bytes.ReadInt32(ref position);

        HasAlpha = bytes.ReadInt32(ref position);
        Textures = new Texture[texturesCount];

        for (int i = 0; i < texturesCount; i++)
        {
            Textures[i] = Texture.Read(bytes, ref position);
        }
    }

    private void ReadPNG(byte[] bytes, ref int position)
    {
        if (Version == 0)
        {
            Png = bytes.ReadUntil([0xEF, 0xBE, 0xAD, 0xDE], ref position);
        }
        else if (Version == 1)
        {
            Png = bytes.ReadWithLength(ref position);
        }
        else
        {
            throw new Exception($"Unsupported pack version: '{Version}'");
        }
    }

    public void SavePng(string path)
    {
        File.WriteAllBytes(path, Png);
    }
}

public class Texture
{
    public string Name { get; set; }

    public int X { get; set; }

    public int Y { get; set; }

    public int Width { get; set; }

    public int Height { get; set; }

    public int Ox { get; set; }

    public int Oy { get; set; }

    public int Ow { get; set; }

    public int Oh { get; set; }

    public static Texture Read(byte[] bytes, ref int position)
    {
        var texture = new Texture();

        texture.Name = bytes.ReadWithLength(ref position).Decode(Encoding.UTF8);

        texture.X = bytes.ReadInt32(ref position);
        texture.Y = bytes.ReadInt32(ref position);

        texture.Width = bytes.ReadInt32(ref position);
        texture.Height = bytes.ReadInt32(ref position);

        texture.Ox = bytes.ReadInt32(ref position);
        texture.Oy = bytes.ReadInt32(ref position);
        texture.Ow = bytes.ReadInt32(ref position);
        texture.Oh = bytes.ReadInt32(ref position);

        return texture;
    }
}