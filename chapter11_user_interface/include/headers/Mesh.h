#include <string>
#include "Collision.h"
#include <vector>
class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Load(const std::string& fileName, class Renderer* renderer);
    void Unload();
    class VertexArray* GetVertexArray() { return mVertexArray; }
    class Texture* GetTexture(size_t index);
    class Shader* GetShader() { return mShader; }
    float GetRadius() const { return mRadius; }
    float GetSpecPower() const { return mSpecPower; }
    const AABB& GetBox() const { return mBox; }
private:
    class VertexArray* mVertexArray;
    class Shader* mShader;
    std::vector<class Texture*> mTextures;
    float mRadius;
    std::string mShaderName;
    float mSpecPower;

    // AABB collision
	AABB mBox;
};