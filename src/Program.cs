using System;
using System.IO;

public class Program
{
    const string mapPath = "C:/SteamLibrary/steamapps/common/ProjectZomboidB41/media/maps/Muldraugh, KY";

    public static void Main(string[] args)
    {
        // var header = LotheaderFile.Read("ignore/27_38.lotheader");
        // var lotpack = LotpackFile.Read("ignore/world_27_38.lotpack", header);

        ReadAllMapFiles();
    }

    public static void ReadAllMapFiles()
    {
        var totalTimer = Utils.StartTimer();
        var filesCount = 0;

        for (int x = 0; x < 99; x++)
        {
            for (int y = 0; y < 99; y++)
            {
                string headerPath = $"{mapPath}/{x}_{y}.lotheader";
                string lotpackPath = $"{mapPath}/world_{x}_{y}.lotpack";

                if (!Path.Exists(headerPath))
                    continue;

                var header = LotheaderFile.Read(headerPath);
                var lotpack = LotpackFile.Read(lotpackPath, header);

                filesCount++;
            }
        }

        Console.WriteLine($"{filesCount} read in {totalTimer.ElapsedMilliseconds / 1000:F3}s (average = {totalTimer.ElapsedMilliseconds / filesCount}ms / file)");
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