using System;
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
        return ReadUntil(bytes, [0x0A], ref offset).Decode(Encoding.UTF8);
    }

    public static int IndexOf(this byte[] bytes, byte[] pattern, int offset = 0)
    {
        ReadOnlySpan<byte> span = bytes;

        var index = span[offset..].IndexOf(pattern);

        if (index < 0)
        {
            throw new Exception("Index not found");
        }

        return index + offset;
    }

    public static byte[] ReadUntil(this byte[] bytes, byte[] pattern, ref int offset)
    {
        var end = bytes.IndexOf(pattern, offset) + pattern.Length;
        var slice = bytes[offset..end];

        offset = end;

        return slice;
    }

    public static byte[] ReadWithLength(this byte[] bytes, ref int offset)
    {
        var length = bytes.ReadInt32(ref offset);
        var text = bytes[offset..(offset + length)];

        offset += length;

        return text;
    }

    public static string Decode(this byte[] bytes, Encoding encoding)
    {
        return encoding.GetString(bytes);
    }

}