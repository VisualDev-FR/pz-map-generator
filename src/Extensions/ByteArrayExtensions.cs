using System;
using System.Linq;
using System.Text;


public static class ByteArrayExtensions
{
    public static int ReadInt32(this byte[] bytes, ref int offset)
    {
        return BitConverter.ToInt32(bytes, (offset += 4) - 4);
    }

    public static int ReadInt32(this byte[] bytes, int offset)
    {
        return BitConverter.ToInt32(bytes, offset);
    }

    public static string ReadLine(this byte[] bytes, ref int offset)
    {
        return ReadUntil(bytes, "\n", ref offset).Decode(Encoding.UTF8);
    }

    public static byte[] ReadUntil(this byte[] bytes, string pattern, ref int offset)
    {
        ReadOnlySpan<byte> span = bytes;
        ReadOnlySpan<byte> pat = Utils.Encode(pattern);

        int index = span[offset..].IndexOf(pat);

        if (index < 0)
            throw new Exception("Pattern not found");

        var absoluteIndex = offset + index;
        var length = absoluteIndex - offset;
        var result = span.Slice(offset, length).ToArray();

        offset = absoluteIndex + pat.Length;

        return result;
    }

    public static string Decode(this byte[] bytes, Encoding encoding)
    {
        return encoding.GetString(bytes);
    }

    public static byte[] ReadWithLength(this byte[] bytes, ref int offset)
    {
        var length = bytes.ReadInt32(ref offset);
        var text = bytes[offset..(offset + length)];

        offset += length;

        return text;
    }
}