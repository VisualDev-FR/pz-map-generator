using System;
using System.IO;
using System.Linq;
using System.Text;


public class LotpackFile
{
    private static readonly byte[] magic = Encoding.UTF8.GetBytes("LOTP");

    public int Version { get; set; }

    public LotheaderFile Header { get; set; }

    public SquareData[][,,] BlockDatas { get; set; }

    public static LotpackFile Read(string path, LotheaderFile header)
    {
        var bytes = File.ReadAllBytes(path);
        var lotpack = new LotpackFile();
        var position = 0;

        lotpack.Header = header;
        lotpack.ReadVersion(bytes, ref position);
        lotpack.ReadBlocks(bytes, ref position);

        if (position != bytes.Length)
        {
            throw new Exception($"End of file not reached: {position:N0} / {bytes.Length:N0} ('{path}')");
        }

        return lotpack;
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

        if(Version != Header.Version)
        {
            throw new Exception($"Inconsistant version: '{Version}', header: '{Header.Version}'");
        }
    }

    private void ReadBlocks(byte[] bytes, ref int position)
    {
        var blockCount = bytes.ReadInt32(ref position);
        var blockTable = position;

        BlockDatas = new SquareData[blockCount][,,];

        for (int i = 0; i < blockCount; i++)
        {
            position = bytes.ReadInt32(blockTable + i * 8);
            BlockDatas[i] = ReadSquareDatas(bytes, Header, ref position);
        }
    }

    public SquareData[,,] ReadSquareDatas(byte[] bytes, LotheaderFile header, ref int position)
    {
        var blockSize = header.BlockSizeInSquare;
        var squarePerLayer = blockSize * blockSize;
        var minLayer = header.MinLayer;
        var maxLayer = header.MaxLayer;
        var skip = 0;

        var blockDatas = new SquareData[blockSize, blockSize, maxLayer - minLayer];

        for (int z = 0; z < maxLayer - minLayer; z++)
        {
            if (skip >= squarePerLayer)
            {
                skip -= squarePerLayer;
                continue;
            }

            for (int x = 0; x < blockSize; x++)
            {
                if (skip >= blockSize)
                {
                    skip -= blockSize;
                    continue;
                }

                for (int y = 0; y < blockSize; y++)
                {
                    if (skip > 0)
                    {
                        skip -= 1;
                        continue;
                    }

                    var count = bytes.ReadInt32(ref position);

                    if (count == -1)
                    {
                        skip = bytes.ReadInt32(ref position);

                        if (skip > 0)
                        {
                            skip -= 1;
                            continue;
                        }
                    }
                    if (count > 1)
                    {
                        blockDatas[x, y, z] = SquareData.FromLotpack(bytes, count - 1, ref position);
                    }
                }
            }
        }

        return blockDatas;
    }
}

public class SquareData
{
    public int[] Tiles { get; set; }

    public static SquareData FromLotpack(byte[] bytes, int count, ref int position)
    {
        position += 4; // drop room id as it is not used

        var blockData = new SquareData()
        {
            Tiles = new int[count],
        };

        for (int i = 0; i < count; i++)
        {
            blockData.Tiles[i] = bytes.ReadInt32(ref position);
        }

        return blockData;
    }
}