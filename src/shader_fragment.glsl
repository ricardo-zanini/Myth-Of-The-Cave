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

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define SPHERE 0
#define BUNNY  1
#define PLANE  2
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;
uniform sampler2D TextureImage6;
uniform sampler2D TextureImage7;
uniform sampler2D TextureImage8;
uniform sampler2D TextureImage9;
uniform sampler2D TextureImage10;
uniform sampler2D TextureImage11;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
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
    vec4 l_light_point = vec4(0.0,2.0,1.0,1.0);

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

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

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd = vec3(1.0, 1.0, 1.0); // Refletância difusa
    vec3 Ks = vec3(1.0, 1.0, 1.0); // Refletância especular
    vec3 Ka = vec3(1.0, 1.0, 1.0); // Refletância ambiente
    float q = 100; // Expoente especular para o modelo de iluminação de Phong

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    if ( object_id == SPHERE )
    {
        // PREENCHA AQUI as coordenadas de textura da esfera, computadas com
        // projeção esférica EM COORDENADAS DO MODELO. Utilize como referência
        // o slides 134-150 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // A esfera que define a projeção deve estar centrada na posição
        // "bbox_center" definida abaixo.

        // Você deve utilizar:
        //   função 'length( )' : comprimento Euclidiano de um vetor
        //   função 'atan( , )' : arcotangente. Veja https://en.wikipedia.org/wiki/Atan2.
        //   função 'asin( )'   : seno inverso.
        //   constante M_PI
        //   variável position_model

        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        vec4 p_linha = bbox_center + ( (position_model - bbox_center) / length(position_model - bbox_center) );

        vec4 p_vetor = p_linha - bbox_center;

        float theta = atan(p_vetor.x, p_vetor.z);

        float phi = asin(p_vetor.y);

        U = (theta + M_PI) / (2 * M_PI);

        V = (phi + (M_PI_2) ) / M_PI;

    }
    else if ( object_id == BUNNY )
    {
        // Propriedades espectrais do coelho
        Kd = vec3(1.0, 1.0, 1.0);
        Ks = vec3(1.0, 1.0, 1.0);
        Ka = vec3(0.0, 0.0, 0.0);
        q = 100.0;

        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        U = texcoords.x;
        V = texcoords.y;
    }
    else if ( object_id >= PLANE )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        U = texcoords.x;
        V = texcoords.y;
    }

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0, 1.0, 1.0); // PREENCH AQUI o espectro da fonte de luz

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2, 0.2, 0.2); // PREENCHA AQUI o espectro da luz ambiente

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd * I * max( 0, dot(n,l) ); // PREENCHA AQUI o termo difuso de Lambert

    // Termo ambiente
    vec3 ambient_term = Ka*Ia; // PREENCHA AQUI o termo ambiente

    // Termo especular utilizando o modelo de iluminação de Phong
    vec3 phong_specular_term  = Ks * I * pow( max( 0, dot(r,v) ), q ) * max( 0, dot(n,l) ); // PREENCH AQUI o termo especular de Phong


    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
    vec3 Kd0 = texture(TextureImage0, vec2(U,V)).rgb;
    vec3 Kd1 = texture(TextureImage1, vec2(U,V)).rgb;
    vec3 Kd2 = texture(TextureImage2, vec2(U,V)).rgb;
    vec3 Kd3 = texture(TextureImage3, vec2(U,V)).rgb;
    vec3 Kd4 = texture(TextureImage4, vec2(U,V)).rgb;
    vec3 Kd5 = texture(TextureImage5, vec2(U,V)).rgb;
    vec3 Kd6 = texture(TextureImage6, vec2(U,V)).rgb;
    vec3 Kd7 = texture(TextureImage7, vec2(U,V)).rgb;
    vec3 Kd8 = texture(TextureImage8, vec2(U,V)).rgb;
    vec3 Kd9 = texture(TextureImage9, vec2(U,V)).rgb;
    vec3 Kd10 = texture(TextureImage10, vec2(U,V)).rgb;
    vec3 Kd11 = texture(TextureImage11, vec2(U,V)).rgb;

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));

    if ( object_id == SPHERE )
    {
        color.rgb = Kd0 * (lambert + 0.01) + Kd1 * max(0,(1.0 - (lambert*4)));
    }
    else if ( object_id == BUNNY )
    {
        color.rgb = Kd0 * (lambert + 0.01);
    }
    else
    {
        color.rgb = Kd1 * (lambert + 0.01);
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

