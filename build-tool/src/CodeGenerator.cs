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
/*
 * Generated Code for file '{{file.name}}' by Codekraft Build Tool
 * Do not edit
 */

#include ""Serialization.hpp""

namespace ckg
{

{{ for obj in objects -}}

struct {{obj.name}}S : AssetS
{
    /*
     * Variables, one-to-one to real class
     */

    {{ for var in obj.vars }}
    {{ var.type }} {{ var.name }} {{-if var.default_val | string.size > 0}} = {{ var.default_val }} {{- end }};
    {{end}}

    /*
     * Serialization Functions, required for serialization to work,
     * and to convert between normal class ({{obj.name}})
     */
    {{obj.name}}S();
    {{obj.name}}S({{obj.name}} *{{obj.name | string.slice 0 1 | string.downcase}});
    virtual {{obj.name}} *asset();
    virtual DatSize save();
    virtual void load(unsigned char *data, size_t size);
};

{{- end }}
}

/*
 * End of File
 */
");
                var source = Template.Parse(@"
#include ""{{- include_name -}}""

namespace ckg
{
{{ for obj in objects -}}

{{obj.name}}S::{{obj.name}}S(){};

{{obj.name}}S::{{obj.name}}S({{obj.name}} *obj)
{

    {{ for var in obj.vars }}
    {{ var.name }} = obj->{{ var.name }};
    {{end}}
};

{{obj.name}} *{{obj.name}}S::asset()
{
    {{obj.name}} *obj = new {{obj.name}}();
    {{ for var in obj.vars }}
    obj->{{ var.name }} = {{ var.name }};
    {{end}}

    return obj;
};

DatSize {{obj.name}}S::save()
{
    size_t size;
    {{ for var in obj.vars }}
    size += {{ var.size }};
    {{end}}
    START_SAVE()
    {{ for var in obj.vars }}
    S_{{var.type | string.upcase | string.replace ""::"" ""_"" }}()
    {{end}}
    END_SAVE()
};

void {{obj.name}}S::load(unsigned char *data, size_t size){
    START_LOAD()
    {{ for var in obj.vars }}
    L_{{var.type | string.upcase | string.replace ""::"" ""_"" }}()
    {{end}}
    END_LOAD()

};

{{- end }}
}

/*
 * End of File
 */            
");

                var header_o = header.Render(new { objects = objs, file = file });

                var source_o = source.Render(new { include_name = Path.GetFileNameWithoutExtension(file.name) + ".generated.hpp", objects = objs });

                var header_p = Path.GetFullPath(dir + Path.GetFileNameWithoutExtension(file.name) + ".generated.hpp");

                var source_p = Path.GetFullPath(dir + Path.GetFileNameWithoutExtension(file.name) + ".generated.cpp");

                Directory.CreateDirectory(dir);

                File.WriteAllText(header_p, header_o);

                File.WriteAllText(source_p, source_o);

            }
        }
    }
}
