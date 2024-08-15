#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

in vec3 tangent;
in vec3 bitangent;
in vec3 crossEdge;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define CAMPFIRE  0
#define FIRE 1
#define CAVE 2
#define CAVE_WALLS 3
#define CAVE_FLOOR 4
#define GRUTA_BASE 5
#define GREEK_BODY 6
#define GREEK_HEAD 7
#define TITLE 8
#define PRISIONER 9
#define PRISIONER_EYES 10
#define PRISIONER_TEETH 11
#define PRISIONER_ROCK 12
#define PRISIONER_CHAIN 13
#define LADDER 14
#define GREEK2 15
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImageCampfire;
uniform sampler2D TextureNormalCampfire;
uniform sampler2D TextureImageFire;
uniform sampler2D TextureImageCaveWalls;
uniform sampler2D TextureNormalCaveWalls;
uniform sampler2D TextureImageCaveFloor;
uniform sampler2D TextureNormalCaveFloor;
uniform sampler2D TextureImageGruta;
uniform sampler2D TexturNormalGruta;
uniform sampler2D TextureImageGreekBody;
uniform sampler2D TextureImageGreekHead;
uniform sampler2D TextureImageTitle;
uniform sampler2D TextureImagePrisioner;
uniform sampler2D TextureNormalPrisioner;
uniform sampler2D TextureImagePrisionerEyes;
uniform sampler2D TextureImagePrisionerTeeth;
uniform sampler2D TextureImagePrisionerRock;
uniform sampler2D TextureNormalPrisionerRock;
uniform sampler2D TextureImagePrisionerChain;
uniform sampler2D TextureNormalPrisionerChain;
uniform sampler2D TextureImageLadder;
uniform sampler2D TextureNormalLadder;
uniform sampler2D TextureImageGreek2;
uniform sampler2D TextureNormalGreek2;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    //------------------------------- FONTE: https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%2027%20-%20Normal%20Maps/default.geom

    vec4 T = normalize(vec4(normal * vec4(tangent, 0.0f)));
    vec4 B = normalize(vec4(normal * vec4(bitangent, 0.0f)));
    vec4 N = normalize(vec4(normal * vec4(crossEdge, 0.0f)));

    mat4 TBN = mat4(T, B, N, vec4(0.0,0.0,0.0,0.1));
    // TBN is an orthogonal matrix and so its inverse is equal to its transpose
    TBN = transpose(TBN);

    //----------------------------------------------------------------------------


    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Ponto onde está localizada a fonte de luz
    vec4 l_light_point = vec4(-14.0,1.0,15.5,1.0);

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd = vec3(0.698039,0.698039,0.698039); // Refletância difusa
    vec3 Ks = vec3(0.0,0.0,0.0); // Refletância especular
    vec3 Ka = vec3(0.0, 0.0, 0.0); // Refletância ambiente
    float q = 9.84916; // Expoente especular para o modelo de iluminação de Phong

    // Coordenadas de textura U e V, obtidas do arquivo OBJ.
    float U = texcoords.x;
    float V = texcoords.y;


    if ( object_id == CAMPFIRE )
    {
        // Propriedades espectrais
        Ks = vec3(0.222727,0.222727,0.222727);
        Ka = vec3(0.5, 0.5, 0.5);
        q = 1000;
        /*n = vec4(normalize(texture(TextureNormalCampfire, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }
    else if ( object_id == FIRE )
    {
        // Propriedades espectrais
        Kd = vec3(0.0,0.0,0.0);
        Ks = vec3(1.0,1.0,1.0);
        Ka = vec3(0.5, 0.5, 0.5);
        q = 1000;
    }
    else if ( object_id == CAVE_WALLS )
    {
        /*n = vec4(normalize(texture(TextureNormalCaveWalls, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }
    else if ( object_id == CAVE_FLOOR )
    {
        /*n = vec4(normalize(texture(TextureNormalCaveFloor, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }
    else if ( object_id == GRUTA_BASE )
    {
        // Propriedades espectrais
        Kd = vec3(0.8,0.8,0.8);
        Ka = vec3(1.0, 1.0, 1.0);
        q = 0.0;
        /*n = vec4(normalize(texture(TextureNormalGruta, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }
    else if ( object_id == PRISIONER )
    {
        // Propriedades espectrais
        Ks = vec3(0.5,0.5,0.5);
        Ka = vec3(1.0, 1.0, 1.0);
        q = 0.0;
        /*n = vec4(normalize(texture(TextureNormalPrisioner, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }
    else if ( object_id == PRISIONER_EYES || object_id == PRISIONER_TEETH )
    {
        // Propriedades espectrais
        Ks = vec3(0.5,0.5,0.5);
        Ka = vec3(1.0, 1.0, 1.0);
        q = 0.0;
    }
    else if ( object_id == PRISIONER_ROCK )
    {
        // Propriedades espectrais
        Ks = vec3(0.5,0.5,0.5);
        Ka = vec3(1.0, 1.0, 1.0);
        q = 0.0;
        /*n = vec4(normalize(texture(TextureNormalPrisionerRock, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }
    else if ( object_id == PRISIONER_CHAIN )
    {
        // Propriedades espectrais
        Ks = vec3(0.5,0.5,0.5);
        Ka = vec3(1.0, 1.0, 1.0);
        q = 0.0;
        /*n = vec4(normalize(texture(TextureNormalPrisionerChain, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }
    else if ( object_id == LADDER )
    {
        // Propriedades espectrais
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(0.5,0.5,0.5);
        Ka = vec3(1.0, 1.0, 1.0);
        q = 250.0;
        /*n = vec4(normalize(texture(TextureNormalPrisionerChain, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }
    else if ( object_id == GREEK2 )
    {
        // Propriedades espectrais
        Ka = vec3(1.0, 1.0, 1.0);
        /*n = vec4(normalize(texture(TextureNormalGreek2, vec2(U,V)).rgb * 2.0f - 1.0f),0.0);
        p = vec4(TBN * normal);
        l_light_point = vec4(TBN * l_light_point);
        camera_position = vec4(TBN * camera_position);*/
    }

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(l_light_point - p);

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = vec4( ((-1)*l.x) + (2 * n.x * dot(n,l)),
                   ((-1)*l.y) + (2 * n.y * dot(n,l)),
                   ((-1)*l.z) + (2 * n.z * dot(n,l)),
                   ((-1)*l.w) + (2 * n.w * dot(n,l))
                  );

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0, 0.5, 0.5); // PREENCH AQUI o espectro da fonte de luz

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2, 0.2, 0.2); // PREENCHA AQUI o espectro da luz ambiente

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd * I * max( 0, dot(n,l) ); // PREENCHA AQUI o termo difuso de Lambert

    // Termo ambiente
    vec3 ambient_term = Ka*Ia; // PREENCHA AQUI o termo ambiente

    // “half-vector”: Meio do caminho entre v e l
    vec4 vmaisl = v+l;
    vec4 h = vmaisl / sqrt( vmaisl.x*vmaisl.x + vmaisl.y*vmaisl.y + vmaisl.z*vmaisl.z );

    // q'
    float q_linha = 1000 - q;

    // Termo especular utilizando o modelo de iluminação de Blinn-Phong
    vec3 blinn_phong_specular_term  = Ks * I * pow( max( 0, dot(n,h) ), q_linha ); // PREENCH AQUI o termo especular de Phong


    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
    vec3 KdCampfire = texture(TextureImageCampfire, vec2(U,V)).rgb;
    vec3 KdFire = texture(TextureImageFire, vec2(U,V)).rgb;
    vec3 KdCaveWalls = texture(TextureImageCaveWalls, vec2(U,V)).rgb;
    vec3 KdCaveFloor = texture(TextureImageCaveFloor, vec2(U,V)).rgb;
    vec3 KdGruta = texture(TextureImageGruta, vec2(U,V)).rgb;
    vec3 KdGreekBody = texture(TextureImageGreekBody, vec2(U,V)).rgb;
    vec3 KdGreekHead = texture(TextureImageGreekHead, vec2(U,V)).rgb;
    vec3 KdTitle = texture(TextureImageTitle, vec2(U,V)).rgb;
    vec3 KdPrisioner = texture(TextureImagePrisioner, vec2(U,V)).rgb;
    vec3 KdPrisionerEyes = texture(TextureImagePrisionerEyes, vec2(U,V)).rgb;
    vec3 KdPrisionerTeeth = texture(TextureImagePrisionerTeeth, vec2(U,V)).rgb;
    vec3 KdPrisionerRock = texture(TextureImagePrisionerRock, vec2(U,V)).rgb;
    vec3 KdPrisionerChain = texture(TextureImagePrisionerChain, vec2(U,V)).rgb;
    vec3 KdLadder = texture(TextureImageLadder, vec2(U,V)).rgb;
    vec3 KdGreek2 = texture(TextureImageGreek2, vec2(U,V)).rgb;

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));

    if ( object_id == CAMPFIRE )
    {
        color.rgb = KdCampfire * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == FIRE )
    {
        color.rgb = KdFire * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == CAVE || object_id == CAVE_WALLS )
    {
        color.rgb = KdCaveWalls * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == CAVE_FLOOR )
    {
        color.rgb = KdCaveFloor * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == GRUTA_BASE )
    {
        color.rgb = KdGruta * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == GREEK_BODY )
    {
        color.rgb = KdGreekBody * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == GREEK_HEAD )
    {
        color.rgb = KdGreekHead * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == TITLE )
    {
        color.rgb = KdTitle * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == PRISIONER )
    {
        color.rgb = KdPrisioner * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == PRISIONER_EYES )
    {
        color.rgb = KdPrisionerEyes * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == PRISIONER_TEETH )
    {
        color.rgb = KdPrisionerTeeth * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == PRISIONER_ROCK )
    {
        color.rgb = KdPrisionerRock * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == PRISIONER_CHAIN )
    {
        color.rgb = KdPrisionerChain * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == LADDER )
    {
        color.rgb = KdLadder * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }
    else if ( object_id == GREEK2 )
    {
        color.rgb = KdGreek2 * (lambert_diffuse_term + ambient_term + blinn_phong_specular_term);
    }


    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}

