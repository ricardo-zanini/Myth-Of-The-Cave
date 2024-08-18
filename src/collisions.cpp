#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "matrices.h"
#include "objModel.h"
#include "MinMax.h"
#include "SceneObject.h"

void collision_player_plane_points(
    glm::mat4 model, 
    int indice, 
    ObjModel* ObjModel, 
    float player_X, 
    float player_Y, 
    float player_Z, 
    glm::vec4 camera_view_vector,
    bool *movement_restricted,
    glm::vec4 *movement_normal
){
    // Tamanho do cubo do jogador
    float range_cubo = 0.5f;

    // Centro do Jogador
    glm::vec4 player_center_point = glm::vec4(player_X, player_Y + 1.0f, player_Z, 1.0f);

    std::vector<GLuint> indices;
    size_t first_index = indices.size();
    size_t num_triangles = ObjModel->shapes[indice].mesh.num_face_vertices.size();

    glm::vec4 view_aux  = glm::vec4(camera_view_vector.x, 0.0f, camera_view_vector.z, 0.0f);

    for (size_t triangle = 0; triangle < num_triangles; ++triangle)
    {
        //------------------------ PONTO A ---------------------------------

        tinyobj::index_t idx1 = ObjModel->shapes[indice].mesh.indices[3*triangle];

        indices.push_back(first_index + 3*triangle);

        glm::vec4 pA_aux = glm::vec4(
            ObjModel->attrib.vertices[3*idx1.vertex_index + 0],
            ObjModel->attrib.vertices[3*idx1.vertex_index + 1],
            ObjModel->attrib.vertices[3*idx1.vertex_index + 2],
            1.0f
        );

        auto pA = model*pA_aux;

        //------------------------ PONTO B ---------------------------------

        tinyobj::index_t idx2 = ObjModel->shapes[indice].mesh.indices[3*triangle + 1];

        indices.push_back(first_index + 3*triangle + 1);

        glm::vec4 pB_aux = glm::vec4(
            ObjModel->attrib.vertices[3*idx2.vertex_index + 0],
            ObjModel->attrib.vertices[3*idx2.vertex_index + 1],
            ObjModel->attrib.vertices[3*idx2.vertex_index + 2],
            1.0f
        );

        auto pB = model*pB_aux;

        //------------------------ PONTO C ---------------------------------

        tinyobj::index_t idx3 = ObjModel->shapes[indice].mesh.indices[3*triangle + 2];

        indices.push_back(first_index + 3*triangle + 2);

        glm::vec4 pC_aux = glm::vec4(
            ObjModel->attrib.vertices[3*idx3.vertex_index + 0],
            ObjModel->attrib.vertices[3*idx3.vertex_index + 1],
            ObjModel->attrib.vertices[3*idx3.vertex_index + 2],
            1.0f
        );

        auto pC = model*pC_aux;

        //------------------------------------------------------------------

        // busca-se por dois pontos para representar o antigo triangulo como um cubo,
        // como as paredes são planas e alinhadas com os eixos X e Z não temos problemas
        glm::vec4 min_XZ = glm::vec4(min3(pA.x, pB.x, pC.x), min3(pA.y, pB.y, pC.y), min3(pA.z, pB.z, pC.z), 1.0f);
        glm::vec4 max_XZ = glm::vec4(max3(pA.x, pB.x, pC.x), max3(pA.y, pB.y, pC.y), max3(pA.z, pB.z, pC.z), 1.0f);
        glm::vec4 normal_XZ = crossproduct((pA - pB), (pC - pB));

        float margem = 0.5f;
        float margem_plano = 20.0f;

        // Verifica se o vértice se encontra nos limites do plano
        // (usa-se uma margem pois os planos allinhados com os eixos X e Z podem ter min.x e max.x iguais, oque dificulta a igualdade visto que utilizamos float)
        if(
            player_center_point.x >= min_XZ.x - margem
            && player_center_point.z >= min_XZ.z - margem
            && player_center_point.x <= max_XZ.x + margem
            && player_center_point.z <= max_XZ.z + margem
        ){
            float pertence_plano = dotproduct((player_center_point - max_XZ), normal_XZ);

            // Verifica-se se o ponto está em [0,1] na equação do plano
            // (Gostaria de entender porque essa "margem_2" é necessária ;-;)
            // - Ricardo depois de 3h nesse problema
            if(pertence_plano >= 0.0f - margem_plano && pertence_plano <= 1.0f + margem_plano){
                *movement_restricted = true;
                *movement_normal = glm::vec4(normal_XZ.x, 0.0f, normal_XZ.z, 0.0f);

                if(dotproduct(view_aux, *movement_normal) > 0){
                    *movement_normal = -(*movement_normal);
                }
            }
        }
    }
}

