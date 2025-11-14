using System;

public class Program
{
    public static void Main(string[] args)
    {
        var header = LotheaderFile.Read("ignore/27_38.lotheader");
        var lotpack = LotpackFile.Read("ignore/world_27_38.lotpack", header);
    }

    public static void ReadTmxFile()
    {
        var txmFile = TmxFile.Read("ignore/Challenge1/tmx/0_0.tmx");

        txmFile.GetGID("0_FloorOverlay", 152, 163);

        // txmFile.Clear(128, 177, 130, 182);
        // txmFile.Clear(148, 151, 162, 161);
        // txmFile.Save("ignore/Challenge1/tmx/0_0.parsed.tmx");
    }
}