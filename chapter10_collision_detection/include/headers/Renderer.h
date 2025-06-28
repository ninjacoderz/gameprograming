#include <string>
#include <vector>
#include <unordered_map>
#include <SDL3/SDL.h>
#include "GameMath.h"

struct DirectionalLight
{
	// Direction of light
	Vector3 mDirection;
	// Diffuse color
	Vector3 mDiffuseColor;
	// Specular color
	Vector3 mSpecColor;
};

class Renderer {
public:
    Renderer(class Game* game);
    ~Renderer();
    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    void UnloadData();
    void Draw();

    void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

    class Texture* GetTexture(const std::string& fileName);
    class Mesh* GetMesh(const std::string& fileName);
    void AddMeshComp(class MeshComponent* mesh);
    void RemoveMeshComp(class MeshComponent* mesh);
    void SetViewMatrix(const Matrix4& view) { mView = view; }
    void SetProjectionMatrix(const Matrix4& proj) { mProjection = proj; }
    void SetLightUniforms(class Shader* shader);
    void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
    DirectionalLight& GetDirectionalLight() { return mDirLight; }
	SDL_Window* getWindow() const { return mWindow; }

    Vector3 Unproject(const Vector3& screenPoint) const;
private:
    bool LoadShaders();
    void CreateSpriteVerts();
    // Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;
    // Game
	class Game* mGame;
    // Mesh shader
    class Shader* mMeshShader;
    // Sprite shader
	class Shader* mSpriteShader;
    // Map of meshes loaded
	std::unordered_map<std::string, class Mesh*> mMeshes;
    // All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;
    // Sprite vertex array
	class VertexArray* mSpriteVerts;
    // All mesh components drawn
	std::vector<class MeshComponent*> mMeshComps;

    // Width/height of screen
	float mScreenWidth;
	float mScreenHeight;

    // Window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;

    // View/projection for 3D shaders
    Matrix4 mView;
    Matrix4 mProjection;
    
    // Lighting data
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;
};