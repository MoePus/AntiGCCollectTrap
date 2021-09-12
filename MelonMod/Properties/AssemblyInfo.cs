using System.Reflection;
using MelonLoader;

[assembly: AssemblyTitle(AntiGCCollectTrap.BuildInfo.Description)]
[assembly: AssemblyDescription(AntiGCCollectTrap.BuildInfo.Description)]
[assembly: AssemblyCompany(AntiGCCollectTrap.BuildInfo.Company)]
[assembly: AssemblyProduct(AntiGCCollectTrap.BuildInfo.Name)]
[assembly: AssemblyCopyright("Created by " + AntiGCCollectTrap.BuildInfo.Author)]
[assembly: AssemblyTrademark(AntiGCCollectTrap.BuildInfo.Company)]
[assembly: AssemblyVersion(AntiGCCollectTrap.BuildInfo.Version)]
[assembly: AssemblyFileVersion(AntiGCCollectTrap.BuildInfo.Version)]
[assembly: MelonInfo(typeof(AntiGCCollectTrap.AntiGCCollectTrap), AntiGCCollectTrap.BuildInfo.Name, AntiGCCollectTrap.BuildInfo.Version, AntiGCCollectTrap.BuildInfo.Author, AntiGCCollectTrap.BuildInfo.DownloadLink)]
[assembly: MelonColor()]

// Create and Setup a MelonGame Attribute to mark a Melon as Universal or Compatible with specific Games.
// If no MelonGame Attribute is found or any of the Values for any MelonGame Attribute on the Melon is null or empty it will be assumed the Melon is Universal.
// Values for MelonGame Attribute can be found in the Game's app.info file or printed at the top of every log directly beneath the Unity version.
[assembly: MelonGame(null, null)]