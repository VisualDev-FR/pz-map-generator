using System.Diagnostics;

public static class Utils
{
    public static Stopwatch StartTimer()
    {
        var stopwatch = new Stopwatch();
        stopwatch.Start();

        return stopwatch;
    }

}