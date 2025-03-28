#include <string>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Load(const std::string& fileName);
    void Unload();
    class VertexArray* GetVertexArray() { return mVertexArray; }
    class Texture* GetTexture(size_t index);
    class Shader* GetShader() { return mShader; }
    float GetRadius() const { return mRadius; }

private:
    class VertexArray* mVertexArray;
    class Shader* mShader;
    std::vector<class Texture*> mTextures;
    float mRadius;
};