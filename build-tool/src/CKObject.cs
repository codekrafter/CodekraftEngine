using System.Collections.Generic;
using System.Json;

namespace ckb
{
    public class Variable
    {
        public string name = "";
        public string type = "";
        public string defaultVal = "";
        public override string ToString()
        {
            return "{name='" + name + "',type='" + type + "',defaultVal='" + defaultVal + "'}";
        }
        public string size
        {
            get
            {
                switch (type)
                {
                    case "std::string":
                        return name + ".size() + 1 + sizeof(SizeS)";
                    default:
                        return "sizeof(" + type + ")";
                }
            }
        }
    }

    public struct Event
    {
        public string label;
        public string callableName;
        public Event(string callableName)
        {
            label = "LABEL: " + callableName;
            this.callableName = callableName;
        }
    }
    public class CKObject
    {
        public string name = "";

        // Name : Type
        public List<Variable> vars = new List<Variable>();

        // Label : Callable Name
        public List<Event> events = new List<Event>();

        public override string ToString()
        {
            string output = "{name=\"" + name + "\",vars=[";
            foreach (var var in vars)
            {
                output = output + var.ToString();
            }
            output = output + "],events=[";
            foreach (var Event in events)
            {
                output = output + Event.ToString();
            }
            output = output + "]}";
            return output;
        }

    }
}