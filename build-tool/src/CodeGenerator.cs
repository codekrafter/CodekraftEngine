using System;
using System.Collections.Generic;
using System.IO;
using Scriban;

namespace ckb
{
    public class CodeGenerator
    {
        Dictionary<SourceFile, List<CKObject>> filesDict;
        public CodeGenerator(Dictionary<SourceFile, List<CKObject>> fd)
        {
            filesDict = fd;
        }

        public void generate(string dir)
        {
            // Cleanse Path
            string path = Environment.CurrentDirectory + "/" + dir;
            path = Path.GetRelativePath(".", path);

            foreach (var pair in filesDict)
            {
                // Retrive file and objects for easy access
                SourceFile file = pair.Key;
                List<CKObject> objs = pair.Value;

                var header = Template.Parse(@"
                #include ""Serialization.hpp""
                
                {{ for obj in objects }}
                    <h2>{{ product.name }}</h2>
                    Price: {{ product.price }}
                    {{ product.description | string.truncate 15 }}
                {{ end }}
                
");
                var source = Template.Parse(@"
                #include ""{{- include_name -}}""                
");

                var header_o = header.Render(new { objects = objs });

                var source_o = source.Render(new { include_name = file.name + ".generated.hpp" });
            }
        }
    }
}
