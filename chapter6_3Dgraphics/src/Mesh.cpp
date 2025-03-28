#include "Mesh.h"
#include <fstream>
#include <sstream>

Mesh::Mesh():
    mVertexArray(nullptr),
    mRadius(0.0f)
{
}

Mesh::~Mesh()
{
}   
bool Mesh::Load(const std::string& fileName, Renderer* renderer)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        SDL_Log("File not found: Mesh %s", fileName.c_str());
        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject())
    {
        SDL_Log("Mesh %s is not valid json", fileName.c_str());
        return false;
    }

    int ver = doc["version"].GetInt();

    // Check the version
    if (ver != 1)
    {
        SDL_Log("Mesh %s not version 1", fileName.c_str());
        return false;
    }

    mShaderName = doc["shader"].GetString();

    // Skip the vertex format/shader for now
    // (This is changed in a later chapter's code)
    size_t vertSize = 8;

    // Load the vertices
    const rapidjson::Value& vertsJson = doc["vertices"];
    if (!vertsJson.IsArray() || vertsJson.Size() < 1)
    {
        SDL_Log("Mesh %s has no vertices", fileName.c_str());
        return false;
    }

    std::vector<float> vertices;
    vertices.reserve(vertsJson.Size() * vertSize);
    mRadius = 0.0f;
    for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
    {
        // For now, just assume we have 8 elements
        const rapidjson::Value& vert = vertsJson[i];
        if (!vert.IsArray() || vert.Size() != 8)
        {
            SDL_Log("Unexpected vertex format for %s", fileName.c_str());
            return false;
        }

        Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
        mRadius = Math::Max(mRadius, pos.LengthSq());

        // Add the floats
        for (rapidjson::SizeType i = 0; i < vert.Size(); i++)
        {
            vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
        }
    }

    // We were computing length squared earlier
    mRadius