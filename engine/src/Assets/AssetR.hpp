/*
 * A RAsset or Raw Asset is an asset that has an external repersention in disk.
 * For example a mesh(.obj), a texture(.png), or even a shader(raw string, not a file)
 */

class RAsset : public virtual Asset
{
};

/*
 * A Importer takes in some raw asset and converts it to a RAsset object of that type
 * (.png->Texture or .obj->StaticMesh)
 */