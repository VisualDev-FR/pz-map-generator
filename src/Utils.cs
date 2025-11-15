using System.Diagnostics;
using System.Text;

public static class Utils
{
    public static Stopwatch StartTimer()
    {
        var stopwatch = new Stopwatch();
        stopwatch.Start();

        return stopwatch;
    }

    public static byte[] Encode(string text, Encoding encoding = null)
    {
        if (encoding == null)
            encoding = Encoding.UTF8;

        return encoding.GetBytes(text);
    }
}