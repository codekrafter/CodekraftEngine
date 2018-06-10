using System.IO;
using System.Collections.Generic;
using System;

namespace ckb
{
    class BuildSelection
    {
        public BuildSelection(string dir)
        {
            files = recurseGetSRC(dir);

            foreach (string file in files)
            {
                Console.WriteLine(file);
            }
        }

        private List<string> recurseGetSRC(string path)
        {
            List<string> fs = new List<string>(Directory.GetFiles(path));
            List<string> ofs = new List<string>();
            foreach (string f in fs.ToArray())
            {
                if ((File.GetAttributes(f) & FileAttributes.Directory) == FileAttributes.Directory)
                {
                    List<string> nfs = recurseGetSRC(f);
                    ofs.AddRange(nfs);
                }
                else
                {
                    string[] parts = f.Split(".", StringSplitOptions.RemoveEmptyEntries);
                    if (parts[parts.Length - 1] == ".cpp" || parts[parts.Length - 1] == ".c")
                    {
                        ofs.Add(f);
                    }
                }
            }
            return ofs;
        }
        List<string> files;
        List<string> includePaths;

        List<string> libPaths;

        List<string> rootDir;
    }
}