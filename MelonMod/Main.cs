using MelonLoader;
using System;
using System.IO;
using System.Runtime.InteropServices;

namespace AntiGCCollectTrap
{
    public static class BuildInfo
    {
        public const string Name = "AntiGCCollectTrap"; // Name of the Mod.  (MUST BE SET)
        public const string Description = "Get rid of GC collecting from unknown thread error."; // Description for the Mod.  (Set as null if none)
        public const string Author = "MoePus"; // Author of the Mod.  (MUST BE SET)
        public const string Company = null; // Company that made the Mod.  (Set as null if none)
        public const string Version = "1.0.0"; // Version of the Mod.  (MUST BE SET)
        public const string DownloadLink = null; // Download Link for the Mod.  (Set as null if none)
    }
    static class NativeMethods
    {
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern IntPtr LoadLibrary(string dllToLoad);

        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern bool FreeLibrary(IntPtr hModule);
    }

    public class AntiGCCollectTrap : MelonMod
    {
        public override void OnApplicationStart() // Runs after Game Initialization.
        {
            string dllpath = Path.Combine(Directory.GetCurrentDirectory(), "AntiGCCollectTrapNative.dll");
            MelonLogger.Msg($"Load library {dllpath}");
            IntPtr dll = NativeMethods.LoadLibrary(dllpath);
            if (dll == IntPtr.Zero)
            {
                var error = Marshal.GetLastWin32Error();
                MelonLogger.Error("Cannot load native dll.");
                MelonLogger.Msg("The last Win32 Error was: " + error);
            }
        }
    }
}