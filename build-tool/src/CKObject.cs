using System.Collections.Generic;
using System.Json;

namespace ckb
{
    public class CKObject
    {
        public string name = "";

        // Name : Type
        public Dictionary<string, string> vars = new Dictionary<string, string>();

        // Label : Callable Name
        public Dictionary<string, string> events = new Dictionary<string, string>();

        public override string ToString()
        {
            string output = "{name=\"" + name + "\",vars=[";
            foreach (var pair in vars)
            {
                output = output + "{\"" + pair.Key + "\",\"" + pair.Value + "\"},";
            }
            output = output.Substring(0, output.Length - 1);
            output = output + "],events=[";
            foreach (var pair in events)
            {
                output = output + "{\"" + pair.Key + "\",\"" + pair.Value + "\"},";
            }
            output = output.Substring(0, output.Length - 1);
            output = output + "]}";
            return output;
        }

    }
}