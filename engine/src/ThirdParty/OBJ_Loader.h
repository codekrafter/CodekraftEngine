// OBJ_Loader.h
//

#ifndef LZZ_OBJ_Loader_h
#define LZZ_OBJ_Loader_h
// Iostream - STD I/O Library
#include <iostream>

// Vector - STD Vector/Array Library
#include <vector>

// String - STD String Library
#include <string>

// fStream - STD File I/O Library
#include <fstream>

// Math.h - STD math Library
#include <math.h>

// Print progress to console while loading (large models)
//#define OBJL_CONSOLE_OUTPUT
#define LZZ_INLINE inline
namespace objl
{
  struct Vector2
  {
    Vector2 ();
    Vector2 (float X_, float Y_);
    bool operator == (Vector2 const & other) const;
    bool operator != (Vector2 const & other) const;
    Vector2 operator + (Vector2 const & right) const;
    Vector2 operator - (Vector2 const & right) const;
    Vector2 operator * (float const & other) const;
    float X;
    float Y;
  };
}
namespace objl
{
  struct Vector3
  {
    Vector3 ();
    Vector3 (float X_, float Y_, float Z_);
    bool operator == (Vector3 const & other) const;
    bool operator != (Vector3 const & other) const;
    Vector3 operator + (Vector3 const & right) const;
    Vector3 operator - (Vector3 const & right) const;
    Vector3 operator * (float const & other) const;
    float X;
    float Y;
    float Z;
  };
}
namespace objl
{
  struct Vertex
  {
    Vector3 Position;
    Vector3 Normal;
    Vector2 TextureCoordinate;
  };
}
namespace objl
{
  struct Material
  {
    Material ();
    std::string name;
    Vector3 Ka;
    Vector3 Kd;
    Vector3 Ks;
    float Ns;
    float Ni;
    float d;
    int illum;
    std::string map_Ka;
    std::string map_Kd;
    std::string map_Ks;
    std::string map_Ns;
    std::string map_d;
    std::string map_bump;
  };
}
namespace objl
{
  struct Mesh
  {
    Mesh ();
    Mesh (std::vector <Vertex> & _Vertices, std::vector <unsigned int> & _Indices);
    std::string MeshName;
    std::vector <Vertex> Vertices;
    std::vector <unsigned int> Indices;
    Material MeshMaterial;
  };
}
namespace objl
{
  namespace math
  {
    Vector3 CrossV3 (Vector3 const a, Vector3 const b);
  }
}
namespace objl
{
  namespace math
  {
    float MagnitudeV3 (Vector3 const in);
  }
}
namespace objl
{
  namespace math
  {
    float DotV3 (Vector3 const a, Vector3 const b);
  }
}
namespace objl
{
  namespace math
  {
    float AngleBetweenV3 (Vector3 const a, Vector3 const b);
  }
}
namespace objl
{
  namespace algorithm
  {
    Vector3 operator * (float const & left, Vector3 const & right);
  }
}
namespace objl
{
  namespace algorithm
  {
    bool inTriangle (Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3);
  }
}
namespace objl
{
  namespace algorithm
  {
    void split (std::string const & in, std::vector <std::string> & out, std::string token);
  }
}
namespace objl
{
  namespace algorithm
  {
    std::string tail (std::string const & in);
  }
}
namespace objl
{
  namespace algorithm
  {
    std::string firstToken (std::string const & in);
  }
}
namespace objl
{
  namespace algorithm
  {
    template <typename T>
    T const & getElement (std::vector <T> const & elements, std::string & index);
  }
}
namespace objl
{
  class Loader
  {
  public:
    Loader ();
    ~ Loader ();
    bool LoadFile (std::string Path);
    std::vector <Mesh> LoadedMeshes;
    std::vector <Vertex> LoadedVertices;
    std::vector <unsigned int> LoadedIndices;
    std::vector <Material> LoadedMaterials;
  private:
    void GenVerticesFromRawOBJ (std::vector <Vertex> & oVerts, std::vector <Vector3> const & iPositions, std::vector <Vector2> const & iTCoords, std::vector <Vector3> const & iNormals, std::string icurline);
    void VertexTriangluation (std::vector <unsigned int> & oIndices, std::vector <Vertex> const & iVerts);
    bool LoadMaterials (std::string path);
  };
}
namespace objl
{
  namespace algorithm
  {
    LZZ_INLINE void split (std::string const & in, std::vector <std::string> & out, std::string token)
                {
			out.clear();

			std::string temp;

			for (int i = 0; i < int(in.size()); i++)
			{
				std::string test = in.substr(i, token.size());

				if (test == token)
				{
					if (!temp.empty())
					{
						out.push_back(temp);
						temp.clear();
						i += (int)token.size() - 1;
					}
					else
					{
						out.push_back("");
					}
				}
				else if (i + token.size() >= in.size())
				{
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else
				{
					temp += in[i];
				}
			}
		}
  }
}
namespace objl
{
  namespace algorithm
  {
    LZZ_INLINE std::string tail (std::string const & in)
                {
			size_t token_start = in.find_first_not_of(" \t");
			size_t space_start = in.find_first_of(" \t", token_start);
			size_t tail_start = in.find_first_not_of(" \t", space_start);
			size_t tail_end = in.find_last_not_of(" \t");
			if (tail_start != std::string::npos && tail_end != std::string::npos)
			{
				return in.substr(tail_start, tail_end - tail_start + 1);
			}
			else if (tail_start != std::string::npos)
			{
				return in.substr(tail_start);
			}
			return "";
		}
  }
}
namespace objl
{
  namespace algorithm
  {
    LZZ_INLINE std::string firstToken (std::string const & in)
                {
			if (!in.empty())
			{
				size_t token_start = in.find_first_not_of(" \t");
				size_t token_end = in.find_first_of(" \t", token_start);
				if (token_start != std::string::npos && token_end != std::string::npos)
				{
					return in.substr(token_start, token_end - token_start);
				}
				else if (token_start != std::string::npos)
				{
					return in.substr(token_start);
				}
			}
			return "";
		}
  }
}
namespace objl
{
  namespace algorithm
  {
    template <typename T>
    LZZ_INLINE T const & getElement (std::vector <T> const & elements, std::string & index)
                {
			int idx = std::stoi(index);
			if (idx < 0)
				idx = int(elements.size()) + idx;
			else
				idx--;
			return elements[idx];
		}
  }
}
#undef LZZ_INLINE
#endif
