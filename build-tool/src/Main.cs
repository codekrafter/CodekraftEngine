using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;

namespace ckb
{
    class Program
    {

        ProgramSettings settings = new ProgramSettings();

        void PrintFatal(string message, int code = -1)
        {
            Console.ForegroundColor = ConsoleColor.DarkRed;
            Console.WriteLine(message);
            Console.ResetColor();
            Environment.Exit(code);
        }

        void PrintFatalSoft(string message)
        {
            Console.ForegroundColor = ConsoleColor.DarkRed;
            Console.WriteLine(message);
            Console.ResetColor();
        }

        void PrintError(string message)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(message);
            Console.ResetColor();
        }

        void PrintWarning(string message)
        {
            Console.ForegroundColor = ConsoleColor.DarkYellow;
            Console.WriteLine(message);
            Console.ResetColor();
        }

        void PrintVerbose(string message)
        {
            if (!settings.verbose)
                return;
            Console.WriteLine(message);
        }

        static int Main(string[] args)
        {
            Program P = new Program();
            int code = P.Start(args);
            return code;
        }
        int Start(string[] args)
        {
            if (args.Length == 0)
            {
                PrintFatal("No Arguments Provided");
            }
            List<string> ToParse = new List<string>();
            List<string> NonOpts = new List<string>();

            // Simple Pre-Parsing and syntax checking
            foreach (string a in args)
            {
                // Eliminate 3+ dashes prefix
                if (a.Length > 2 && a.Substring(0, 3) == "---")
                {
                    PrintFatal("Error Parsing Argument '" + a + "': 3+ dash prefix");
                }

                if (a.Length > 1 && a.Substring(0, 2) == "--")
                {
                    ToParse.Add(a.Substring(2));
                    continue;
                }

                if (a.Substring(0, 1) == "-")
                {
                    foreach (char c in a.Substring(1))
                    {
                        Console.WriteLine(c);
                        ToParse.Add(c.ToString());
                    }
                    continue;
                }

                NonOpts.Add(a);
            }

            // Parse the Arguments
            bool shouldDie = false;
            foreach (string a in ToParse)
            {
                switch (a)
                {
                    case "engine":
                    case "e":
                        settings.engine = true;
                        break;
                    case "v":
                    case "verbose":
                        settings.verbose = true;
                        break;
                    case "i":
                    case "install":
                        settings.install = true;
                        break;
                    default:
                        PrintFatalSoft("Error Parsing Argument '" + a + "': unrecognized argument");
                        shouldDie = true;
                        break;
                }
                if (shouldDie)
                {
                    return -1;
                }
            }

            // For now we only want one target, but it is a list for future compatibility
            if (NonOpts.Count > 1)
            {
                PrintFatal("Error Parsing Argument '" + NonOpts[1] + "': unrecognized extra argument");
                return -1;
            }
            string path = NonOpts[0];
            string dir = "";
            if ((File.GetAttributes(path) & FileAttributes.Directory) == FileAttributes.Directory)
            {
                string[] files = Directory.GetFiles(path, "*.ckproj");
                if (files.Length == 0)
                {
                    PrintFatal("No project configuration file in target directory(" + path + ")");
                }
                if (files.Length > 1)
                {
                    PrintWarning("More than 1 project configuration file in directory '" + path + "', using first entry(" + files[0] + ")");
                }
                dir = path;
                path = files[0];
            }
            string[] parts = path.Split(".");
            if (parts[parts.Length - 1] != "ckproj")
            {
                PrintFatal("Target File '" + path + "' is not a project configuration file");
            }
            string configJSON = File.ReadAllText(path);
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                settings.platform = OSPlatform.Linux;
            }
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                settings.platform = OSPlatform.Windows;
            }
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
            {
                settings.platform = OSPlatform.OSX;
            }
            else
            {
                settings.platform = OSPlatform.Windows;
                PrintWarning("Unknow Platform '" + RuntimeInformation.OSDescription + "' Using Windows");
            }

            ProjectSettings proj = new ProjectSettings();
            if (dir == "")
            {
                string[] dira = path.Split("/", StringSplitOptions.RemoveEmptyEntries);
                foreach (string a in dira)
                {
                    dir = dir + a;
                }
            }

            string confString = File.ReadAllText(path);
            proj.parse(confString, settings);
            BuildSelection bs = new BuildSelection(dir + "/src");
            return 0;
        }
    }
}
