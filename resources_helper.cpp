//
// Created by akann on 6/6/17.
//
#include "resources_helper.h"
#include "Framework/Loaders/textureloader.h"
#include "Framework/Loaders/meshloader.h"
#include "Logger/logger.h"

const auto logger = Logger::GetLogger("ResourcesHelper");

void ResourcesHelper::Init(StorageApi& api)
{
    loadTextures(api);
    loadMeshes(api);
    logger.LogInfo("Rejoice! All resources are loaded.");
}

void ResourcesHelper::loadTextures(StorageApi& api)
{
    TextureLoader textureLoader{api};
    MaterialProperties properties;

    properties.albedoTexture = true;
    properties.metallnessMap = true;
    properties.aoMap = true;
    properties.roughnessMap = true;
    properties.normalMap = true;

    textures.emplace(integral(Materials::Gold), Material{{textureLoader.LoadArray({"pbr/gold/normal.png", "pbr/gold/albedo.png",
                                                                                   "pbr/gold/metallic.png", "pbr/gold/roughness.png"})
                                                         , textureLoader.Load("pbr/gold/ao.png")}, properties});
    textures.emplace(integral(Materials::Grass), Material{{textureLoader.LoadArray({"pbr/grass/normal.png", "pbr/grass/albedo.png",
                                                                                    "pbr/grass/metallic.png", "pbr/grass/roughness.png"})
                                                          , textureLoader.Load("pbr/grass/ao.png")}, properties});
    textures.emplace(integral(Materials::Iron), Material{{textureLoader.LoadArray({"pbr/rusted_iron/normal.png", "pbr/rusted_iron/albedo.png",
                                                                                   "pbr/rusted_iron/metallic.png", "pbr/rusted_iron/roughness.png"})
                                                         , textureLoader.Load("pbr/rusted_iron/ao.png")}, properties});
    textures.emplace(integral(Materials::Plastic), Material{{textureLoader.LoadArray({"pbr/plastic/normal.png", "pbr/plastic/albedo.png",
                                                                                      "pbr/plastic/metallic.png", "pbr/plastic/roughness.png"})
                                                            , textureLoader.Load("pbr/plastic/ao.png")}, properties});
    textures.emplace(integral(Materials::Stone), Material{{textureLoader.LoadArray({"pbr/stone/octostoneNormalc.png", "pbr/stone/octostoneAlbedo.png",
                                                                                    "pbr/stone/octostoneMetallic.png", "pbr/stone/octostoneRoughness2.png"})
                                                          , textureLoader.Load("pbr/stone/octostoneAmbient_Occlusionc.png")}, properties});
    textures.emplace(integral(Materials::Fabric), Material{{textureLoader.LoadArray({"pbr/fabric/old-textured-fabric-normal.png", "pbr/fabric/old-textured-fabric-albedo3.png",
                                                                                    "pbr/fabric/old-textured-fabric-metalic.png", "pbr/fabric/old-textured-fabric-roughness2.png"})
                                                                  , textureLoader.Load("pbr/fabric/old-textured-fabric-ao.png")}, properties});

    std::string basePath = "pbr/aluminum/Aluminum-Scuffed_";
    properties.aoMap = false;
    textures.emplace(integral(Materials::Aluminum), Material{{textureLoader.LoadArray({basePath + "normal.png", basePath + "basecolor.png",
                                                                                       basePath + "metallic.png", basePath + "roughness.png"})}, properties});

    basePath = "pbr/wood_floor/mahogfloor_";
    properties.metallness = 0;
    properties.metallnessMap = false;
    properties.aoMap = true;
    textures.emplace(integral(Materials::WoodenFloor), Material{{textureLoader.LoadArray({basePath + "normal.png", basePath + "basecolor.png", basePath + "roughness.png"})
                                                                   , textureLoader.Load(basePath + "AO.png")}, properties});
}