void collision_player_sphere_points(
    glm::mat4 model,
    char* object,
    float radius,
    float player_X, 
    float player_Y, 
    float player_Z, 
    bool *movement_restricted,
    glm::vec4 *movement_normal,
    std::map<std::string, SceneObject> g_VirtualScene
){
        // Tamanho do cubo do jogador
    float range_cubo = 0.5f;

    // Centro do Jogador
    glm::vec4 player_center_point = glm::vec4(player_X, player_Y + 1.0f, player_Z, 1.0f);

    // Procura e calcula coordenadas mínima e máxima do modelo (aplicando matriz de transformação)
    glm::vec3 bbox_min = g_VirtualScene[object].bbox_min;
    glm::vec3 bbox_max = g_VirtualScene[object].bbox_max;

    glm::vec4 coords_min =  glm::vec4(bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glm::vec4 coords_max =  glm::vec4(bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);

    auto coords_min_T = model * coords_min;
    auto coords_max_T = model * coords_max;

    // Encontra o centro do modelo
    glm::vec4 sphere_center = glm::vec4(
        coords_min_T.x + ((coords_max_T.x - coords_min_T.x) / 2),
        coords_min_T.y + ((coords_max_T.y - coords_min_T.y) / 2),
        coords_min_T.z + ((coords_max_T.z - coords_min_T.z) / 2),
        1.0f
    );

    // Verifica se o centro do player colide com uma esfera
    if(norm(player_center_point - sphere_center) <= radius){
        *movement_restricted = true;
        *movement_normal = (player_center_point - sphere_center);
        *movement_normal = glm::vec4(movement_normal->x, 0.0f, movement_normal->z, 0.0f);
    }
}

void collision_player_box_points(
    glm::mat4 model, 
    char* object,
    float player_X, 
    float player_Y, 
    float player_Z, 
    bool *movement_restricted,
    glm::vec4 *movement_normal,
    std::map<std::string, SceneObject> g_VirtualScene
){

    // Centro do Jogador
    glm::vec4 player_center_point = glm::vec4(player_X, player_Y + 1.0f, player_Z, 1.0f);

    // Procura e calcula coordenadas mínima e máxima do modelo (aplicando matriz de transformação)
    glm::vec3 bbox_min = g_VirtualScene[object].bbox_min;
    glm::vec3 bbox_max = g_VirtualScene[object].bbox_max;

    glm::vec4 coords_min =  glm::vec4(bbox_min.x - 10, bbox_min.y - 10, bbox_min.z - 10, 1.0f);
    glm::vec4 coords_max =  glm::vec4(bbox_max.x + 10, bbox_max.y + 10, bbox_max.z + 10, 1.0f);

    auto coords_min_T = model * (coords_min);
    auto coords_max_T = model * coords_max;

    glm::vec4 box_center = glm::vec4(
        coords_min_T.x + ((coords_max_T.x - coords_min_T.x) / 2),
        coords_min_T.y + ((coords_max_T.y - coords_min_T.y) / 2),
        coords_min_T.z + ((coords_max_T.z - coords_min_T.z) / 2),
        1.0f
    );

    // Verifica se o player entra nos limites do cubo
    if(
           player_center_point.x >= coords_min_T.x
        && player_center_point.y >= coords_min_T.y
        && player_center_point.z >= coords_min_T.z

        && player_center_point.x <= coords_max_T.x
        && player_center_point.y <= coords_max_T.y
        && player_center_point.z <= coords_max_T.z
    ){
        printf("min ---> x:%f // y: %f // z:%f // w: %f \n", player_X, player_Y + 1.0f, player_Z, 1.0f);
        printf("min ---> x:%f // y: %f // z:%f // w: %f \n", coords_min_T.x, coords_min_T.y, coords_min_T.z, coords_min_T.w);
        printf("max ---> x:%f // y: %f // z:%f // w: %f \n\n", coords_max_T.x, coords_max_T.y, coords_max_T.z, coords_max_T.w);
        *movement_restricted = true;
        *movement_normal = (player_center_point - box_center);
        *movement_normal = glm::vec4(movement_normal->x, 0.0f, movement_normal->z, 0.0f);
    }
}