void ResourcesHelper::loadMeshes(StorageApi& api)
{
    MeshLoader loader{api};
    meshes.emplace(integral(Meshes::Sofa), loader.Load("sofa/sofa.obj"));
    meshes.emplace(integral(Meshes::Pistol), loader.Load("cerberus/Cerberus_LP.FBX"));
    meshes.emplace(integral(Meshes::Cube), loader.Load("cube.obj"));
    meshes.emplace(integral(Meshes::Sphere), loader.Load("sphere.blend"));
    meshes.emplace(integral(Meshes::Table), loader.Load("table/table.obj"));

    TextureLoader textureLoader{api};
    Texture pistolTex = textureLoader.LoadArray({"cerberus/Textures/Cerberus_N.tga", "cerberus/Textures/Cerberus_A.tga",
                                                "cerberus/Textures/Cerberus_M.tga", "cerberus/Textures/Cerberus_R.tga"});
    Texture pistolAo = textureLoader.Load("cerberus/Textures/Cerberus_AO.tga");
    MaterialProperties properties;
    properties.albedoTexture = true;
    properties.metallnessMap = true;
    properties.aoMap = true;
    properties.roughnessMap = true;
    properties.normalMap = true;

    for (Mesh& mesh : meshes[integral(Meshes::Pistol)])
    {
        mesh.AddTexture(pistolTex);
        mesh.AddTexture(pistolAo);
        mesh.SetMaterialProperties(properties);
    }

    Texture tableWoodPart = textureLoader.LoadArray({"table/coffee_table_Menu_Turning_Table_wood_diffuse.png",
                                                     "table/coffee_table_Menu_Turning_Table_wood_specular.png"});
    Material aluminum = textures[integral(Materials::Aluminum)];
    MaterialProperties tableProperties;
    tableProperties.metallness = 0.6f;
    tableProperties.albedo = {1, 1, 1};
    tableProperties.roughness = 0.2f;
    for (int i = 0; i < 6; i++)
    {
        if (i == 1 || i == 3)
            continue;
        meshes[integral(Meshes::Table)][i].SetMaterialProperties(aluminum.properties);
        meshes[integral(Meshes::Table)][i].SetTextures(aluminum.texture);
    }

    tableProperties.albedoTexture = true;
    tableProperties.roughnessMap = true;
    tableProperties.inverseRoughness = true;
    tableProperties.metallness = 0;
    meshes[integral(Meshes::Table)][1].SetMaterialProperties(tableProperties);
    meshes[integral(Meshes::Table)][3].SetMaterialProperties(tableProperties);
    meshes[integral(Meshes::Table)][1].AddTexture(tableWoodPart);
    meshes[integral(Meshes::Table)][3].AddTexture(tableWoodPart);


    Texture sofa = textureLoader.LoadArray({"sofa/sofa_modular_UrbanCouture_Danielle_fabric_diffuse.jpg",
                                            "sofa/sofa_modular_UrbanCouture_Danielle_fabric_specular.jpg"});
    Texture aoLeft = textureLoader.Load("sofa/sofa_modular_UrbanCouture_Danielle_fabric_AO1.jpg");
    Texture aoRight = textureLoader.Load("sofa/sofa_modular_UrbanCouture_Danielle_fabric_AO2.jpg");
    MaterialProperties sofaProperties;
    sofaProperties.metallness = 0.0f;
    sofaProperties.roughness = 1.0f;
    sofaProperties.albedo = {1, 1, 1};
    meshes[integral(Meshes::Sofa)][3].SetMaterialProperties(sofaProperties);
    sofaProperties.albedoTexture = true;
    sofaProperties.metallnessMap = false;
    sofaProperties.roughnessMap = true;
    sofaProperties.inverseRoughness = true;
    sofaProperties.metallness = 0;
    meshes[integral(Meshes::Sofa)][2].SetMaterialProperties(sofaProperties);
    for (int i = 0; i < 3; i++)
        meshes[integral(Meshes::Sofa)][i].AddTexture(sofa);
    sofaProperties.aoMap = true;
    meshes[integral(Meshes::Sofa)][0].SetMaterialProperties(sofaProperties);
    meshes[integral(Meshes::Sofa)][1].SetMaterialProperties(sofaProperties);
    meshes[integral(Meshes::Sofa)][1].AddTexture(aoLeft);
    meshes[integral(Meshes::Sofa)][0].AddTexture(aoRight);
}

std::vector<Mesh>& ResourcesHelper::GetMesh(Meshes id)
{
    return meshes[integral(id)];
}

Material ResourcesHelper::GetMaterial(Materials id)
{
    return textures[integral(id)];
}
